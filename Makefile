CC=gcc # the compiler for the current system.
CC_PREPRO=$(CC) -E # the C prerpoccessor used in z80 assembly files
PREPRO= gcc -E # the preproccessor used to preproccess the assembly.
               # (c preproccessor directives are used in the z80 assembly files)
ROM_UPLOAD=minipro -S -p at28c256 -w
DEPS=$(shell find |grep \\\.S|grep -v \\\.pp\\\.S)
Z80_ASM=../z80asm/z80asm
MAKEDEPEND = gcc -M  -o $*.Td $<

all: bootloader.bin 

%.bin:%.pp.S .config
	$(Z80_ASM) $< -o $@

config .config:
	make -s -C scripts/config CC=$(CC)
	./scripts/config/clico 2> .config 

%.pp.S:%.S $(DEPS)
	$(CC_PREPRO) -P -Iinclude $< > $@

upload:bootloader.bin
	$(ROM_UPLOAD) $<

clean:
	rm *.bin *.pp.S -f

