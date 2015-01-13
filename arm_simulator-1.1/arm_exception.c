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

#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "util.h"

#ifdef BIG_ENDIAN_SIMULATOR
#define CP15_reg1_EEbit 1
#else
#define CP15_reg1_EEbit 0
#endif

#define Exception_bit_9 (CP15_reg1_EEbit << 9)

void arm_exception(arm_core p, unsigned char exception)
{
	uint32_t addr, cpsr, spsr;

	switch(exception)
	{
		case RESET:							// Semantics of reset interrupt (ARM manual A2-18)
			arm_write_cpsr(p, 0x1d3 | Exception_bit_9);
			arm_write_usr_register(p, PC_USER, 0);
			break;
		case UNDEFINED_INSTRUCTION:
			addr	= arm_read_register(p, PC_USER);
			cpsr	= arm_read_cpsr(p);
			spsr	= cpsr;
			cpsr	&= UND;						// Passer en mode UND
			cpsr	&= ~(1 << 5);					// Interpreter le code en ARM
			cpsr	|= 1 << 7;					// Interdire les interruptions
			#ifdef BIG_ENDIAN_SIMULATOR
			cpsr	|= 1 << 9;					// Bit endiane = BIG_ENDIAN
			#else
			cpsr	&= ~(1 << 9);					// Bit endiane = LITLE_ENDIAN
			#endif
			arm_write_cpsr(p, cpsr);
			arm_write_register(p, 14, addr);
			arm_write_spsr(p, spsr);
			arm_write_register(p, 15, 0x00000004);			// Ecriture de pc ~(high_vectors_configured)
			break;
		case SOFTWARE_INTERRUPT:
			
			break;
/*%%%%%%%%%%%%%%%%%%%%%%% Tous  le reste doit etre en commentaire jusqu'au test de instruction
#define PREFETCH_ABORT          4
#define DATA_ABORT              5
#define INTERRUPT               6
#define FAST_INTERRUPT          7
*/		default:
			printf("\n\n**** Traitement d'execption ****\n");
			printf("**** Exception non geree: %d ****\n", exception);
			exit(0);
	}
}
