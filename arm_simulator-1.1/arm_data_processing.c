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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

// ------------------------------------------
// Lit les valeur des registres dans l'instruction
// Parametre d'entree:
//	- p:		etat du processeur
//	- ins:		instruction
// Parametre de sortie:
//	- o1, o2:	Valeur des operandes 1 et 2
//	- rd:		numero du registre destination
//	- S: 		indique si les codes conditions doivent etre mis a jours
// Rend -1 en cas d'erreur et 0 si non
// ------------------------------------------
char readOperand(arm_core p, uint32_t ins, uint32_t *o1, uint32_t *o2, uint8_t *rd, uint8_t *S)
{
	uint8_t irn, I;

	*rd	= (ins & 0x00F000) >> 12;				// Calcule numero registre dest
	irn	= (ins & 0x0F0000) >> 16;				// Calcule numero registre src 1
	*S	= (ins & 0x100000) >> 20;				// Calcule le bit S

	*o1 = arm_read_register(p, irn);				// Calcule Operande 1
	I = ins & 0x3000000;
	if (I)	*o2 = rd;						// Calcule Operande 2
	else
	{
		if ((rd < 0) || (rd > 15))	return -1;	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire
		*o2 = arm_read_register(p, rd);
	}
	return 0;
}
// ------------------------------------------
// Ecrit le code condition dans le registre CPSR de p
//	- cond:	valeur des flag ZNCV: Z en 0, N en 1 ....
// ------------------------------------------
void writeCondition(arm_core p, uint8_t cond)
{
	if (cond & 0x1)	set_bit(p->cpsr, Z);
	else		clr_bit(p->cpsr, Z);
	if (cond & 0x2)	set_bit(p->cpsr, N);
	else		clr_bit(p->cpsr, N);
	if (cond & 0x4)	set_bit(p->cpsr, C);
	else		clr_bit(p->cpsr, C);
	if (cond & 0x8)	set_bit(p->cpsr, V);
	else		clr_bit(p->cpsr, V);
}
// ------------------------------------------
// Calcule o1 op o2
// Parametre d'entree:
//	- o1, o2:	Valeur des operandes 1 et 2
// Parametre de sortie:
//	- res:	resultat du calcul
//	- cond:	valeur des flag ZNCV: Z en 0, N en 1 ....
// ------------------------------------------
void add(uint32_t o1, uint32_t o2, uint32_t *res, uint8_t cond)
{
	uint64_t r = (uint64_t)o1 + (uint64_t)o2;

	*cond = 0;
	if (r == 0)	*cond |= 0x1;		// Condition Z
/////	if (r)	*cond |= 0x2;			// Condition N			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire
/////	if ()	*cond |= 0x4;			// Condition C			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire
	if (r >0xFFFFFFFF)	*cond |= 0x8;	// Condition V
	*res = (uint32_t)r;
}

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins)
{
	uint32_t o0, o1, res;
	uint8_t rd, S, cond;
	char test;

	test = readOperand(p, ins, &o0, &o1, &rd, &S);

	if (test == -1)	return UNDEFINED_INSTRUCTION;	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire
	switch(i)
	{
		case INSTR_ADD:	test = add(o1, o2, *res, *cond);	break;
		case INSTR_EOR:	test = eor(o1, o2, *res, *cond);	break;
		case INSTR_SUB:	test = sub(o1, o2, *res, *cond);	break;
		default return UNDEFINED_INSTRUCTION;
	}
	if (test == -1)	return UNDEFINED_INSTRUCTION;	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire
	arm_write_register(p, rd, res);
	if (S)	writeCondition(p, cond);
	return 0; 			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire

}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}
