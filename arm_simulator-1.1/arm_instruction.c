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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

uint32_t instruction;

static int arm_execute_instruction(arm_core p) {

	if(evaluer_condition(p)){
		megaswitchdebatard;
	}
	else{
		return 0;
	}
}

int arm_step(arm_core p) {
	int result;
		
	if(!arm_fetch(p,&instruction))return 0;
	
	result = arm_execute_instruction(p);
	if (result){
		arm_exception(p, result);
	}
	return result;
}

int evaluer_condition(arm_core p){
	
	int condition_courante, Z_courant, N_courant, C_courant, V_courant, flag_condition;
	uint32_t CPSR_courant;
	CPSR_courant = arm_read_cpsr(p);
	N_courant=get_bit(CPSR_courant,N);
	Z_courant=get_bit(CPSR_courant,Z);
	C_courant=get_bit(CPSR_courant,C);
	V_courant=get_bit(CPSR_courant,V);
	condition_courante = instruction >> 28;

	switch(condition_courante){
		case COND_EQ:
			flag_condition = (Z_courant==1);
			break;
		case COND_NE:
			flag_condition = (Z_courant==0);
			break;
		case COND_CS:
			flag_condition = (C_courant==1);
			break;
		case COND_CC:
			flag_condition = (C_courant==0);
			break;
		case COND_MI:
			flag_condition = (N_courant==1);
			break;
		case COND_PL:
			flag_condition = (N_courant==0);
			break;
		case COND_VS:
			flag_condition = (V_courant==1);
			break;
		case COND_VC:
			flag_condition = (V_courant==0);
			break;
		case COND_HI:
			flag_condition = (C_courant==1 && Z_courant==0);
			break;
		case COND_LS:
			flag_condition = (C_courant==0 && Z_courant==1);
			break;
		case COND_GE:
			flag_condition = (N_courant==V_courant==1);
			break;
		case COND_LT:
			flag_condition = (N_courant!=V_courant==1);
			break;
		case COND_GT:
			flag_condition = (Z_courant==0 && (N_courant==V_courant));
			break;
		case COND_LE:
			flag_condition = (Z_courant==1 && (N_courant!=V_courant));
			break;
		case COND_AL:
			flag_condition = 1;
			break;
		case COND_-:
			break;
		default:
			flag_condition = 0;
	}
	return flag_condition
}