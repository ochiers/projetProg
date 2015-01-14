#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"








// ------------------------------------------
// Decoding functions for different classes of instructions
// Execute la fonction
// Met a jours le registre destination
// Met a jour le registre d'etat cpsr en fonction du bit S et
// de l'algo de la commande
// ------------------------------------------
int arm_data_processing_shift(arm_core p, uint32_t ins)
{
	uint32_t res=0, o0, o1;
	uint32_t cpsr = arm_read_cpsr(p);
	uint8_t rd, rn, S, op, shifter_carry_out, I;
	uint8_t cflag	= (get_bit(cpsr, CPSR_C) >> CPSR_C);
	uint8_t ncflag	= ((~cflag) & 0x1);
	int ret = SUCCESS;
	description	= malloc(sizeof(char) * 1024);
	oper		= malloc(sizeof(char) * 1024);

	readOperand(p, ins, &o0, &o1, &shifter_carry_out, &rn, &rd, &S, &I, &op);		// Parser l'instruction
	switch(op)										// Executer l'instruction
	{
		case INSTR_AND:	res = o0 & o1;
				sprintf(oper, "AND");
				if (shifter_carry_out)		cpsr = set_bit(cpsr, CPSR_C);
				else				cpsr = clr_bit(cpsr, CPSR_C);
				break;
		case INSTR_EOR:	res = eor(o0, o1);
				sprintf(oper, "EOR");
				if (shifter_carry_out)		cpsr = set_bit(cpsr, CPSR_C);
				else				cpsr = clr_bit(cpsr, CPSR_C);
				break;
		case INSTR_SUB:	res = o0 - o1;
				sprintf(oper, "SUB");
				if (borrowFrom(o0, o1, 0))	cpsr = clr_bit(cpsr, CPSR_C);
				else				cpsr = set_bit(cpsr, CPSR_C);
				if (overflowFromSub(o0, o1, 0))	cpsr = set_bit(cpsr, CPSR_V);
				else				cpsr = set_bit(cpsr, CPSR_V);
				break;
		case INSTR_RSB:	res = o1 - o0;
				sprintf(oper, "RSB");
				if (borrowFrom(o1, o0, 0))	cpsr = clr_bit(cpsr, CPSR_C);
				else				cpsr = set_bit(cpsr, CPSR_C);
				if (overflowFromSub(o1, o0, 0))	cpsr = set_bit(cpsr, CPSR_V);
				else				cpsr = set_bit(cpsr, CPSR_V);
				break;
		case INSTR_ADD:	res = o0 + o1;
				sprintf(oper, "ADD");
				if (carryFrom(o0, o1, 0))	cpsr = clr_bit(cpsr, CPSR_C);
				else				cpsr = set_bit(cpsr, CPSR_C);
				if (overflowFromAdd(o0, o1, 0))	cpsr = set_bit(cpsr, CPSR_V);
				else				cpsr = set_bit(cpsr, CPSR_V);
				break;
		case INSTR_ADC:	res = o0 + o1 + cflag;
				sprintf(oper, "ADC");
				if (carryFrom(o0, o1, cflag))	cpsr = clr_bit(cpsr, CPSR_C);
				else				cpsr = set_bit(cpsr, CPSR_C);
				if (overflowFromAdd(o0, o1, cflag))cpsr = set_bit(cpsr, CPSR_V);
				else				cpsr = set_bit(cpsr, CPSR_V);
				break;
		case INSTR_SBC:	res = o0 - o1 - ncflag;
				sprintf(oper, "SBC");
				if (borrowFrom(o0, o1, ncflag))	cpsr = clr_bit(cpsr, CPSR_C);
				else				cpsr = set_bit(cpsr, CPSR_C);
				if (overflowFromSub(o0, o1, ncflag))cpsr = set_bit(cpsr, CPSR_V);
				else				cpsr = set_bit(cpsr, CPSR_V);
				break;
		case INSTR_RSC:	res = o1 - o0 - ncflag;
				sprintf(oper, "RSC");
				if (borrowFrom(o1, o0, ncflag))	cpsr = clr_bit(cpsr, CPSR_C);
				else				cpsr = set_bit(cpsr, CPSR_C);
				if (overflowFromSub(o1, o0, ncflag))cpsr = set_bit(cpsr, CPSR_V);
				else				cpsr = set_bit(cpsr, CPSR_V);
				break;
		case INSTR_ORR:	res = o0 | o1;
				sprintf(oper, "ORR");
				if (shifter_carry_out)		cpsr = set_bit(cpsr, CPSR_C);
				else				cpsr = clr_bit(cpsr, CPSR_C);
				break;
		case INSTR_MOV:	res = o1;
				sprintf(oper, "MOV");
				if (shifter_carry_out)		cpsr = set_bit(cpsr, CPSR_C);
				else				cpsr = clr_bit(cpsr, CPSR_C);
				break;
		case INSTR_BIC:	res = o0 & (~o1);
				sprintf(oper, "BIC");
				if (shifter_carry_out)		cpsr = set_bit(cpsr, CPSR_C);
				else				cpsr = clr_bit(cpsr, CPSR_C);
				break;
		case INSTR_MVN:	res = ~o1;
				sprintf(oper, "MVN");
				if (shifter_carry_out)		cpsr = set_bit(cpsr, CPSR_C);
				else				cpsr = clr_bit(cpsr, CPSR_C);
				break;
		case INSTR_TST:	if (S == 0)
				{
					printInstrdataProcessingShiftTest(op, S, I, rn, o0, o1, 1);
					return UNDEFINED_INSTRUCTION;
				}
				sprintf(oper, "TST");
				res = o0 & o1;
				if (get_bit(res, 31))		cpsr = set_bit(cpsr, CPSR_N);
				else				cpsr = clr_bit(cpsr, CPSR_N);
				if (res == 0)			cpsr = set_bit(cpsr, CPSR_Z);
				else				cpsr = clr_bit(cpsr, CPSR_Z);
				if (shifter_carry_out)		cpsr = set_bit(cpsr, CPSR_C);
				else				cpsr = clr_bit(cpsr, CPSR_C);
				arm_write_cpsr(p, cpsr);
				printInstrdataProcessingShiftTest(op, S, I, rn, o0, o1, 0);
				return SUCCESS;
		case INSTR_TEQ:	if (S == 0)
				{
					printInstrdataProcessingShiftTest(op, S, I, rn, o0, o1, 1);
					return UNDEFINED_INSTRUCTION;
				}
				sprintf(oper, "TEQ");
				res = eor(o0, o1);
				if (get_bit(res, 31))		cpsr = set_bit(cpsr, CPSR_N);
				else				cpsr = clr_bit(cpsr, CPSR_N);
				if (res == 0)			cpsr = set_bit(cpsr, CPSR_Z);
				else				cpsr = clr_bit(cpsr, CPSR_Z);
				if (shifter_carry_out)		cpsr = set_bit(cpsr, CPSR_C);
				else				cpsr = clr_bit(cpsr, CPSR_C);
				arm_write_cpsr(p, cpsr);
				printInstrdataProcessingShiftTest(op, S, I, rn, o0, o1, 0);
				return SUCCESS;
		case INSTR_CMP:	if (S == 0)
				{
					printInstrdataProcessingShiftTest(op, S, I, rn, o0, o1, 1);
					return UNDEFINED_INSTRUCTION;
				}
				sprintf(oper, "CMP");
				res = o0 - o1;
				if (get_bit(res, 31))		cpsr = set_bit(cpsr, CPSR_N);
				else				cpsr = clr_bit(cpsr, CPSR_N);
				if (res == 0)			cpsr = set_bit(cpsr, CPSR_Z);
				else				cpsr = clr_bit(cpsr, CPSR_Z);
				if (borrowFrom(o0, o1, 0))	cpsr = clr_bit(cpsr, CPSR_C);
				else				cpsr = set_bit(cpsr, CPSR_C);
				if (overflowFromSub(o0, o1, 0))	cpsr = set_bit(cpsr, CPSR_V);
				else				cpsr = clr_bit(cpsr, CPSR_V);
				arm_write_cpsr(p, cpsr);
				printInstrdataProcessingShiftTest(op, S, I, rn, o0, o1, 0);
				return SUCCESS;
		case INSTR_CMN:	if (S == 0)
				{
					printInstrdataProcessingShiftTest(op, S, I, rn, o0, o1, 1);
					return UNDEFINED_INSTRUCTION;
				}
				sprintf(oper, "CMN");
				res = o0 + o1;
				if (get_bit(res, 31))		cpsr = set_bit(cpsr, CPSR_N);
				else				cpsr = clr_bit(cpsr, CPSR_N);
				if (res == 0)			cpsr = set_bit(cpsr, CPSR_Z);
				else				cpsr = clr_bit(cpsr, CPSR_Z);
				if (carryFrom(o0, o1, 0))	cpsr = set_bit(cpsr, CPSR_C);
				else				cpsr = clr_bit(cpsr, CPSR_C);
				if (overflowFromSub(o0, o1, 0))	cpsr = set_bit(cpsr, CPSR_V);
				else				cpsr = clr_bit(cpsr, CPSR_V);
				arm_write_cpsr(p, cpsr);
				printInstrdataProcessingShiftTest(op, S, I, rn, o0, o1, 0);
				return SUCCESS;
		default:	printInstrdataProcessingShiftTest(op, S, I, rn, o0, o1, 1);
				return UNDEFINED_INSTRUCTION;
	}
	arm_write_register(p, rd, res);								// Ecriture du resultat
	printInstrdataProcessingShiftOp(p, op, S, I, rn, rd, o0, o1, res);
	if (S)											// Mise a jour du cpsr
	{
		if (rd == 15)									//	Cas d'un changement de pc
		{
printf("\n\n\n****************%s*************\n\n", arm_get_mode_name(cpsr & 0x1F));
			if (arm_current_mode_has_spsr(p)) arm_write_cpsr(p, arm_read_spsr(p));	//		CPSR <- SPSR
			else ret = UNPREDICTABLE;
		}
		else										//	Cas general (Les flags C, v sont mis a
		{										//	jours par l'execution des instructions)
			if (res == 0)		cpsr = set_bit(cpsr, CPSR_Z);			//		Flag Z
			else			cpsr = clr_bit(cpsr, CPSR_Z);
			if (get_bit(res, 31))	cpsr = set_bit(cpsr, CPSR_N);			//		Flag N
			else			cpsr = clr_bit(cpsr, CPSR_N);
			arm_write_cpsr(p, cpsr);
		}
	}
	free(description);
	free(oper);
	return ret;
}
// ------------------------------------------
// Lit les valeur des registres dans l'instruction
// Parametre d'entree:
//	- p:			etat du processeur
//	- ins:			instruction
// Parametre de sortie:
//	- o0, o1:		Valeur des operandes 0 et 1
//	- shifter_carry_out:	retenue sortante de l'opperande o1
//	- irn:			numero du registre operande 0
//	- rd:			numero du registre destination
//	- S: 			indique si les codes conditions doivent etre mis a jours
//	- I: 			indique si le secon operande est un immediat
//	- op:			code de l'operation a realiser
// ------------------------------------------
void readOperand(arm_core p, uint32_t ins, uint32_t *o0, uint32_t *o1, uint8_t *shifter_carry_out, uint8_t *irn, uint8_t *rd, uint8_t *S, uint8_t *I, uint8_t *op)
{
	uint8_t irm, rotate_imm;
	uint32_t immed;

	*rd	= (ins & 0x000F000) >> 12;							// Calcule numero registre dest
	*irn	= (ins & 0x00F0000) >> 16;							// Calcule numero registre src 0
	*S	= (ins & 0x0100000) >> 20;							// Calcule le bit S
	*op	= (ins & 0x1E00000) >> 21;							// Calcule le code operation
	*o0	= arm_read_register(p, *irn);							// Calcule Operande 0

	*I = (ins & 0x2000000) >> 25;								// Calcule Operande 1
	if (*I)											//	Cas d'un immediat
	{



		immed		= (ins & 0x0FF);
		rotate_imm	= (ins & 0xF00) >> 8;
		*o1		= ror (immed, rotate_imm*2);
		if (rotate_imm == 0)	*shifter_carry_out = get_bit(arm_read_cpsr(p), CPSR_C);
		else			*shifter_carry_out = get_bit(ins, 31);
		sprintf(description, " Operande Immediat %u + rotation droite de %u", immed, rotate_imm*2);
	}
	else											//	Cas d'un registre
	{
		irm	= (ins & 0xF);
		*o1	= arm_read_register(p, irm);
		sprintf(description, " Operande Register %u", *o1);
		if (get_bit(ins, 4))	readOperand1_regShift(p, ins, o1, shifter_carry_out);	//		Cas d'un shift lu dans un registre
		else			readOperand1_immShift(p, ins, o1, shifter_carry_out);	//		Cas d'un shift lu dans un immediat
	}
}
// ------------------------------------------
// Affiche les differantes valeurs de l'instruction
// ------------------------------------------
void printInstrdataProcessingShiftOp(arm_core p, uint8_t op, uint8_t S, uint8_t I, uint8_t rn, uint8_t rd, uint32_t o0, uint32_t o1, uint32_t res)
{
	printf("\t----------------------------------------\n");
	printf("\t\t* opcode\t: ");			printBin(op, 4, 0); printf(" %s\n", oper);
	printf("\t\t* S\t\t: ");			printBin(S,  1, 1);
	printf("\t\t* I\t\t: ");			printBin(I,  1, 1);
	printf("\t\t* Rd\t\t: %u\n",			rd);
	printf("\t\t* Rn\t\t: %u\n",			rn);
	printf("\t\t* Valeur o0:\t: %u\n",		o0);
	printf("\t\t* Valeur o1:\t: %u (%s)\n",		o1, description);
	printf("\t\t* Valeur res\t: %u\n",		res);
	printf("\t\t* Valeur Rd\t: ");			printBin(arm_read_register(p, rd), 32, 1);
	printf("\t----------------------------------------\n");
}
// ------------------------------------------
// Affiche les differantes valeurs de l'instruction
// ------------------------------------------
void printInstrdataProcessingShiftTest(uint8_t op, uint8_t S, uint8_t I, uint8_t rn, uint32_t o0, uint32_t o1, uint8_t err)
{
	printf("\t----------------------------------------\n");
	if (err)
	{
		printf("\t\t* Instruction inconnue\n");
	}
	else
	{
		printf("\t\t* opcode\t: ");		printBin(op, 4, 0); printf(" %s\n", oper);
		printf("\t\t* S\t\t: ");		printBin(S,  1, 1);
		printf("\t\t* I\t\t: ");		printBin(I,  1, 1);
		printf("\t\t* Rn\t\t: %u\n",		rn);
		printf("\t\t* Valeur o0:\t: %u\n",	o0);
		printf("\t\t* Valeur o1:\t: %u (%s)\n",	o1, description);
	}
	printf("\t----------------------------------------\n");
}
// ------------------------------------------
// Lit la valeur de l'opperande 1 et du shifter_carry dans le
// cas ou l'opperande 1 est code par un registre et le decalage est
// code par un immediat
// Parametre d'entree:
//	- p:	etat du processeur
//	- ins:	instruction
// Parametre d'entree sortie:
//	- o1:	Valeur de l'operande 1 (doit avoir ete
//		initialise avec la valeur de Rm (doc p 448))
// Parametre de sortie:
//	- shifter_carry_out: voir doc p 446
// ------------------------------------------
void readOperand1_immShift(arm_core p, uint32_t ins, uint32_t *o1, uint8_t *shifter_carry_out)
{
	uint8_t shift_imm	= ((ins>>7) & 0x1F);
	uint8_t cflag		= get_bit(arm_read_cpsr(p), CPSR_C);

	switch((ins>>5) & 0x3)
	{
		case 0: if (shift_imm == 0)	*shifter_carry_out = cflag;
			else
			{
				*shifter_carry_out	= get_bit(*o1, (32-shift_imm));
				*o1			= lsl(*o1, shift_imm);
			}
			sprintf(description, "%s + lsl immediat de %u", description, shift_imm);
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
			sprintf(description, "%s + lsr immediat de %u", description, shift_imm);
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
			sprintf(description, "%s + asr immediat de %u", description, shift_imm);
			break;
		case 3:	if (shift_imm == 0)
			{
				*shifter_carry_out	= get_bit(*o1, 0);
				*o1			= (lsl(cflag, 31)) | (lsr(*o1, 1));
				sprintf(description, "%s + ror avec extention (rrx)", description);
			}
			else
			{
				*shifter_carry_out	= get_bit(*o1, shift_imm-1);
				*o1			= ror(*o1, shift_imm);
				sprintf(description, "%s + ror immediat de %u", description, shift_imm);
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
//	- o1:		Valeur de l'operande 1
// Parametre de sortie:
//	- shifter_carry_out: voir doc p 446
// ------------------------------------------
void readOperand1_regShift(arm_core p, uint32_t ins, uint32_t *o1, uint8_t *shifter_carry_out)
{
	uint8_t		irs	= ((ins >> 8) & 0xF);
	uint32_t	RS	= arm_read_register(p, irs);
	uint8_t		RS70	= RS & 0xFF;
	uint8_t		RS40	= RS & 0x0F;
	uint8_t		cflag	= get_bit(arm_read_cpsr(p), CPSR_C);

	if	(RS70 == 0)	{*shifter_carry_out = cflag; return;}
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
			sprintf(description, "%s + lsl register %u", description, RS);
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
			sprintf(description, "%s + lsr register %u", description, RS);
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
			sprintf(description, "%s + asr register %u", description, RS);
			break;
		case 3:	if	(RS40 == 0)*shifter_carry_out = get_bit(*o1, 31);
			else
			{
					*shifter_carry_out	= get_bit(*o1, RS40-1);
					*o1			= ror(*o1, RS40);
			}
			sprintf(description, "%s + ror register %u", description, RS);
			break;
	}
}
