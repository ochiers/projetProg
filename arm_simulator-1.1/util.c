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
#include "util.h"





/* We implement asr because shifting a signed is non portable in ANSI C */
uint32_t asr(uint32_t value, uint8_t shift)
{
	return (value >> shift) | (get_bit(value, 31) ? ~0<<(32-shift) : 0);
}

uint32_t ror(uint32_t value, uint8_t rotation)
{
	return (value >> rotation) | (value << (32-rotation));
}
int is_big_endian()
{
	static uint32_t one = 1;
	return ((* (uint8_t *) &one) == 0);
}
// ------------------------------------------
// Calcule o1 OuExclusif o2
// Parametre d'entree:
//	- o1, o2:	Valeur des operandes 1 et 2
// Parametre de sortie:
//	- res:	resultat du calcul sur 32 bits
// ------------------------------------------
uint32_t eor(uint32_t o0, uint32_t o1)
{
	return (o0 ^ o1);
}
// ------------------------------------------
// Calcule o1 Logical Shift Left ..
// Parametre d'entree:
//	- o1, o2:	Valeur des operandes 1 et 2 sur 64 bit (32 bit + des 0)
// Parametre de sortie:
//	- res:	resultat du calcul sur 32 bits
// ------------------------------------------
uint32_t lsl(uint32_t a, uint8_t shift)
{
	return (a << shift);
}
// ------------------------------------------
// Calcule a Logical Shift Right ..
// Parametre d'entree:
//	- o1, o2:	Valeur des operandes 1 et 2 sur 64 bit (32 bit + des 0)
// Parametre de sortie:
//	- res:	resultat du calcul sur 32 bits
// ------------------------------------------
uint32_t lsr(uint32_t a, uint8_t shift)
{
	return (a >> shift);
}
// ------------------------------------------
// Rend 1 si la soustraction a - b - c cree un emprunt
// ------------------------------------------
uint8_t borrowFrom(uint32_t a, uint32_t b, uint32_t c)
{
	if (c > b)	return 0;
	uint32_t x = b - c;
	if (x > a)	return 1;
	else		return 0;
}
// ------------------------------------------
// Rend 1 si l'addition a + b + c cree un debordement
// ------------------------------------------
uint8_t overflowFromAdd(uint32_t a, uint32_t b, uint32_t c)
{
	uint64_t res = (uint64_t)a + (uint64_t)b + (uint64_t)c;

	return (get_bit(res, 32) == 1);
}
// ------------------------------------------
// Rend 1 si la soustraction a + b + c cree un debordement
// ------------------------------------------
uint8_t overflowFromSub(uint32_t a, uint32_t b, uint32_t c)
{
	uint64_t res = (uint64_t)a - (uint64_t)b - (uint64_t)c;

	return (get_bit(res, 32) == 1);
}
// ------------------------------------------
// Rend 1 si la soustraction a + b + c cree un debordement
// ------------------------------------------
uint8_t carryFrom(uint32_t a, uint32_t b, uint32_t c)
{
	uint64_t res = (uint64_t)a + (uint64_t)b + (uint64_t)c;

	return (res > 0xFFFFFFFF);
}
// ------------------------------------------
// Affiche la valeur de a en binaire sur nbrBit
// ------------------------------------------
void printBin(uint32_t a, uint8_t nbrBit)
{
	uint8_t i;

	for (i=0; i<nbrBit; i++)	printf("%d", get_bit(a, i));
	printf("\n");
}
