#define PC_USER 15

#define PROCESSING 1 
#define	LOAD_STORE 2
#define	BRANCH_AUTRES 3 
#define	NON_ENCORE_TRAITE 4
#define	ERREUR 0

#define SIMPLE_LOAD_STORE 0
#define MULTIPLE_LOAD_STORE 1
#define COPROCESSOR_LOAD_STORE 2
#define EXTRA_LOAD_STORE 3

#define SHIFT_PROCESSING 0
#define IMMEDIATE_PROCESSING 1

#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include <debug.h>

////////////////////////////////////////////////////////////////////////////////
/* Retourne 0 si la condition est mauvaise, 
   			1 lorsque la condition est bonne, 
   			2 si on doit executer arm_miscellaneous (contenu dans arm_branch_other)
			3 si erreur (ne peut theoriquement pas arriver)*/
int evaluer_condition(arm_core p, uint32_t instruction){
	int flag_condition;
	int condition_courante;
	int Z_courant, N_courant, C_courant, V_courant;
	uint32_t CPSR_courant;	
	CPSR_courant = arm_read_cpsr(p);	
	N_courant=get_bit(CPSR_courant,N);
	Z_courant=get_bit(CPSR_courant,Z);
	C_courant=get_bit(CPSR_courant,C);
	V_courant=get_bit(CPSR_courant,V);
	//On prend que les 4 bits de condition
	condition_courante = instruction >> 28; 
	switch(condition_courante){
		case COND_EQ:
			flag_condition = (Z_courant==1);							break;
		case COND_NE:
			flag_condition = (Z_courant==0);							break;
		case COND_CS:
			flag_condition = (C_courant==1);							break;
		case COND_CC:
			flag_condition = (C_courant==0);							break;
		case COND_MI:
			flag_condition = (N_courant==1);							break;
		case COND_PL:
			flag_condition = (N_courant==0);							break;
		case COND_VS:
			flag_condition = (V_courant==1);							break;
		case COND_VC:
			flag_condition = (V_courant==0);							break;
		case COND_HI:
			flag_condition = (C_courant==1 && Z_courant==0);			break;
		case COND_LS:
			flag_condition = (C_courant==0 && Z_courant==1);			break;
		case COND_GE:
			flag_condition = (N_courant==V_courant);					break;
		case COND_LT:
			flag_condition = (N_courant!=V_courant);					break;
		case COND_GT:
			flag_condition = (Z_courant==0 && (N_courant==V_courant));	break;
		case COND_LE:
			flag_condition = (Z_courant==1 && (N_courant!=V_courant));	break;
		case COND_AL:
			flag_condition = 1;											break;
		case COND_ZZ:
			flag_condition = 2;											break;
		default:
			flag_condition = 3; //Impossible car tout les mots appartiennent au codes
	}
	debug("condition : %x, %d\n", condition_courante, flag_condition);
	return flag_condition;
}

////////////////////////////////////////////////////////////////////////////////
/* Retourne	PROCESSING 1
			LOAD_STORE 2
			BRANCH_AUTRES 3 
			NON_ENCORE_TRAITE 4
			ERREUR 0 */
int evaluer_categorie(arm_core p, uint32_t instruction) {
	int categorie = 0;
	// On recupere les 3 bits apres les 4 bits de cond dans champ_categorie
	uint32_t champ_categorie;
	champ_categorie = instruction >> 25;
	champ_categorie &= 0x7;		
	// On récupere le bit4, 7, 20, 21, 22, 23, 24
	uint32_t bit4, bit5, bit6, bit7, bit20, bit21, bit23, bit24;
	bit4 = get_bit(instruction, 4);
	bit5 = get_bit(instruction, 5);
	bit6 = get_bit(instruction, 6);	
	bit7 = get_bit(instruction, 7);
	bit20 = get_bit(instruction, 20);
	bit21 = get_bit(instruction, 21);
	bit23 = get_bit(instruction, 23);
	bit24 = get_bit(instruction, 24);
	switch(champ_categorie) {
		case 0:
			if (bit24 && !bit23 && !bit20 && !bit4) { //ligne 2
				categorie = BRANCH_AUTRES;
			}
			else if (!bit4 || (bit4 && !bit7)) { //ligne 1 & 3
				categorie = PROCESSING;
			}
			else if (bit24 && !bit23 && !bit20 && !bit7 && bit4) { //ligne 4
				categorie = BRANCH_AUTRES;
			}
			else if (bit7 && !bit6 && !bit5 && bit4) { //ligne 5, Multiplies
				categorie = NON_ENCORE_TRAITE;
			}
			else if (bit7 && !(!bit6 && !bit5) && bit4) { //ligne 5.5, Extra load/store
				categorie = LOAD_STORE;
			}
			break; 
		case 1:
			if (bit24 && !bit23 && !bit21 && !bit20) { //ligne 7
				categorie = ERREUR;				
			} else {
				categorie = PROCESSING; //ligne 6 & 8
			}
			break; 
		case 2:
			categorie = LOAD_STORE;	//ligne 9
			break; 
		case 3:
			if (!bit4) { //ligne 10
				categorie = LOAD_STORE;
			}
			else {
				categorie = NON_ENCORE_TRAITE; //ligne 11 & 12
			}
			break; 
		case 4:
			categorie = LOAD_STORE; //ligne 13
			break; 
		case 5:
			categorie = BRANCH_AUTRES; //ligne 14
			break; 
		case 6:
			categorie = NON_ENCORE_TRAITE; //ligne 15
			break; 
	}
	return categorie;	
}
////////////////////////////////////////////////////////////////////////////////
int sous_categorie_load_store(uint32_t instruction) { //a check avec Seb
	uint32_t champ_categorie;
	champ_categorie = instruction >> 25;
	champ_categorie &= 0x7;
	int resultat = -1;
	uint32_t bit4, bit5, bit6, bit7, bit25, bit26, bit27;
	bit4 = get_bit(instruction, 4);
	bit5 = get_bit(instruction, 5);
	bit6 = get_bit(instruction, 6);	
	bit7 = get_bit(instruction, 7);
	bit25 = get_bit(instruction, 25);
	bit26 = get_bit(instruction, 26);
	bit27 = get_bit(instruction, 27);
	if (!bit27 && !bit26 && !bit25 && bit7 && !(!bit6 && !bit5) && bit4) { //ligne 5.5
		resultat = EXTRA_LOAD_STORE;
	}
	else if (!bit27 && bit26 && !bit25) { //ligne 9
		resultat = SIMPLE_LOAD_STORE;
	}
	else if (!bit27 && bit26 && bit25) { //ligne 10
		resultat = SIMPLE_LOAD_STORE;
	}
	else if (bit27 && !bit26 && !bit25) { //ligne 13
		resultat = MULTIPLE_LOAD_STORE;
	}
	return resultat;	
}
////////////////////////////////////////////////////////////////////////////////
int sous_categorie_processing(uint32_t instruction) {
	uint32_t champ_categorie;
	champ_categorie = instruction >> 25;
	champ_categorie &= 0x7;
	int resultat = -1;
	uint32_t bit4, bit7, bit20, bit21, bit23, bit24, bit25, bit26, bit27;
	bit4 = get_bit(instruction, 4);
	bit7 = get_bit(instruction, 7);
	bit20 = get_bit(instruction, 20);
	bit21 = get_bit(instruction, 21);
	bit23 = get_bit(instruction, 23);	
	bit24 = get_bit(instruction, 24);	
	bit25 = get_bit(instruction, 25);
	bit26 = get_bit(instruction, 26);
	bit27 = get_bit(instruction, 27);
	if (!bit27 && !bit26 && !bit25 && !bit4) { //ligne 1
		resultat = SHIFT_PROCESSING;
	}
	else if (!bit27 && !bit26 && !bit25 && !bit7 && bit4) { //ligne 3
		resultat = SHIFT_PROCESSING;
	}
	else if (!bit27 && !bit26 && bit25) { //ligne 6
		resultat = SHIFT_PROCESSING;
	}
	else if (!bit27 && !bit26 && bit25 && bit24 && !bit23 && bit21 && !bit20) { //ligne 8
		resultat = IMMEDIATE_PROCESSING;
	}	
	return resultat;
}
////////////////////////////////////////////////////////////////////////////////
static int arm_execute_instruction(arm_core p) {
	int condition, resultat = 1, type, categorie;
	uint32_t instruction;	
	resultat = arm_fetch(p, &instruction);
	if(!resultat) {
		debug("erreur de fetch. %d\n", resultat);
		return 0;
	}
	debug("instruction %x\n", instruction);
	condition = evaluer_condition(p, instruction);
	categorie = evaluer_categorie(p, instruction);
	if (!condition) {
		debug("condition non respecée.\n");	
	}
	else {
		switch(categorie) {
			case PROCESSING:
				type = sous_categorie_processing(instruction);
				switch(type) {
					case SHIFT_PROCESSING:
						resultat = arm_data_processing_shift(p, instruction);			
						debug("instruction : %d\n", resultat);							break;
					case IMMEDIATE_PROCESSING:
						resultat = arm_data_processing_immediate_msr(p, instruction);	
						debug("instruction : %d\n", resultat);							break;
				}
			break;
			case LOAD_STORE:
				type = sous_categorie_load_store(instruction);
				switch(type) {
					case SIMPLE_LOAD_STORE:
						resultat = arm_load_store(p, instruction);				
						debug("instruction : %d\n", resultat);					break;
					case MULTIPLE_LOAD_STORE:
						resultat = arm_load_store_multiple(p, instruction);		
						debug("instruction : %d\n", resultat);					break;
					case EXTRA_LOAD_STORE:
						resultat = arm_load_store(p, instruction);				
						debug("instruction : %d\n", resultat);					break;
					case COPROCESSOR_LOAD_STORE:
						resultat = arm_coprocessor_load_store(p, instruction);	
						debug("instruction : %d\n", resultat);					break;						
				}
			break;
			/*case BRANCH_AUTRES:
				type = sous_categorie_branch_autres(instruction);
				switch(type){
					non implementé
				}
			break; */
			case NON_ENCORE_TRAITE:
				resultat = 1;
			break;
		}
	}
	return resultat;
}
////////////////////////////////////////////////////////////////////////////////
int arm_step(arm_core p) {
	int result;
	result = arm_execute_instruction(p);
	if (result) {
		arm_exception(p, result);
	}
	return result;
}
////////////////////////////////////////////////////////////////////////////////
