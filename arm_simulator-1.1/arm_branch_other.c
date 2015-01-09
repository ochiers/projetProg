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


/*Implémentation des instructions B et BL*/
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
	
    arm_write_register(p, PC, target_address);
	
	return SUCCESS;
}	


int arm_miscellaneous(arm_core p, uint32_t ins) {
    uint32_t  bits27_20=get_bits(ins, 28, 20);
	uint32_t  bits7_4=get_bits(ins, 8, 4);
	
	uint32_t champ_27a23=get_bits(ins, 28, 23);
	uint32_t champ_21a20=get_bits(ins, 22, 20); 

	if( (bits27_20==18) && (bits7_4==3) ) { //respectivement 00010010 et 0011 BLX
		return arm_branch_X( p, ins );
	}else if ( champ_27a23==2 && champ_21a20==0 ) {
		return arm_mrs( p, ins);
	}else if ( champ_27a23==6 && champ_21a20==2 ) {
		return arm_msr_immediate_operand( p, ins );
	}else if ( champ_27a23==2 && champ_21a20==2 && bits7_4==0 ) {
		return arm_msr_register_operand( p, ins);
	}else{
		return UNDEFINED_INSTRUCTION;
	}
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




int arm_msr_immediate_operand(arm_core p, uint32_t ins){
	return UNIMPLEMENTED_INSTRUCTION;
}
int arm_msr_register_operand(arm_core p, uint32_t ins){
	return UNIMPLEMENTED_INSTRUCTION;
}
int arm_mrs(arm_core p, uint32_t ins){
	return UNIMPLEMENTED_INSTRUCTION;
}





/* Implémentation de l'instruction BLX: branchement à l'adresse contenue dans un registre
Attention !!! BLX vers une addresse impaire enclenche le jeu d'instruction Thumb
Cette partie du processeur n'est pas simulé, option non disponible */
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
			CPSR_courant=set_bit(CPSR_courant, CPSR_T);			//// CPSR T bit = target[0]
		} else {											//
			CPSR_courant=clr_bit(CPSR_courant, CPSR_T);			//
		}													//	
		PC_courant = arm_read_register(p, PC);	//
		arm_write_register(p, LR, PC_courant);	// LR =  address of instruction after the BLX instruction
		

		arm_write_cpsr(p, CPSR_courant);						//

		arm_write_register(p, PC, target_address & 0xFFFFFFFE);	// PC = target AND 0xFFFFFFFE
		
		
		print_information_instruction_BLX( p, ins);
		
		
		
		return SUCCESS;	
		
}




uint32_t lecture_entier_immediat_signe_24bits(uint32_t instruction){
	uint32_t valeur=get_bits(instruction, 24, 0);
	
	if ( valeur & (1 << 23)) { //Si le bit 23 est à 1: valeur negative, on complete avec des 1 pour passer en signé sur 32 bits
		valeur = valeur | 0xFF000000 ;
	}

	return valeur;
}


void print_information_instruction_B_BL(arm_core p,	uint32_t instruction){ 
	uint32_t cond, Link, champ_signed_immed_24;
	
	cond=get_bits(instruction, 31, 28);
	Link=get_bit(instruction, 24);
	champ_signed_immed_24=get_bits(instruction, 24, 0);
	
	printf("\t----------------------------------------\n");
	printf("\t BRANCH (AND LINK) \n");
	printf("\t\t* cond \t\t\t: "); printBin(cond, 4, 1);
	printf("\t\t* L(ink)\t\t: "); printBin(Link, 1, 1);
	printf("\t\t* signed_immed_24\t:"); printBin(champ_signed_immed_24, 24, 1);
	printf("\t----------------------------------------\n");
}

void print_information_instruction_BLX(arm_core p,	uint32_t instruction){ 
	uint32_t cond, champ_Rm;
	
	cond=get_bits(instruction, 31, 28);
	champ_Rm=get_bits(instruction, 4, 0);
	
	printf("\t----------------------------------------\n");
	printf("\t Branch Link eXchange \n");
	printf("\t\t* cond \t\t\t: "); printBin(cond, 4, 1);
	printf("\t\t* Rm\t\t\t: "); printBin(champ_Rm, 4, 1);	
	printf("\t\t* target address\t: \t %X \n", arm_read_register(p, 1)); 
	printf("\t----------------------------------------\n");
}



