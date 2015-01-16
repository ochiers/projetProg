#include "arm_exception.h"






#define PC_USER 15

#ifdef BIG_ENDIAN_SIMULATOR
#define CP15_reg1_EEbit 1
#else
#define CP15_reg1_EEbit 0
#endif

#define Exception_bit_9 (CP15_reg1_EEbit << 9)






// ------------------------------------------
// Partie commune du traitement de toutes les exceptions du processeur.
//	- offset_pc	: decalage par rapport a la caleur de pc a enregistrer dans lr
//	- disableFIQ	: indique si le bit FIQ de cpsr doit etre mis a 1 ou noon
//	- disableIRQ	: indique si le bit IRQ de cpsr doit etre mis a 1 ou noon
//	- interruptVectAdr	: adresse du vecteur d'interruption de l'exception courente
// Ne prend pas en compte le bite VE du coprocesseur
// Ne prend pas en compte les valeurs haute des vecteurs d'interruptions
// ------------------------------------------
void arm_exception_traitement_commun(arm_core p, uint8_t offset_pc, char positifOff, uint8_t mode, char disableFIQ, char disableIRQ, char updateSPSR, char updateLR, uint32_t interruptVectAdr)
{
	uint32_t addr, cpsr, spsr;

	addr	= arm_read_register(p, PC_USER);
	if (positifOff)	addr += offset_pc;
	else		addr -= offset_pc;
	cpsr	= arm_read_cpsr(p);
	spsr	= cpsr;
	cpsr	&= ~(0x1F);
	cpsr	|= mode;						// Changer de mode
	cpsr	&= ~(1 << 5);						// Interpreter le code en ARM
	if (disableFIQ)		cpsr	|= 1 << 6;			// Interdire les interruptions normales (FIQ)
	else			cpsr	&= ~(1 << 6);
	if (disableIRQ	)	cpsr	|= 1 << 7;			// Interdire les interruptions rapides (IRQ)
	else			cpsr	&= ~(1 << 7);
	#ifdef BIG_ENDIAN_SIMULATOR
	cpsr	|= 1 << 9;						// Bit endiane = BIG_ENDIAN
	#else
	cpsr	&= ~(1 << 9);						// Bit endiane = LITLE_ENDIAN
	#endif
	arm_write_cpsr(p, cpsr);
	if (updateLR)	arm_write_register(p, 14, addr);
	if (updateSPSR)	arm_write_spsr(p, spsr);
	arm_write_register(p, 15, interruptVectAdr);			// Branchement sur le vecteur d'interruption si VE == 0 (voir coprocesseur)
									// ~(high_vectors_configured)
}
// ------------------------------------------
// Traite les 7 types d'exception gerees par le processeur
// Ne prend pas en compte le bite VE du coprocesseur
// Ne prend pas en compte les valeurs haute des vecteurs d'interruptions
// ------------------------------------------
void arm_exception(arm_core p, unsigned char exception)
{
	uint8_t fiq = (arm_read_cpsr(p) & (0x1<<6)) >> 6;
	uint8_t irq = (arm_read_cpsr(p) & (0x1<<7)) >> 7;

	switch(exception)
	{
		case RESET:								// Semantics of reset interrupt (ARM manual A2-18)
			arm_write_cpsr(p, 0x1d3 | Exception_bit_9);
			arm_exception_traitement_commun(p, 0, 0, SVC, 0, 0, 0, 0, 0x0+FAKE_INTERUPT_VECT);
			arm_write_usr_register(p, PC_USER, 0x1000);
			break;
		case UNDEFINED_INSTRUCTION:	arm_exception_traitement_commun(p, 0, 0, UND, 0, 0, 1, 1, 0x00000004+FAKE_INTERUPT_VECT);	break;
		case SOFTWARE_INTERRUPT:	arm_exception_traitement_commun(p, 0, 1, SVC, 0, 0, 1, 1, 0x00000008+FAKE_INTERUPT_VECT);	break;
		case PREFETCH_ABORT:		arm_exception_traitement_commun(p, 4, 1, ABT, 0, 0, 1, 1, 0x0000000C+FAKE_INTERUPT_VECT);	break;
		case DATA_ABORT:		arm_exception_traitement_commun(p, 8, 1, ABT, 0, 0, 1, 1, 0x00000010+FAKE_INTERUPT_VECT);	break;
		case INTERRUPT:
			if (irq)		printf("\n***** Traitement de IRQ en cours: nouveau signal IRQ ignore ****** \n");
			else			arm_exception_traitement_commun(p, 0, 1, IRQ, 0, 1, 1, 1, 0x00000018+FAKE_INTERUPT_VECT);
			break;
		case FAST_INTERRUPT:
			if (fiq)		printf("\n***** Traitement de FIQ en cours: nouveau signal FIQ ignore ****** \n");
			else			arm_exception_traitement_commun(p, 0, 1, FIQ, 1, 1, 1, 1, 0x0000001C+FAKE_INTERUPT_VECT);
			break;
		default:
			printf("\n\n**** Traitement d'exception ****\n");
			printf("**** Exception non geree: %d ****\n", exception);
			exit(0);
	}
}
