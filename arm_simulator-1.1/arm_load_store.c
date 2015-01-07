/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

int arm_load_store(arm_core p, uint32_t ins) {

	uint8_t cond = get_bits(ins, 32, 28);
	uint8_t I = get_bit(ins, 25);	
	uint8_t P = get_bit(ins, 24);
	uint8_t U = get_bit(ins, 23);
	uint8_t B = get_bit(ins, 22);
	uint8_t W = get_bit(ins, 21);
	uint8_t L = get_bit(ins, 20);
	uint8_t Rn = get_bits(ins, 20, 16);
	uint8_t Rd = get_bits(ins, 16, 12);
	
	uint8_t shifter = get_bits(ins, 12, 4);
	uint8_t shift_imm = get_bits(ins, 12, 7);
	uint8_t shift = get_bits(ins, 7, 5);
	
	uint8_t Rm = get_bits(ins, 4, 0);
	
	uint32_t address = arm_read_register(p, Rn); // Contenu de Rn
	
	if(L) { // Load
		
	}
	else { // Store
		
	}
	
    return UNDEFINED_INSTRUCTION;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
