#define label(a) Z84C40XX##a //add "Z84c40XX to a . this is used to not cause conflict in the namespace

#if Z84C40XX_CD_ON_A1 == 1
	#define Z80SIOAREG(loc)  Z84C40XXaddr##loc+0b10 
	#define Z80SIOBREG(loc)  Z84C40XX_LOCAaddr##loc+0b11 
	#define Z80SIOADATA(loc) Z84C40XX_LOCAaddr##loc+0b00 
	#define Z80SIOBDATA(loc) Z84C40XX_LOCAaddr##loc+0b10 
#else
	#define Z80SIOAREG(loc)  Z84C40XX_LOCAaddr##loc+0b01
	#define Z80SIOBREG(loc)  Z84C40XX_LOCAaddr##loc+0b11
	#define Z80SIOADATA(loc) Z84C40XX_LOCAaddr##loc+0b00
	#define Z80SIOBDATA(loc) Z84C40XX_LOCAaddr##loc+0b01
#endif

//BC= A REGISTER
//DE= B REGISTER
label(SIO_A_RESET):
	push af
	;set up TX and RX:
	ld a,00110000b		;write into WR0: error reset, select WR0
	out (c),A
	
	ld a,018h			;write into WR0: channel reset
	out (C),A
	
	ld a,004h			;write into WR0: select WR4
	out (c),A
	ld a,44h			;44h write into WR4: clkx16,1 stop bit, no parity
	out (c),A

	ld a,005h			;write into WR0: select WR5
	out (c),A
	ld a,0E8h			;DTR active, TX 8bit, BREAK off, TX on, RTS inactive
	out (c),A

	ld a,01h			;write into WR0: select WR1

	ld a,003h			;write into WR0: select WR3
	out (c),A
	ld a,0C1h			;RX 8bit, auto enable off, RX on
	out (c),A
	;Channel A RX active

	ld b,d
	ld c,e
	out (c),A
	ld a,00000100b		;no interrupt in CH B, special RX condition affects vect
	out (c),A

	ld a,02h			;write into WR0: select WR2
	out (c),A
	ld a,0h				;write into WR2: cmd line int vect (see int vec table)
						;bits D3,D2,D1 are changed according to RX condition

	out (c),A

	ld a,01h			;write into WR0: select WR1
	out (c),A
	ld a,00011000b		;interrupt on all RX characters, parity is not a spec RX condition
						;buffer overrun is a spec RX condition
	out (c),A

	pop af
	ret

//HL: string pointer
//DE: (A/B)DATA
label(puts):	
	push bc
	push af
	ld a,(hl)
	xor 0
	jp z,label(end)
	ld b,h
	ld c,l
	out (c),A
	inc hl
    ;delay some 
	ld bc,0xFFE0
label(beep_function_delay):
	nop
	nop
	nop
	inc bc
	ld a,b
	xor 0x00
	jr nz,label(beep_function_delay)
	ld a,c
	xor 0x00
	jr nz,label(beep_function_delay)
	jp label(puts)
label(end):
	ret
