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
char readOperand(arm_core p, uint32_t ins, uint64_t *o1, uint64_t *o2, uint8_t *rd, uint8_t *S, uint8_t *op)
{
	uint8_t irn, I, rotate_imm;
	uint32_t immed;

	*rd	= (ins & 0x000F000) >> 12;				// Calcule numero registre dest
	irn	= (ins & 0x00F0000) >> 16;				// Calcule numero registre src 1
	*S	= (ins & 0x0100000) >> 20;				// Calcule le bit S
	*op	= (ins & 0x1E00000) >> 21;				// Calcule le code operation
	*o1	= (uint64_t)arm_read_register(p, irn);			// Calcule Operande 1

	I = (ins & 0x2000000) >> 25;					// Calcule Operande 2		// *********************** A completer: voir p 442 et 443
	if (I)
	{
		immed		= (ins & 0x0FF);
		rotate_imm	= (ins & 0xF00) >> 8;
		
	}
	else
	{
		if ((rd < 0) || (rd > 15))	return -1;	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire
		*o2 = arm_read_register(p, rd);
	}
	return 0;
}
// ------------------------------------------
// Ecrit le code condition dans le registre
// CPSR de p en fonction du resultat du calcul res
// ------------------------------------------
void writeCondition(arm_core p, uint64_t res)
{
	if (res == 0)		set_bit(p->cpsr, Z);
	else			clr_bit(p->cpsr, Z);
	if (res & 0x080000000)	set_bit(p->cpsr, N);
	else			clr_bit(p->cpsr, N);
	if (res & 0x100000000)	set_bit(p->cpsr, C);
	else			clr_bit(p->cpsr, C);
	if (res > 0x0FFFFFFFF)	set_bit(p->cpsr, V);
	else			clr_bit(p->cpsr, V);
}
// ------------------------------------------
// Calcule o1 op o2
// Parametre d'entree:
//	- o1, o2:	Valeur des operandes 1 et 2 sur 64 bit (32 bit + des 0)
// Parametre de sortie:
//	- res:	resultat du calcul sur 64 bits
// ------------------------------------------
uint64_t eor(uint64_t o0, uint64_t o1)
{
	uint64_t res = 0;
	char i, b0, b1;

	for (i=0; i<32; i++)
	{
		b0 = get_bit(o0, i);
		b1 = get_bit(o1, i);
		res |= (((b0 | b1) & ~(b0 & b1)) >> i);
	}
	return res;
}
// ------------------------------------------
// Decoding functions for different classes of instructions
// ------------------------------------------
int arm_data_processing_shift(arm_core p, uint32_t ins)
{
	uint64_t res=0, o0, o1;
	uint8_t rd, S, cond, op;
	char test;

	test = readOperand(p, ins, &o0, &o1, &rd, &S, &op);

	if (test == -1)	return UNDEFINED_INSTRUCTION;	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire
	switch(i)
	{
		case INSTR_AND:	res = o0 & o1;				break;
		case INSTR_EOR:	res = eor(o0, o1);			break;
		case INSTR_SUB:	res = o0 - o1;				break;
		case INSTR_RSB:	res = o1 - o0;				break;
		case INSTR_ADD:	res = o0 + o1;				break;
		case INSTR_ADC:	res = o0 + o1 + (p->cpsr & (1<<C));	break;
		case INSTR_SBC:	break;				// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% voir page 275
		case INSTR_RSC:	test = rsc(o1, o2, *res, *cond);	break;
		case INSTR_TST:	test = tst(o1, o2, *res, *cond);	break;
		case INSTR_TEQ:	test = teq(o1, o2, *res, *cond);	break;
		case INSTR_CMP:	test = cmp(o1, o2, *res, *cond);	break;
		case INSTR_CMN:	test = cmn(o1, o2, *res, *cond);	break;
		case INSTR_ORR:	test = orr(o1, o2, *res, *cond);	break;
		case INSTR_MOV:	test = mov(o1, o2, *res, *cond);	break;
		case INSTR_BIC:	test = bic(o1, o2, *res, *cond);	break;
		case INSTR_MVN:	test = mvn(o1, o2, *res, *cond);	break;
		default return UNDEFINED_INSTRUCTION;
	}
	if (test == -1)	return UNDEFINED_INSTRUCTION;	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire
	arm_write_register(p, rd, (uint32_t)res);
	if (S)	writeCondition(p, res);		// %%%%%%%%%%%%%%%%%%%%%% A corriger voir page 157
	return 0; 			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}
