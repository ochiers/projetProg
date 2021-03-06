#ifndef __ARM_DATA_PROCESSING_H__
#define __ARM_DATA_PROCESSING_H__
#include <stdint.h>
#include "arm_core.h"




char *description;
char *oper;






// ------------------------------------------
// Decoding functions for different classes of instructions
// Execute la fonction
// Met a jours le registre destination
// Met a jour le registre d'etat cpsr en fonction du bit S et
// de l'algo de la commande
// ------------------------------------------
	int arm_data_processing_shift(arm_core p, uint32_t ins);
// ------------------------------------------
// Decoding functions for the specific mul instructions
// Execute la fonction
// Met a jours le registre destination
// Met a jour le registre d'etat cpsr en fonction du bit S et
// de l'algo de la commande
// ------------------------------------------
	int arm_data_processing_shift_mul(arm_core p, uint32_t ins);
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
	void readOperand(arm_core p, uint32_t ins, uint32_t *o0, uint32_t *o1, uint8_t *shifter_carry_out, uint8_t *irn, uint8_t *rd, uint8_t *S, uint8_t *I, uint8_t *op);
// ------------------------------------------
// Affiche les differantes valeurs de l'instruction
// ------------------------------------------
	void printInstrdataProcessingShiftOp(arm_core p, uint8_t op, uint8_t S, uint8_t I, uint8_t rn, uint8_t rd, uint32_t o0, uint32_t o1, uint32_t res);
// ------------------------------------------
// Affiche les differantes valeurs de l'instruction
// ------------------------------------------
	void printInstrdataProcessingShiftTest(uint8_t op, uint8_t S, uint8_t I, uint8_t rn, uint32_t o0, uint32_t o1, uint8_t err);
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
	void readOperand1_immShift(arm_core p, uint32_t ins, uint32_t *o1, uint8_t *shifter_carry_out);
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
	void readOperand1_regShift(arm_core p, uint32_t ins, uint32_t *o1, uint8_t *shifter_carry_out);

#endif
