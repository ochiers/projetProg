#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#define PC_USER	15

#define PROCESSING_SHIFT	1 
#define PROCESSING_IMM_MSR	2 
#define	LOAD_STORE			3
#define MISCELLANEOUS		4
#define	BRANCH				5
#define	MULTIPLIE			6
#define	SWI					7
#define	NON_ENCORE_TRAITE	8
#define	ERREUR				0

#define SIMPLE_LOAD_STORE		0
#define MULTIPLE_LOAD_STORE		1
#define COPROCESSOR_LOAD_STORE	2
#define EXTRA_LOAD_STORE		3

//---------------------------------------------------
// Retourne:
//	 0 si la condition est mauvaise, 
//	 1 lorsque la condition est bonne, 
//	 2 si on doit executer arm_miscellaneous (contenu dans arm_branch_other)
//	 3 si erreur (ne peut theoriquement pas arriver)*/
//---------------------------------------------------
int evaluer_condition(arm_core p, uint32_t instruction) {
	int flag_condition;
	int condition_courante;
	int Z_courant, N_courant, C_courant, V_courant;
	uint32_t CPSR_courant;	

	CPSR_courant = arm_read_cpsr(p);	
	N_courant=get_bit(CPSR_courant,CPSR_N);
	Z_courant=get_bit(CPSR_courant,CPSR_Z);
	C_courant=get_bit(CPSR_courant,CPSR_C);
	V_courant=get_bit(CPSR_courant,CPSR_V);

	condition_courante = instruction >> 28; 
	switch (condition_courante) {
		case COND_EQ:	flag_condition = (Z_courant==1);							break;
		case COND_NE:	flag_condition = (Z_courant==0);							break;
		case COND_CS:	flag_condition = (C_courant==1);							break;
		case COND_CC:	flag_condition = (C_courant==0);							break;
		case COND_MI:	flag_condition = (N_courant==1);							break;
		case COND_PL:	flag_condition = (N_courant==0);							break;
		case COND_VS:	flag_condition = (V_courant==1);							break;
		case COND_VC:	flag_condition = (V_courant==0);							break;
		case COND_HI:	flag_condition = (C_courant==1 && Z_courant==0);			break;
		case COND_LS:	flag_condition = (C_courant==0 && Z_courant==1);			break;
		case COND_GE:	flag_condition = (N_courant==V_courant);					break;
		case COND_LT:	flag_condition = (N_courant!=V_courant);					break;
		case COND_GT:	flag_condition = (Z_courant==0 && (N_courant==V_courant));	break;
		case COND_LE:	flag_condition = (Z_courant==1 && (N_courant!=V_courant));	break;
		case COND_AL:	flag_condition = 1;											break;
		case COND_ZZ:	flag_condition = 2;											break;
		default:	flag_condition = 3; //Impossible car tout les mots appartiennent au codes
	}
	return flag_condition;
}
//---------------------------------------------------
// Retourne:
//		PROCESSING_SHIFT	1 
//		PROCESSING_IMM_MSR	2 
//		LOAD_STORE			3
//		MISCELLANEOUS		4
//		BRANCH				5
//		NON_ENCORE_TRAITE	6
//		SWI					7
//		MULTIPLIE			8
//		ERREUR				0
//---------------------------------------------------
int evaluer_categorie(arm_core p, uint32_t instruction) {
	int categorie = 0;
	uint32_t champ_categorie = (instruction >> 25) & 0x7;
	uint32_t bit4, bit5, bit6, bit7, bit20, bit21, bit23, bit24;

	bit4	= get_bit(instruction, 4);
	bit5	= get_bit(instruction, 5);
	bit6	= get_bit(instruction, 6);	
	bit7	= get_bit(instruction, 7);
	bit20	= get_bit(instruction, 20);
	bit21	= get_bit(instruction, 21);
	bit23	= get_bit(instruction, 23);
	bit24	= get_bit(instruction, 24);

	switch (champ_categorie) {
		case 0:
			if	(bit24 && !bit23 && !bit20 && !bit4)		
				categorie = MISCELLANEOUS;		//ligne 2
			else if (bit24 && !bit23 && !bit20 && !bit7 && bit4)	
				categorie = MISCELLANEOUS;		//ligne 4
			else if (bit7 && !bit6 && !bit5 && bit4)		
				categorie = MULTIPLIE;			//ligne 5
			else if (bit7 && !(!bit6 && !bit5) && bit4)		
				categorie = LOAD_STORE;			//ligne 5.5, Extra load/store
			else if (!bit4 || (bit4 && !bit7))			
				categorie = PROCESSING_SHIFT;	//ligne 1 & 3
			break;
		case 1:
			if	(bit24 && !bit23 && !bit21 && !bit20)		
				categorie = ERREUR;				//ligne 7
			else if (bit24 && !bit23 && bit21 && !bit20)		
				categorie = PROCESSING_IMM_MSR;	//ligne 8
			else							
				categorie = PROCESSING_SHIFT;	//ligne 6
			break;
		case 2:								
			categorie = LOAD_STORE;				//ligne 9
			break; 
		case 3:
			if (!bit4)						
				categorie = LOAD_STORE;			//ligne 10
			else							
				categorie = NON_ENCORE_TRAITE;	//ligne 11 & 12
			break;
		case 4:								
			categorie = LOAD_STORE;				//ligne 13
			break; 
		case 5:
			categorie = BRANCH;					//ligne 14
			break;
		case 6:
			categorie = NON_ENCORE_TRAITE;		//ligne 15
			break; 
		case 7:
			if (bit24)						
				categorie = SWI;				//ligne 18
			else							
				categorie = NON_ENCORE_TRAITE;	//ligne 16 & 17
			break; 
		default:
			categorie = NON_ENCORE_TRAITE;		//ligne 16 & 17 & 18
	}
	return categorie;
}
//---------------------------------------------------
// Retourne: 
//		SIMPLE_LOAD_STORE		0
//		MULTIPLE_LOAD_STORE		1
//		COPROCESSOR_LOAD_STORE	2
//		EXTRA_LOAD_STORE		3
//---------------------------------------------------
int sous_categorie_load_store(uint32_t instruction) {
	int resultat = -1;
	uint32_t bit4, bit5, bit6, bit7, bit25, bit26, bit27;

	bit4	= get_bit(instruction, 4);
	bit5	= get_bit(instruction, 5);
	bit6	= get_bit(instruction, 6);	
	bit7	= get_bit(instruction, 7);
	bit25	= get_bit(instruction, 25);
	bit26	= get_bit(instruction, 26);
	bit27	= get_bit(instruction, 27);

	if	(!bit27 && !bit26 && !bit25 && bit7 && !(!bit6 && !bit5) && bit4)	
		resultat = EXTRA_LOAD_STORE;		//ligne 5.5
	else if	(!bit27 && bit26 && !bit25)						
		resultat = SIMPLE_LOAD_STORE;		//ligne 9
	else if	(!bit27 && bit26 && bit25)						
		resultat = SIMPLE_LOAD_STORE;		//ligne 10
	else if (bit27 && !bit26 && !bit25)						
		resultat = MULTIPLE_LOAD_STORE;		//ligne 13
	return resultat;
}
// ------------------------------------------------------
// Affiche le code de la condition et sa definition
// ------------------------------------------------------
void affichage_condition(arm_core p, uint32_t instruction) {
	uint8_t condition = (instruction >> 28);

	printf("\t- Condition\t\t: ");	printBin(condition, 4, 0);printf("\t");
	switch (condition) {
		case 0:	printf("Z = 1");			break;
		case 1:	printf("Z = 0");			break;
		case 2:	printf("C = 1");			break;
		case 3:	printf("C = 0");			break;
		case 4:	printf("N = 1");			break;
		case 5:	printf("N = 0");			break;
		case 6:	printf("V = 1");			break;
		case 7:	printf("V = 0");			break;
		case 8:	printf("C = 1 && Z = 0");	break;
		case 9:	printf("C = 0 && Z = 1");	break;
		case 10:printf("N = V");			break;
		case 11:printf("N != V");			break;
		case 12:printf("Z = 0 && N = V");	break;
		case 13:printf("Z = 1 || N != V");break;
		case 14:printf("Always");			break;
		case 15:printf("miscellaneous");	break;
	}
	if (!evaluer_condition(p, instruction))
		printf("   ***** Condition non respectée****** \n");
	else 
		printf("\n");
}
// ------------------------------------------
// Execute une instruction
// ------------------------------------------
static int arm_execute_instruction(arm_core p) {
	int condition, resultat = 1, type, categorie;
	uint32_t instruction;
	uint32_t cpsr;
	uint8_t champ_categorie;

	resultat = arm_fetch(p, &instruction);
	printf("\nInstruction:\t"); printBin(instruction, 32, 1);
	printf("\t- Valeur de PC\t\t: %x\n", arm_read_register(p, 15));
	if (resultat == -1) {
		printf("erreur de fetch. %d\n", resultat);
		return PREFETCH_ABORT;
	}
	affichage_condition(p, instruction);
	champ_categorie = (instruction >> 25) & 0x7;
	
	condition = evaluer_condition(p, instruction);
	categorie = evaluer_categorie(p, instruction);

	if (!condition) {
		return SUCCESS;
	} else if (condition == 1) {
		printf("\t- Instruction\t\t: "); printBin(champ_categorie, 3, 0); printf("\tFonction utilisée: ");	
		switch (categorie) {
			case PROCESSING_SHIFT:
				printf("arm_data_processing_shift\n");			
				resultat = arm_data_processing_shift(p, instruction);
				break;
			case PROCESSING_IMM_MSR:
				printf("arm_miscellanous\n");	
				resultat = arm_miscellaneous(p, instruction);	
				break;
			case LOAD_STORE:
				type = sous_categorie_load_store(instruction);
				switch (type) {
					case SIMPLE_LOAD_STORE:
						printf("arm_load_store\n");
						resultat = arm_load_store(p, instruction);				
						break;
					case MULTIPLE_LOAD_STORE:
						printf("arm_load_store_multiple\n");					
						resultat = arm_load_store_multiple(p, instruction);		
						break;
					case EXTRA_LOAD_STORE:
						printf("arm_load_store\n");
						resultat = arm_load_store(p, instruction);				
						break;
					case COPROCESSOR_LOAD_STORE:
						printf("arm_coprocessor_load_store\n");
						resultat = arm_coprocessor_load_store(p, instruction);	
						break;						
				}
			break;
			case BRANCH:
				printf("arm_branch\n");
				resultat = arm_branch(p, instruction);	
				break;
			case MISCELLANEOUS:
				printf("arm_miscellaneous\n");
				resultat = arm_miscellaneous(p, instruction);	
				break;
/*			case MULTIPLIE:
				type = sous_categorie_branch_autres(instruction);
				switch(type){
					non implementé
				}
				break;
*/			
			case SWI:
				printf("arm_coprocessor_others_swi\n");
				resultat = arm_coprocessor_others_swi(p, instruction);
				break;
			case NON_ENCORE_TRAITE:	
				resultat = UNIMPLEMENTED_INSTRUCTION;
				break;
			case ERREUR:		
				resultat = UNDEFINED_INSTRUCTION;	
				break;
		}
	} else if (condition == 2) {
		printf("\t- Instruction\t\t: "); printBin(champ_categorie, 3, 0); printf("\tFonction utilisée: ");
		printf("arm_miscellaneous\n");
		resultat = arm_miscellaneous(p, instruction);
	}
	if (resultat == SUCCESS) {
		cpsr = arm_read_cpsr(p);
		printf("\t- Resultat:\n");
		printf("\t\t* Mode\t\t: ");	printBin((cpsr & 0x1F), 5, 0); printf("\t%s\n", arm_get_mode_name(cpsr & 0x1F));
		printf("\t\t* NZCV\t\t: ");	printBin((cpsr >> 28), 4, 1);
	}
	return resultat;
}
// ------------------------------------------
// Fonction principale
// Execute une instruction
// ------------------------------------------
int arm_step(arm_core p) {
	int result = arm_execute_instruction(p);
	if (result > 0)	{ 			// Exception du processeur
		arm_exception(p, result);
	} else if (result < 0) { 	// Exceptions personelles
		switch (result)	{
			case UNIMPLEMENTED_INSTRUCTION:	
				printf("\t- Instruction non implantee\n");	
				break;
			case UNPREDICTABLE:
				printf("\t- Comportement indéfini \n");	
				break;				
			
			default: 
				printf("*** Code exception non pris en charge: %d ***\n", result);
		}
		result = SUCCESS;
	}
	return result;
}
