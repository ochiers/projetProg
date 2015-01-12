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
	switch(exception)
	{
		case RESET:							// Semantics of reset interrupt (ARM manual A2-18)
			arm_write_cpsr(p, 0x1d3 | Exception_bit_9);
			arm_write_usr_register(p, PC_USER, 0);
			break;
/*%%%%%%%%%%%%%%%%%%%%%%% Tous  le reste doit etre en commentaire jusqu'au test de instruction
#define UNDEFINED_INSTRUCTION	2
#define SOFTWARE_INTERRUPT      3
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
