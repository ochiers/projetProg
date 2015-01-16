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

	int bit24 = get_bit(ins, 24);
	uint32_t target_address;
	uint32_t PC_courant;	
	
	print_information_instruction_B_BL(p,ins);	
	
	PC_courant = arm_read_register(p, PC);
	
	target_address=lecture_entier_immediat_signe_24bits(ins) << 2;  //Lecture de signed_immed_24 et multiplication par 4 car instructions sur 32bits
	target_address+=PC_courant;												//Adressage relatif a PC
	
	if (bit24) {
		arm_write_register(p, LR, PC_courant-4);				//Sauvegarde adresse de retour dans le link register (après fetch donc -4)
	}
	
    arm_write_register(p, PC, target_address);			//On force PC a l'adresse de branchement
    
	return SUCCESS;
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
																	//Remarque: target[0] ne doit pas valoir 1 (voir spécification: passage en mode Thumb)
	CPSR_courant = arm_read_cpsr(p);

	if (get_bit(target_address,0)){						//
		return UNIMPLEMENTED_INSTRUCTION;				//Mode Thumb non implémenté
		CPSR_courant=set_bit(CPSR_courant, CPSR_T);		//// CPSR T bit = target[0]
	} else {											
		CPSR_courant=clr_bit(CPSR_courant, CPSR_T);	
	}	
											
	PC_courant = arm_read_register(p, PC);	//
	arm_write_register(p, LR, PC_courant);	// LR =  address of instruction after the BLX instruction
	
	arm_write_cpsr(p, CPSR_courant);						//
	
	arm_write_register(p, PC, target_address & 0xFFFFFFFE);	// Force réalignement (inutile pour nous sert pour le passage en mode thumb)
	
	print_information_instruction_BLX( p, ins);
	
	return SUCCESS;	
}	



int arm_miscellaneous(arm_core p, uint32_t ins) {
    uint32_t  bits27_20=get_bits(ins, 28, 20);
	uint32_t  bits7_4=get_bits(ins, 8, 4);
	uint32_t champ_27a23=get_bits(ins, 28, 23);
	uint32_t champ_21a20=get_bits(ins, 22, 20); 
	
	if((bits27_20 == 18) && (bits7_4 == 3)) { //respectivement 00010010 et 0011 BLX
		printf("\t Sous fonction : arm_branch_X\n");
		return arm_branch_X(p, ins);
	} else if (champ_27a23 == 2 && champ_21a20 == 0) {
		printf("\t Sous fonction : arm_mrs\n");
		return arm_mrs(p, ins);
	} else if (champ_27a23 == 6 && champ_21a20 == 2) {
		printf("\t Sous fonction : arm_msr_immediate_operand\n");
		return arm_msr_immediate_operand(p, ins);
	} else if (champ_27a23 == 2 && champ_21a20 == 2 && bits7_4 == 0) {
		printf("\t Sous fonction : arm_msr_register_operand\n");
		return arm_msr_register_operand(p, ins);
	} else {
		return UNDEFINED_INSTRUCTION;
	}
}



// CPSR <- Rd OR CPSR <- Imm
// SPSR <- Rd OR SPSR <- Imm
int arm_msr_immediate_operand(arm_core p, uint32_t ins){
	uint32_t operande = get_bits(ins, 8, 0);
	uint32_t rotate_imm = get_bits(ins, 12, 8);
	
	printf("\n\tAvant instruction MSR (CPSR <- Imm):\n");
	print_information_instruction_MSR(p, ins);
	
	operande = (ins & 0x0FF);
	rotate_imm	= (ins & 0xF00) >> 8;
	operande = ror(operande, rotate_imm * 2);
	
	return partie_commune_msr(p, ins, operande);
}



int arm_msr_register_operand(arm_core p, uint32_t ins){
	uint32_t reg_operande = get_bits(ins, 4, 0);
	uint32_t operande;

	printf("\n\tAvant instruction MSR (CPSR <- Rd):\n");
	print_information_instruction_MSR(p, ins);

	operande = arm_read_register(p, reg_operande);
	
	return partie_commune_msr(p, ins, operande);
}

// Rd <- CPSR OR Rd <- SPSR
int arm_mrs(arm_core p, uint32_t instruction){
	uint32_t Rd = get_bits(instruction , 16, 12);
	uint8_t R = get_bit(instruction, 22); 
	
	printf("\n\tAvant instruction MRS (Rd <- CPSR OR Rd <- SPSR):\n");
	print_information_instruction_MRS(p, instruction, R);
	
	if (R) {
		arm_write_register(p, Rd, arm_read_spsr(p));
	} else {
		arm_write_register(p, Rd, arm_read_cpsr(p));
	}
	
	printf("\n\tApres instruction MRS :\n");
	print_information_instruction_MRS(p, instruction, R);
	
	return SUCCESS;
}

int partie_commune_msr(arm_core p, uint32_t ins, uint32_t operande) {
	uint32_t R = get_bit( ins, 22); 	
	uint32_t field_mask_c = get_bit(ins, 16);
	uint32_t field_mask_x = get_bit(ins, 17);
	uint32_t field_mask_s = get_bit(ins, 18);
	uint32_t field_mask_f = get_bit(ins, 19);
	uint32_t byte_mask = 0x00000000;
	uint32_t mask = 0;
	
	if ((operande & UnallocMask) == 0) {		
		if (field_mask_c)
			byte_mask = 0x000000FF;
		else
			byte_mask = 0x00000000;
		if (field_mask_x) 
			byte_mask |= 0x0000FF00;
		else 
			byte_mask |= 0x00000000;
		if (field_mask_s) 
			byte_mask |= 0x00FF0000;
		else 
			byte_mask |= 0x00000000;
		if (field_mask_f) 
			byte_mask |= 0xFF000000; 
		else 
			byte_mask |= 0x00000000;
			
		if (R == 0) {
			if (arm_in_a_privileged_mode(p)) {
				if (!(operande & StateMask)) {
					mask = byte_mask & (UserMask | PrivMask);
				}
			} else {
				mask = byte_mask & UserMask;
			}
			arm_write_cpsr(p, ((arm_read_cpsr(p) & ~mask) | (operande & mask)));
		} else { //R == 1
			if (arm_current_mode_has_spsr(p)) {
				mask = byte_mask & (UserMask | PrivMask | StateMask);
				arm_write_spsr(p, ((arm_read_spsr(p) & ~mask) | (operande & mask)));
			}
		}
	}
	
	printf("\n\tApres instruction MSR :\n");
	print_information_instruction_MSR(p, ins);	
	
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

void print_information_instruction_B_BL(arm_core p,	uint32_t instruction){ 
	uint32_t Link, champ_signed_immed_24;
	Link = get_bit(instruction, 24);
	champ_signed_immed_24=get_bits(instruction, 24, 0);
	
	printf("\t----------------------------------------\n");
	printf("\t BRANCH (AND LINK) \n");
	printf("\t\t* L(ink)\t\t: "); printBin(Link, 1, 1);
	printf("\t\t* signed_immed_24\t: (%d)", champ_signed_immed_24); printBin(champ_signed_immed_24, 24, 1);
	printf("\t----------------------------------------\n");
}

void print_information_instruction_BLX(arm_core p,	uint32_t instruction){ 
	uint32_t champ_Rm;
	champ_Rm = get_bits(instruction, 4, 0);
	printf("\t----------------------------------------\n");
	printf("\t Branch Link eXchange \n");
	printf("\t\t* Rm\t\t\t: "); printBin(champ_Rm, 4, 1);	
	printf("\t\t* target address\t: \t %X \n", arm_read_register(p, champ_Rm)); 
	printf("\t----------------------------------------\n");
}

void print_information_instruction_MSR(arm_core p, uint32_t instruction){ 
	uint32_t reg_dest, reg_cpsr, reg_spsr;
	reg_spsr = arm_read_spsr(p);
	reg_cpsr = arm_read_cpsr(p);
	reg_dest = get_bits(instruction, 4, 0);
	uint8_t R = get_bit(instruction, 22); 

	printf("\t----------------------------------------\n");
	printf("\t MSR \n");
	if (!R) {
		printf("\t\t* CPSR \t\t\t: "); 
		printBin(reg_cpsr, 32, 1);
	} else {
		printf("\t\t* SPSR \t\t\t: "); 
		printBin(reg_spsr, 32, 1);
	}	
	if (!get_bit(instruction, 25)) { //registre
		printf("\t\t* Rd \t\t\t: "); 
		printBin(arm_read_register(p, reg_dest), 32, 1);
	} else { //Imm
		printf("\t\t* Imm \t\t\t: "); 
		printBin(get_bits(instruction, 8, 0), 32, 1);	
	}
	printf("\t----------------------------------------\n");
}

void print_information_instruction_MRS(arm_core p, uint32_t instruction, uint8_t R) {
	uint32_t reg_dest, reg_cpsr, reg_spsr;
	reg_spsr = arm_read_spsr(p);
	reg_cpsr = arm_read_cpsr(p);
	reg_dest = get_bits(instruction, 16, 12);

	printf("\t----------------------------------------\n");
	printf("\t MRS \n");
	if (!R) {
		printf("\t\t* CPSR \t\t\t: "); 
		printBin(reg_cpsr, 32, 1);
	} else {
		printf("\t\t* SPSR \t\t\t: "); 
		printBin(reg_spsr, 32, 1);
	}		
	printf("\t\t* Rd \t\t\t: "); printBin(arm_read_register(p, reg_dest), 32, 1);;	
	printf("\t----------------------------------------\n");
}




uint32_t lecture_entier_immediat_signe_24bits(uint32_t instruction){
	uint32_t valeur = get_bits(instruction, 24, 0);
	if (valeur & (1 << 23)) { //Si le bit 23 est à 1: valeur negative, on complete avec des 1 pour passer en signé sur 32 bits
		valeur = valeur | 0xFF000000;
	}
	return valeur;
}


