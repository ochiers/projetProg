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
// Decoding functions for different classes of instructions
// ------------------------------------------
int arm_data_processing_shift(arm_core p, uint32_t ins)
{
	uint32_t res=0, o0, o1;
	uint8_t rd, S, cond, op, shifter_carry_out;
	char test;

	test = readOperand(p, ins, &o0, &o1, &shifter_carry_out, &rd, &S, &op);

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
	arm_write_register(p, rd, res);
	if (S)
	{
		test = writeCondition(p, rd, res);
		if (!test) return UNPREDICTABLE;
	}
	return SUCCESS;
}
int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% a faire
    return UNDEFINED_INSTRUCTION;
}
// ------------------------------------------
// Lit les valeur des registres dans l'instruction
// Parametre d'entree:
//	- p:		etat du processeur
//	- ins:		instruction
// Parametre de sortie:
//	- o0, o1:	Valeur des operandes 0 et 1
//	- rd:		numero du registre destination
//	- S: 		indique si les codes conditions doivent etre mis a jours
// Rend -1 en cas d'erreur et 0 si non
// ------------------------------------------
char readOperand(arm_core p, uint32_t ins, uint32_t *o0, uint32_t *o1, uint8_t *shifter_carry_out, uint8_t *rd, uint8_t *S, uint8_t *op)
{
	uint8_t irn, irm, I, rotate_imm;
	uint32_t immed;

	*rd	= (ins & 0x000F000) >> 12;							// Calcule numero registre dest
	irn	= (ins & 0x00F0000) >> 16;							// Calcule numero registre src 0
	*S	= (ins & 0x0100000) >> 20;							// Calcule le bit S
	*op	= (ins & 0x1E00000) >> 21;							// Calcule le code operation
	*o0	= arm_read_register(p, irn);							// Calcule Operande 0

	I = (ins & 0x2000000) >> 25;								// Calcule Operande 1
	if (I)											//	Cas d'un immediat
	{
		immed		= (ins & 0x0FF);
		rotate_imm	= (ins & 0xF00) >> 8;
		*o1		= ror (immed, rotate_imm);
		if (rotate_imm == 0)	shifter_carry_out = get_bit(p->cpsr, C);
		else			shifter_carry_out = get_bit(ins, 31);
	}
	else											//	Cas d'un registre
	{
		irm	= (ins & 0xF);
		*o1	= arm_read_register(p, irm);
		if (get_bit(ins, 4))	readOperand1_regShift(p, ins, o1, shifter_carry_out);	//		Cas d'un shift lu dans un registre
		else			readOperand1_immShift(p, ins, o1, shifter_carry_out);	//		Cas d'un shift lu dans un immediat
	}
	return 0;
}
// ------------------------------------------
// Lit la valeur de l'opperande 1 et du shifter_carry dans le
// cas ou l'opperande 1 est code par un registre et le decalage est
// code par un immediat
// Parametre d'entree:
//	- p:		etat du processeur
//	- ins:		instruction
// Parametre d'entree sortie:
//	- o1:	Valeur de l'operande 1 (doit avoir ete
//		initialise avec la valeur de Rm (doc p 448))
// Parametre de sortie:
//	- shifter_carry_out: voir doc p 446
// ------------------------------------------
void readOperand1_immShift(arm_core p, uint32_t ins, uint32_t *o1, uint8_t *shifter_carry_out)
{
	uint8_t shift_imm = ((ins>>7) & 0x1F);

	switch((ins>>5) & 0x3)
	{
		case 0: if (shift_imm == 0)	*shifter_carry_out = get_bit(p->cpsr, C);
			else
			{
				*shifter_carry_out	= get_bit(*o1, (32-shift_imm));
				*o1			= lsl(*o1, shift_imm);
			}
			break;
		case 1:	if (shift_imm == 0)
			{
				*shifter_carry_out	= get_bit(*o1, 31);
				*o1			= 0;
			}
			else
			{
				*shifter_carry_out	= get_bit(*o1, (shift_imm-1));
				*o1			= lsr(*o1, shift_imm);
			}
			break;
		case 2:	if (shift_imm == 0)
			{
				*shifter_carry_out	= get_bit(*o1, 31);
				if (get_bit(*o1, 31) == 0)	*o1 = 0;
				else				*o1 = 0xFFFFFFFF;
			}
			else
			{
				*shifter_carry_out	= get_bit(*o1, (shift_imm-1));
				*o1			= asr(*o1, shift_imm);
			}
			break;
		case 3:	if (shift_imm == 0)
			{
				*shifter_carry_out	= get_bit(*o1, 0));
				*o1			= ((lsl(get_bit(p->cpsr, C)), 31) | (lsr(*o1, 1)));
			}
			else
			{
				*shifter_carry_out	= get_bit(*o1, shift_imm));
				*o1			= ror(*o1, shift_imm);
			}
			break;
	}
}
// ------------------------------------------
// Lit la valeur de l'opperande 1 et du shifter_carry dans le
// cas ou l'opperande 1 est code par un registre et le decalage est
// code dans un registre
// Parametre d'entree:
//	- p:		etat du processeur
//	- ins:		instruction
// Parametre d'entree sortie:
//	- o1:	Valeur de l'operande 1
// Parametre de sortie:
//	- shifter_carry_out: voir doc p 446
// ------------------------------------------
void readOperand1_regShift(arm_core p, uint32_t ins, uint32_t *o1, uint8_t *shifter_carry_out)
{
	uint8_t		irs	= ((ins >> 8) & 0xF);
	uint32_t	RS	= arm_read_register(p, irs);
	uint8_t		RS70	= RS & 0xFF;
	uint8_t		RS40	= RS & 0x0F;

	if	(RS70 == 0)	{*shifter_carry_out = get_bit(p->cpsr, C); return;}
	switch((ins>>5) & 0x3)
	{
		case 0: 
			if	(RS70 < 32)
			{
				*shifter_carry_out	= get_bit(*o1, 32-RS70);
				*o1			= lsl(*o1, RS70);
			}
			else if	(RS70 == 32)
			{
				*shifter_carry_out	= get_bit(*o1, 0);
				*o1			= 0;
			}
			else
			{
				*shifter_carry_out	= 0;
				*o1			= 0;
			}
			break;
		case 1:	if	(RS70 < 32)
			{
				*shifter_carry_out	= get_bit(*o1, RS70-1);
				*o1			= lsr(*o1, RS70);
			}
			else if	(RS70 == 32)
			{
				*shifter_carry_out	= get_bit(*o1, 31);
				*o1			= 0;
			}
			else
			{
				*shifter_carry_out	= 0;
				*o1			= 0;
			}
			break;
		case 2:	if	(RS70 < 32)
			{
				*shifter_carry_out	= get_bit(*o1, RS70-1);
				*o1			= asr(*o1, RS70);
			}
			else
			{
				*shifter_carry_out	= get_bit(*o1, 31);
				if (get_bit(*o1, 31) == 0)	*o1 = 0;
				else				*o1 = 0xFFFFFFFF;
			}
			break;
		case 3:	if	(RS40 == 0)*shifter_carry_out = get_bit(*o1, 31);
			else
			{
					*shifter_carry_out	= get_bit(*o1, RS40-1);
					*o1			= ror(*o1, RS40);
			}
			break;
	}
}
// ------------------------------------------
// Ecrit le code condition dans le registre
// CPSR de p en fonction du resultat du calcul res
// parametre:
//	- rd:	numero du registre destination
//	- res:	valeur du resulata
// Return 0 en cas de succes ou un code erreur si non (UNPREDICTABLE)
// ------------------------------------------
void writeCondition(arm_core p, uint8_t rd, uint32_t res)
{
	if (rd == 15)
	{
		if (arm_current_mode_has_spsr(p))	arm_write_cpsr(p, arm_read_cpsr(p));	// CPSR <- SPSR
		else return -1;									// Droit d'acces a CPSR refuse
	}
	else
	{
		uint32_t cpsr = arm_read_cpsr(p);
		if (res == 0)		set_bit(cpsr, Z);
		else			clr_bit(cpsr, Z);

		if (get_bit(res, 31))	set_bit(cpsr, N);
		else			clr_bit(cpsr, N);

		if (shifter_carry_out)	set_bit(cpsr, C);
		else			clr_bit(cpsr, C);
		if (res > 0x0FFFFFFFF)	set_bit(cpsr, V);///%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/// Non specifie par la doc
		else			clr_bit(cpsr, V);///%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/// Non specifie par la doc
	}
	return 1;

}
// ------------------------------------------
// Calcule o1 op o2
// Parametre d'entree:
//	- o1, o2:	Valeur des operandes 1 et 2 sur 64 bit (32 bit + des 0)
// Parametre de sortie:
//	- res:	resultat du calcul sur 64 bits
// ------------------------------------------
uint64_t eor(uint32_t o0, uint32_t o1)
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
