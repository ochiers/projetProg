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
#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

#define get_bit(x, i) (((x)>>(i))&1)
#define set_bit(x, i) ((x)|(1<<(i)))
#define clr_bit(x, i) ((x)&~(1<<(i)))

#define get_bits(x, h, l) (((x)>>(l))&~((~0>>((h)-(l)))<<((h)-(l))))
#define set_bits(x, h, l, bits) \
                 (((x)&~((~0>>(l))<<(l)))|((x)&((~0>>(h))<<(h)))|((bits)<<(l)))

#define reverse_2(x) ((((x)&0xFF)<<8)|(((x)>>8)&0xFF))
#define reverse_4(x) ((((x)&0xFF)<<24)|((((x)>>8)&0xFF)<<16)|\
                      ((((x)>>16)&0xFF)<<8)|(((x)>>24)&0xFF))

uint32_t asr(uint32_t value, uint8_t shift);
uint32_t ror(uint32_t value, uint8_t rotation);
// ------------------------------------------
// Calcule o1 OuExclusif o2
// Parametre d'entree:
//	- o1, o2:	Valeur des operandes 1 et 2 sur 64 bit (32 bit + des 0)
// Parametre de sortie:
//	- res:	resultat du calcul sur 32 bits
// ------------------------------------------
	uint32_t eor(uint32_t o0, uint32_t o1);
// ------------------------------------------
// Calcule o1 Logical Shift Left ..
// Parametre d'entree:
//	- o1, o2:	Valeur des operandes 1 et 2 sur 64 bit (32 bit + des 0)
// Parametre de sortie:
//	- res:	resultat du calcul sur 32 bits
// ------------------------------------------
	uint32_t lsl(uint32_t a, uint8_t shift);
// ------------------------------------------
// Calcule a Logical Shift Right ..
// Parametre d'entree:
//	- o1, o2:	Valeur des operandes 1 et 2 sur 64 bit (32 bit + des 0)
// Parametre de sortie:
//	- res:	resultat du calcul sur 32 bits
// ------------------------------------------
	uint32_t lsr(uint32_t a, uint8_t shift);
// ------------------------------------------
// Rend 1 si la soustraction a - b - c cree un emprunt
// ------------------------------------------
	uint8_t borrowFrom(uint32_t a, uint32_t b, uint32_t c);
// ------------------------------------------
// Rend 1 si l'addition a + b + c cree un debordement
// ------------------------------------------
	uint8_t overflowFromAdd(uint32_t a, uint32_t b, uint32_t c);
// ------------------------------------------
// Rend 1 si la soustraction a + b + c cree un debordement
// ------------------------------------------
	uint8_t overflowFromSub(uint32_t a, uint32_t b, uint32_t c);
// ------------------------------------------
// Rend 1 si la soustraction a + b + c cree un debordement
// ------------------------------------------
	uint8_t carryFrom(uint32_t a, uint32_t b, uint32_t c);

int is_big_endian();
#endif
