AM_CFLAGS=-g -mbig-endian
AM_CCASFLAGS=-mbig-endian
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
noinst_PROGRAMS=example1 example2 example3 example4 exampleLoadStore exampleDataProcessing exampleDataProcessingSave exampleMrsMsr exampleException exampleCondition test_ldr test_ldrb test_ldrh exampleGeneral insertion_sort

all-am: $(PROGS)
endif



example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
exampleLoadStore_SOURCES=exampleLoadStore.s
exampleDataProcessing_SOURCES=exampleDataProcessing.s
exampleDataProcessingSave_SOURCES=exampleDataProcessingSave.s
exampleMrsMsr_SOURCES=exampleMrsMsr.s
exampleException_SOURCES=exampleException.s
exampleCondition_SOURCES=exampleCondition.s
test_ldr_SOURCES=test_ldr.s
test_ldrb_SOURCES=test_ldrb.s
test_ldrh_SOURCES=test_ldrh.s
exampleGeneral_SOURCES=exampleGeneral.s
insertion_sort_SOURCES=insertion_sort.c

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
