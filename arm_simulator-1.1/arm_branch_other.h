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

#ifndef __ARM_BRANCH_OTHER_H__
#define __ARM_BRANCH_OTHER_H__
#include <stdint.h>
#include "arm_core.h"

int arm_branch(arm_core p, uint32_t ins);
//!!! L'utilisation de BLX avec un registre contenant une adresse impaire fait passer au jeu d'instruction Thumb
// Cette fonctionnalité n'étant (pas encore ;) implémenter,
// BLX NE DOIT ETRE APPELÉ QUE SUR DES REGISTRES CONTENANT DES ADRESSES PAIRES
int arm_branch_X(arm_core p, uint32_t ins);
int arm_miscellaneous(arm_core p, uint32_t ins);

//MSR
int arm_msr_immediate_operand(arm_core p, uint32_t ins);
int arm_msr_register_operand(arm_core p, uint32_t ins);

//MRS
int arm_mrs(arm_core p, uint32_t ins);

int partie_commune_msr(arm_core p, uint32_t ins, uint32_t operande);
int arm_coprocessor_others_swi(arm_core p, uint32_t ins);



//Fonction d'affichage des valeurs
void print_information_instruction_B_BL(arm_core p,	uint32_t instruction);
void print_information_instruction_BLX(arm_core p,	uint32_t instruction);
void print_information_instruction_MRS(arm_core p, uint32_t instruction, uint8_t R);
void print_information_instruction_MSR(arm_core p, uint32_t instruction);

uint32_t lecture_entier_immediat_signe_24bits(uint32_t instruction);


#endif
