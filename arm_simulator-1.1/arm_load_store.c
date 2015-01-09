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
	uint8_t bitC = get_bit(cpsr, CPSR_C);
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
			/* ROR or RRX */		/* RRX */ 									/* ROR */
			(shift_imm == 0) ? ((index = bitC << 31) || (contentRm >> 1)) : (index = ror(contentRm, shift_imm));
			break;
	}
	return index;
}

int arm_load_store(arm_core p, uint32_t ins) {

	int result = 0;

	uint8_t M = get_bit(ins, 26);
	uint8_t I = get_bit(ins, 25);	
	uint8_t P = get_bit(ins, 24);
	uint8_t U = get_bit(ins, 23);
	uint8_t B = get_bit(ins, 22);
	uint8_t W = get_bit(ins, 21);
	uint8_t L = get_bit(ins, 20);
	uint8_t Rn = get_bits(ins, 20, 16);
	uint8_t Rd = get_bits(ins, 16, 12);
	uint8_t Rm = get_bits(ins, 4, 0);
	
	uint32_t addressRn = arm_read_register(p, Rn);	// Adresse de base, contenue dans Rn
	uint32_t contentRm = arm_read_register(p, Rm);	// Offset contenu dans le registre Rm
	
	uint8_t value8;
	uint32_t value32;
	
	/* Variables pour Load/Store simple */
	
	uint8_t shifter = get_bits(ins, 12, 4);
	uint8_t shift_imm = get_bits(ins, 12, 7);
	uint8_t shift = get_bits(ins, 7, 5);
	
	uint32_t address = 0;
	uint32_t index;
	uint32_t offset12 = get_bits(ins, 12, 0);
	uint32_t contentRd = arm_read_register(p, Rd);
	
	/* Variables pour Miscellaneous Load/Store */
	
	uint8_t S = get_bit(ins, 6);
	uint8_t H = get_bit(ins, 5);
	uint8_t immedH = get_bits(ins, 12, 8);
	uint8_t immedL = get_bits(ins, 4, 0);
	uint8_t offset8 = 0;
	
	uint16_t value16;
	printf("........................................... ARM_LOAD_STORE ...........................................\n");
	printf("	- M : %d\n", M);
	printf("	- I : %d\n", I);
	printf("	- P : %d\n", P);
	printf("	- U : %d\n", U);
	printf("	- B : %d\n", B);
	printf("	- W : %d\n", W);
	printf("	- L : %d\n", L);
	printf("	- Rn : %d\n", Rn);
	printf("	- Rd : %d\n", Rd);
	printf("	- Rm : %d\n", Rm);
	printf("	- addressRn : %d\n", addressRn);
	printf("	- contentRm : %d\n", contentRm);
	printf("	- contentRd : %d\n", contentRm);
	
	if(M == 1 && Rm !=15 && Rn != Rm) { // Load/Store simple
	printf("	........................................ Load/Store Simple ........................................\n");
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
		
		if(L) { // Load
		printf("			------------ Load ------------\n");
			if (B) {
				result = arm_read_byte(p, address, &value8);
				arm_write_register(p, Rd, value8);
				printf("		- value8 : %d\n", value8);
			}
			else {
				result = arm_read_word(p, address, &value32);
				arm_write_register(p, Rd, value32);
				printf("		- value32 : %d\n", value32);
			}
		}
		else { // Store
		printf("			------------ Store ------------\n");
			(B) ? (arm_write_byte(p, address, (uint8_t) contentRd)) : (arm_write_word(p, address, contentRd)) ;
			printf("		- contentRd : %d\n", contentRd);
		}
		
	}
	else if (!M) { //Miscellaneous Load/Store
	printf("	........................................ Miscellaneous Load/Store ........................................\n");
		if(!I && P && B) {
			if (Rn == 15) {
				if (!W)
					address += 8;
			}
			offset8 = (immedH << 4) || immedL;
			(U) ? (address += (addressRn + offset8)) : (address += (addressRn - offset8));
			
		}
		else if (!I && P && !B && !(Rm == 15 || (W && Rn == 15))) {
			(U) ? (address = (addressRn + contentRm)) : (address = (addressRn - contentRm));
			
		}
		else if (!I && !P && B && Rn != 15) {
			address = addressRn;
			offset8 = (immedH << 4) || immedL;
			(U) ? (arm_write_register(p, Rn, addressRn + offset8)) : (arm_write_register(p, Rn, addressRn - offset8)) ;
		}
		else if (!I && !P && !B && Rn != Rm) {
			address = addressRn;
			(U) ? (arm_write_register(p, Rn, addressRn + contentRm)) : (arm_write_register(p, Rn, addressRn - contentRm)) ;
		}
		
		if (!L && !S && H) { /* Store Halfword */
			result = arm_write_half(p, address, (uint16_t) contentRd);
		}
		else if (!L && S && !H) { /* Load Doubleword */
			result = arm_read_word(p, address, &value32);
			arm_write_register(p, Rd, value32);
		}
		else if (!L && S && H) { /* Store Doubleword */
			result = arm_write_word(p, address, contentRd);
		}
		else if (L && !S && H) { /* Load Unsigned Halfword */
			result = arm_read_half(p, address, &value16);
			arm_write_register(p, Rd, value16);
		}
		else if (L && S && H) { /* Load signed Byte */
			result = arm_read_byte(p, address, &value8);
			arm_write_register(p, Rd, value8);
		}
		else if (L && S && H) { /* Load signed Halfword */
			result = arm_read_half(p, address, &value16);
			arm_write_register(p, Rd, value16);
		}
	}
	else {
		printf("------------ Else ------------\n");
		return UNDEFINED_INSTRUCTION;
	}
	
	
	if (I && !B && L) { /* LDR */
		
	}
	else if (I && B && L) { /* LDRB */
	
	}
	else if (!I && L) { /* LDRH */
	
	}
	else if (I && !B && !L) { /* STR */
	
	}
	else if (I && B && !L) { /* STRB */
	
	}
	else if (!I && !L) { /* STRH */
	
	}
	
	
    return result;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
	
	int result = 0;
	
	uint8_t before = get_bit(ins, 24);
	uint8_t increment = get_bit(ins, 23);
	uint8_t S = get_bit(ins, 22);
	uint8_t W = get_bit(ins, 21);
	uint8_t L = get_bit(ins, 20);
	
	uint8_t Rn = get_bits(ins, 20, 16);	
	
	uint32_t startAddress = 0;
	uint32_t endAddress = 0;
	uint32_t addressRn = arm_read_register(p, Rn);
	
	uint8_t numberOfRegister = 0;
	uint8_t listOfRegister[16];
	
	uint32_t value32;
	
	int i = 0;
	for(i = 0; i < 16; i++) {
		listOfRegister[i] = get_bit(ins, i);
		if (listOfRegister[i])
			numberOfRegister++;
	}

	if (L && S) 
		return SUCCESS;
		
	if (!before && increment) { /* IA */
		startAddress = addressRn;
		endAddress = addressRn + (numberOfRegister * 4) - 4;
	}
	else if (before && increment) { /* IB */
		startAddress = addressRn + 4;
		endAddress = addressRn + (numberOfRegister * 4);
	}
	else if (!before && !increment) { /* DA */
		startAddress = addressRn - (numberOfRegister * 4) + 4;
		endAddress = addressRn;
	}
	else if (before && !increment) { /* DB */
		startAddress = addressRn - (numberOfRegister * 4);
		endAddress = addressRn - 4;
	}
	
	if (W)
		(increment) ? arm_write_register(p, Rn, addressRn - (numberOfRegister * 4)) : arm_write_register(p, Rn, addressRn + (numberOfRegister * 4));
		
	if (L) { /* Load */
		i = 0;
		while (numberOfRegister && startAddress != endAddress) {
			while (!listOfRegister[i] && i < 16) {
				i++;
			}
			result = arm_read_word(p, startAddress, &value32);
			arm_write_register(p, i, value32);
			startAddress += 4;
			numberOfRegister--;
		}
	}
	else { /* Store */
		i = 0;
		while (numberOfRegister && startAddress != endAddress) {
			while (!listOfRegister[i] && i < 16) {
				i++;
			}
			result = arm_write_word(p, startAddress, arm_read_register(p, i));
			startAddress += 4;
			numberOfRegister--;
		}
	}
	
    return result;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
