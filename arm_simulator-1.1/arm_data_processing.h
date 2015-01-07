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
#ifndef __ARM_DATA_PROCESSING_H__
#define __ARM_DATA_PROCESSING_H__
#include <stdint.h>
#include "arm_core.h"





// ------------------------------------------
// Decoding functions for different classes of instructions
// ------------------------------------------
	int arm_data_processing_shift(arm_core p, uint32_t ins);
	int arm_data_processing_immediate_msr(arm_core p, uint32_t ins);
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
	char readOperand(arm_core p, uint32_t ins, uint64_t *o0, uint64_t *o1, uint8_t *shifter_carry_out, uint8_t *rd, uint8_t *S, uint8_t *op);
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
	void readOperand1_immShift(arm_core p, uint32_t ins, uint64_t *o1, uint8_t *shifter_carry_out);
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
	void readOperand1_regShift(arm_core p, uint32_t ins, uint64_t *o1, uint8_t *shifter_carry_out);
// ------------------------------------------
// Ecrit le code condition dans le registre
// CPSR de p en fonction du resultat du calcul res
// ------------------------------------------
	void writeCondition(arm_core p, uint64_t res);
// ------------------------------------------
// Calcule o1 op o2
// Parametre d'entree:
//	- o1, o2:	Valeur des operandes 1 et 2 sur 64 bit (32 bit + des 0)
// Parametre de sortie:
//	- res:	resultat du calcul sur 64 bits
// ------------------------------------------
	uint64_t eor(uint64_t o0, uint64_t o1);

#endif
