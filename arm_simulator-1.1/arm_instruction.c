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

#define PC_USER 15

#define PROCESSING 1 
#define	LOAD_STORE 2
#define	BRANCH_AUTRES 3 
#define	NON_ENCORE_TRAITE 4
#define	ERREUR 0

#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

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
			flag_condition = 3;		//Impossible tout les mots appartiennent au codes
	}
	return flag_condition;
}


/* Retourne
PROCESSING 1
LOAD_STORE 2
BRANCH_AUTRES 3 
NON_ENCORE_TRAITE 4
ERREUR 0*/
int evaluer_categorie(arm_core p, uint32_t instruction) {

	int categorie = 0;
	
	// On recupere les 3 bits apres les 4 bits de cond
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
		case 000:
			if (bit24 && !bit23 && !bit20 && !bit4) {
				categorie = BRANCH_AUTRES;
			}
			else if (bit24 && !bit23 && !bit20 && !bit7 && bit4) {
				categorie = BRANCH_AUTRES;
			}
			else if (bit7 && !bit6 && !bit5 && bit4) { //Multiplies
				categorie = PROCESSING;
			}
			else if (bit7 && !(!bit6 && !bit5) && bit4) { //Extra load/store
				categorie = LOAD_STORE;
			}
			else if (!bit4 || (bit4 && !bit7)) {
				categorie = PROCESSING;
			}
			break; 
		case 001:
			if (bit24 && !bit23 && !bit21 && !bit20) {
				categorie = ERREUR;				
			}
			else {
				categorie = PROCESSING;
			}
			break; 
		case 010:
			categorie = LOAD_STORE;	
			break; 
		case 011:
			if (!bit4) {
				categorie = LOAD_STORE;
			}
			else {
				categorie = NON_ENCORE_TRAITE; // Media instruction & Architectually undefined
			}
			break; 
		case 100:
			categorie = LOAD_STORE;
			break; 
		case 101:
			categorie = BRANCH_AUTRES;
			break; 
		case 110:
			categorie = NON_ENCORE_TRAITE;
			break; 
	}	
	return categorie;	
}

static int arm_execute_instruction(arm_core p) {
	int condition, categorie, resultat = 0;
	uint32_t instruction;	
	resultat = arm_fetch(p, &instruction);
	if(!resultat) return 0;
	condition = evaluer_condition(p, instruction);
	categorie = evaluer_categorie(p, instruction);	
	
	if (condition == 0xF) {
		resultat = arm_miscellaneous(p, instruction);
	}
	else {
		switch (categorie) {
			case 1: //Processim
				resultat = arm_data_processing_shift(p, instruction);	
				break;
			case 2: //load/store
				//resultat = 
				break;
			case 3: //branch
				//resultat = 
				break;
			case 0: //erreur
				resultat = 0;
				break;
			default:
				resultat = 0;			
		}
	}
	return resultat;
}

int arm_step(arm_core p) {
	int result;
	result = arm_execute_instruction(p);
	if (result) {
		arm_exception(p, result);
	}
	return result;
}
