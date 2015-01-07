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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

uint32_t scaledRegisterSwitch(arm_core p, uint8_t shift, uint8_t shift_imm, uint32_t addressRn, uint32_t contentRm) {
	uint32_t index = 0;
	uint32_t cpsr = arm_read_cpsr(p);
	uint8_t bitC = get_bit(cpsr, C);
	switch (shift) {
		case 0 : /* LSL */
			index = contentRm << shift_imm;
			break;
		case 1 : /* LSR */
			(shift_imm == 0) ? (index = 0) : (index = addressRn >> shift_imm);
			break;
		case 2 : /* ASR */
			if (shift_imm == 0) {
				if (get_bit(contentRm, 31))
					index = 0xFFFFFFFF;
			}
			else
				index = asr(contentRm, shift_imm);
			break;
		case 3 : 
			/* ROR or RRX */ 		
						
									/* RRX */				 									/* ROR */
			(shift_imm == 0) ? ((index = bitC << 31) || (contentRm >> 1)) : (index = ror(contentRm, shift_imm));
			break;
	}
	return index;
}

int arm_load_store(arm_core p, uint32_t ins) {

//	uint8_t cond = get_bits(ins, 32, 28);
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
	
	uint32_t address = 0;
	uint32_t index;
	uint32_t offset12 = get_bits(ins, 12, 0);
	uint32_t addressRn = arm_read_register(p, Rn);	// Adresse de base, contenue dans Rn
	uint32_t contentRd = arm_read_register(p, Rd);
	uint32_t contentRm = arm_read_register(p, Rm);	// Offset contenu dans le registre Rm 
	
	uint8_t value8;
	uint32_t value32;
	
	if(Rn != 15 && Rm !=15 && Rn != Rm) {
	
		if (!I && P) {
			(U) ? (address = addressRn + offset12) : (address = contentRm - offset12);
		
			if (W)
				arm_write_register(p, Rn, address);
		}
		else if (I && P && shifter == 0) {
			(U) ? (address = addressRn + contentRm) : (address = addressRn - contentRm);
			
			if (W)
				arm_write_register(p, Rn, address);                                 
		}
		else if (I && P) {
		
			index = scaledRegisterSwitch(p, shift, shift_imm, addressRn, contentRm);
			
			(U) ? (address = addressRn + index) : (address = addressRn - index);
			
			if (W)
				arm_write_register(p, Rn, address);
		}
		else if (I && !P && !W && shifter == 0) {
			address = addressRn;
			(U) ? (arm_write_register(p, Rn, addressRn + contentRm)) : (arm_write_register(p, Rn, addressRn - contentRm));
		}
		else if (I && !P && !W) {
			address = addressRn;
			
			index = scaledRegisterSwitch(p, shift, shift_imm, addressRn, contentRm);
			
			(U) ? (arm_write_register(p, Rn, addressRn + index)) : (arm_write_register(p, Rn, addressRn - index));
		}
		else if (!I && !P && !W) {
			address = addressRn;
			
			(U) ? (arm_write_register(p, Rn, addressRn + offset12)) : (arm_write_register(p, Rn, addressRn - offset12));	
		}
		else return UNDEFINED_INSTRUCTION;
	}
	
	if(L) { // Load
		(B) ? (arm_read_byte(p, address, &value8)) : (arm_read_word(p, address, &value32));
		(B) ? arm_write_register(p, Rd, value8) : arm_write_register(p, Rd, value32);
	}
	else { // Store
		(B) ? (arm_write_byte(p, address, (uint8_t) contentRd)) : (arm_write_word(p, address, contentRd)) ;
	}
	
    return SUCCESS;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
