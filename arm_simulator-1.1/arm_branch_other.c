/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include <debug.h>
#include <stdlib.h>

#define LR 14
#define PC 15



int arm_branch(arm_core p, uint32_t ins) {
	int bit24=get_bit(ins, 24);
	uint32_t target_address;
	uint32_t PC_courant;
	
	
	print_information_instruction_B_BL(p,ins);	
	
	
	
	PC_courant = arm_read_register(p, PC);
	target_address=lecture_entier_immediat_signe_24bits(ins) << 2;  //Lecture de signed_immed_24 et multiplication par 4 car instructions sur 32bits

	target_address+=PC_courant;
	
	if ( bit24 ) {
		arm_write_register(p, LR, PC_courant-4);
	}
	printf("----------------------------- \n");
	printf("Target adress: %u \n", target_address );printBin(target_address, 32, 0);  
	printf("----------------------------- \n");

	
    arm_write_register(p, PC, target_address);
	
	return SUCCESS;
}	





// Attention !!! BLX vers une addresse impaire enclenche le jeu d'instruction Thumb
// Cette partie du processeur n'est pas simulé, option non disponible
int arm_branch_X(arm_core p, uint32_t ins){
	int champ_Rm=get_bits(ins,4,0); 
	uint32_t target_address;
	uint32_t PC_courant;
	uint32_t CPSR_courant;
		
		target_address = arm_read_register(p, champ_Rm); //target=Rm
		
		//Remarque: target[0] ne doit pas valoir 1 (voir spécification
		CPSR_courant = arm_read_cpsr(p);					//
		if (get_bit(target_address,0)){						//
			return UNIMPLEMENTED_INSTRUCTION;				//
			CPSR_courant=set_bit(CPSR_courant, 0);			//// CPSR T bit = target[0]
		} else {											//
			CPSR_courant=clr_bit(CPSR_courant, 0);			//
		}													//	
		PC_courant = arm_read_register(p, PC);	//
		arm_write_register(p, LR, PC_courant);	// LR =  address of instruction after the BLX instruction
		

		arm_write_cpsr(p, CPSR_courant);						//

		arm_write_register(p, PC, target_address & 0xFFFFFFFE);	// PC = target AND 0xFFFFFFFE
		
		return SUCCESS;	
		
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins) {
    if (get_bit(ins, 24)) {
        /* Here we implement the end of the simulation as swi 0x123456 */
        if ((ins & 0xFFFFFF) == 0x123456)
            exit(0);
        return SOFTWARE_INTERRUPT;
    } 
    return UNDEFINED_INSTRUCTION;
}

int arm_miscellaneous(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}






uint32_t lecture_entier_immediat_signe_24bits(uint32_t instruction){
	uint32_t valeur=get_bits(instruction, 24, 0);
	printf("blabla \n");
	printBin(valeur, 31, 0);
	printf("blabla \n");
	
	
	
	if ( valeur & (1 << 23)) { //Si le bit 23 est à 1: valeur negative, on complete avec des 1 pour passer en signé sur 32 bits
		valeur = valeur | 0xFF000000 ;
		
		
		printf("\n\n -------DEBUG ULYSSE ARM BRANCH L120----------------------\n");
		printf("valeur: \n\t\t u: %u \n\t\t s: %d \n", valeur,valeur);
		printBin(valeur, 31, 0);
		printf("\n ----------------------------- \n\n");	
		
		
	}
	


	
	return valeur;
}

void print_information_instruction_B_BL(arm_core p,	uint32_t instruction){ 
	uint32_t cond, Link, champ_signed_immed_24;
	
	cond=get_bits(instruction, 31, 28);
	Link=get_bit(instruction, 24);
	champ_signed_immed_24=get_bits(instruction, 23, 0);
	
	printf("\t----------------------------------------\n");
	printf("\t BRANCH (AND LINK) \n");
	printf("\t\t* cond \t\t\t: "); printBin(cond, 4, 1);
	printf("\t\t* L(ink)\t\t\t: "); printBin(Link, 1, 1);
	printf("\t\t* signed_immed_24\t\t:"); printBin(champ_signed_immed_24, 1, 1);
	printf("\t----------------------------------------\n");
	
	
	
}



