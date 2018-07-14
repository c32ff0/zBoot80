#define label(a) AY3891X##a

//a: USEED
//bc: Used
label(write_reg): macro LT VALUE //have to use the assembler's macros because i have a c macro as the name of the macro
inc bc
ld a,LT
out (c),a 
dec bc
ld a,VALUE
out (c),a 
endm

#if FUNCTION_BEEP
//BC=base address of ay3891x
label(beep_start):
	push af
	inc bc
	inc bc
	label(write_reg)  0 0x63
	label(write_reg)  1 0x01
	label(write_reg)  2 0xee
	label(write_reg)  3 0x0e
	label(write_reg)  4 0xdb
	label(write_reg)  5 0x01
	label(write_reg)  6 0x1f
	label(write_reg)  7 0x2a
	label(write_reg)  8 0x0e
	label(write_reg)  9 0x00
	label(write_reg) 10 0x00
	label(write_reg) 11 0x00
	label(write_reg) 12 0x00
	label(write_reg) 13 0x00
	pop af
	ret

//BC=base address of ay3891x
label(mute):
	inc bc
	inc bc
	push af
	label(write_reg)  7 0xFF
	pop af
	ret
#endif
