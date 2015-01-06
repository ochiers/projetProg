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
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include "util.h"





// --------------------------------------------------
// Structure de donnees
// ------------------------------------
struct memory_data
{
	int8_t *address;
	size_t size;
};

// --------------------------------------------------
// Cree une memoire virtuelle de taille size
// --------------------------------------------------
memory memory_create(size_t size)
{
	memory mem;

	mem = malloc(sizeof(struct memory_data));
	if (mem)
	{
		mem->address = malloc(size);
		if (!mem->address)
		{
			free(mem);
			mem = NULL;
		}
	}
	if (mem)	mem->size = size;

	return mem;
}
// --------------------------------------------------
// Determine la taille de la memoire virtuelle
// --------------------------------------------------
size_t memory_get_size(memory mem)
{
	return mem->size;
}
// --------------------------------------------------
// Detruit la memoire virtuelle
// --------------------------------------------------
void memory_destroy(memory mem)
{
	free(mem->address);
	free(mem);
}
// --------------------------------------------------
// Lit un octet dans mem a l'adresse donnee dans la memoire en entree
// Place le resultat dans value
// Rend -1 en cas d'erreur et la taille lue en octet si non
// --------------------------------------------------
int memory_read_byte(memory mem, uint32_t address, uint8_t *value)
{
	if ((address < 0) || (address >= mem->size))	return -1;

	*value = (uint8_t)*(mem->address + address);
	return 1;
}
// --------------------------------------------------
// Lit 2 octet dans mem a l'adresse donnee dans la memoire en entree
// Place le resultat dans value
// be indique l'endianness de la memoire et du resultat
// Rend -1 en cas d'erreur et la taille lue en octet si non
// --------------------------------------------------
int memory_read_half(memory mem, int be, uint32_t address, uint16_t *value)
{
	if ((address < 0) || (address >= mem->size-1))	return -1;

	uint8_t a =  (uint8_t)*(mem->address + address);
	uint8_t b =  (uint8_t)*(mem->address + address + 1);

	if (be)	*value = a << 8 | b;
	else	*value = b << 8 | a;
	return 2;
}
// --------------------------------------------------
// Lit 4 octet dans mem a l'adresse donnee dans la memoire en entree
// Place le resultat dans value
// be indique l'endianness de la memoire et du resultat
// Rend -1 en cas d'erreur et la taille lue en octet si non
// --------------------------------------------------
int memory_read_word(memory mem, int be, uint32_t address, uint32_t *value)
{
	if ((address < 0) || (address >= mem->size-3))	return -1;

	uint8_t a =  (uint8_t)*(mem->address + address);
	uint8_t b =  (uint8_t)*(mem->address + address + 1);
	uint8_t c =  (uint8_t)*(mem->address + address + 2);
	uint8_t d =  (uint8_t)*(mem->address + address + 3);

	if (be)	*value = a << 24 | b << 16 | c << 8 | d;
	else	*value = d << 24 | c << 16 | b << 8 | a;
	return 4;
}
// --------------------------------------------------
// Ecrit 1 octet dans mem a l'adresse donnee dans la memoire en entree
// Place le resultat dans value
// Rend -1 en cas d'erreur et le nombre d'octets ecrits si non
// --------------------------------------------------
int memory_write_byte(memory mem, uint32_t address, uint8_t value)
{
	if ((address < 0) || (address > mem->size - 1))	return -1;

	*(mem ->address + address) = value;
	return 1;
}
// --------------------------------------------------
// Ecrit 2 octet dans mem a l'adresse donnee dans la memoire en entree
// Place le resultat dans value
// be indique l'endianness de la memoire et de value
// Rend -1 en cas d'erreur et le nombre d'octets ecrits si non
// --------------------------------------------------
int memory_write_half(memory mem, int be, uint32_t address, uint16_t value)
{
	if ((address < 0) || (address > mem->size - 2))	return -1;

	uint8_t a =  value >> 8;
	uint8_t b =  value ;

	if (be)
	{
		*(mem->address + address) = a;
		*(mem->address + address +1) = b;
	}
	else {
		*(mem->address + address) = b;
		*(mem->address + address +1) = a;
	}
	
	return 2;
}
// --------------------------------------------------
// Ecrit 4 octet dans mem a l'adresse donnee dans la memoire en entree
// Place le resultat dans value
// be indique l'endianness de la memoire et de value
// Rend -1 en cas d'erreur et le nombre d'octets ecrits si non
// --------------------------------------------------
int memory_write_word(memory mem, int be, uint32_t address, uint32_t value)
{
	if ((address < 0) || (address > mem->size - 4))	return -1;

	uint8_t a =  value >> 24;
	uint8_t b =  value >> 16;
	uint8_t c =  value >> 8;
	uint8_t d =  value;

	if (be) {
		*(mem->address + address) = a;
		*(mem->address + address +1) = b;
		*(mem->address + address +2) = c;
		*(mem->address + address +3) = d;
	}
	else {
		*(mem->address + address) = d;
		*(mem->address + address +1) = c;
		*(mem->address + address +2) = b;
		*(mem->address + address +3) = a;
	}
	
	return 4;
}
