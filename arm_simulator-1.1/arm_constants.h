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
#ifndef __ARM_CONSTANTS_H__
#define __ARM_CONSTANTS_H__
#include <stdint.h>

/* ARM Modes */
#define USR 0x10
#define FIQ 0x11
#define IRQ 0x12
#define SVC 0x13
#define ABT 0x17
#define UND 0x1b
#define SYS 0x1f

#define SUCCESS 0

/* ARM Exceptions (by priority) */
#define RESET                   1
#define UNDEFINED_INSTRUCTION   2
#define SOFTWARE_INTERRUPT      3
#define PREFETCH_ABORT          4
#define DATA_ABORT              5
#define INTERRUPT               6
#define FAST_INTERRUPT          7

#define UNIMPLEMENTED_INSTRUCTION -1		// Exceptions ajoutees pour le simulateur
						// (doivent etre < 0)


/* Some CPSR bits */
#define CPSR_N 31
#define CPSR_Z 30
#define CPSR_C 29
#define CPSR_V 28

#define CPSR_J 24

#define CPSR_GE_3 19
#define CPSR_GE_2 18
#define CPSR_GE_1 17
#define CPSR_GE_0 16
#define CPSR_E 9
#define CPSR_A 8
#define CPSR_I 7
#define CPSR_F 6
#define CPSR_T 5
#define CPSR_M_4 4
#define CPSR_M_3 3
#define CPSR_M_2 2
#define CPSR_M_1 1
#define CPSR_M_0 0



/* shift operations */
#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

/* Bit mask constants for msr */
/* We simulate architecture v5T */
#define UnallocMask 0x0FFFFF00
#define UserMask    0xF0000000
#define PrivMask    0x0000000F
#define StateMask   0x00000020


/* Code Ops 
	Intructions class - Position 27-26*/
#define ARITHMETICS_OPERATION	0
#define LOAD_STORE_OPERATION	1
#define BRANCH_OPERATION	2

/* Arithmetics Operations 
	Position CodeOp 24-21 */
#define INSTR_AND	0	//0000
#define INSTR_EOR	1	//0001
#define INSTR_SUB	2	//0010
#define INSTR_RSB	3	//0011
#define INSTR_ADD	4	//0100
#define INSTR_ADC	5	//0101
#define INSTR_SBC	6	//0110
#define INSTR_RSC	7	//0111
#define INSTR_TST	8	//1000
#define INSTR_TEQ	9	//1001
#define INSTR_CMP	10	//1010
#define INSTR_CMN	11	//1011
#define INSTR_ORR	12	//1100
#define INSTR_MOV	13	//1101
#define INSTR_BIC	14	//1110
#define INSTR_MVN	15	//1111

/* Arithmetics Operations 
	Position CodeOp 24-21 */
#define COND_EQ	0	//0000
#define COND_NE	1	//0001
#define COND_CS	2	//0010
#define COND_CC	3	//0011
#define COND_MI	4	//0100
#define COND_PL	5	//0101
#define COND_VS	6	//0110
#define COND_VC	7	//0111
#define COND_HI	8	//1000
#define COND_LS	9	//1001
#define COND_GE	10	//1010
#define COND_LT	11	//1011
#define COND_GT	12	//1100
#define COND_LE	13	//1101
#define COND_AL	14	//1110
#define COND_ZZ	15	//1111

char *arm_get_exception_name(unsigned char exception);
char *arm_get_mode_name(uint8_t mode);
char *arm_get_register_name(uint8_t reg);

#endif
