#define label(a) Z84C30XX##a
; This is the source file for the Z80 CTC

; Some condensed info:

; Intertupt vector address:  < I Register (8) > < Peripherial (7) > < 0 (1) >
;                               ________________/                 \_____
;                                 < brogrammable (5) > < channel (2) >
 
; SETTING UP A CHANNEL
; 1) out control word to channel  INT MODE PRESCSALE CLK/TRG TRIG CONST RESEET VECTOR

;CTC CONTROL REGISTER BIT FUNCTIONS
label(CTC_CONTROL_MODE):	equ 0x01
label(CTC_SOFT_RESET):		equ 0x02
label(TIME_CONSTANT_FOLLOWS):	equ 0x04
label(TIMER_TRIGGER_ENABLED):	equ 0x08 ; Clk/trg pulse stars timer if this bit is 1
label(CLK_TRG_RISING_EDGE):	equ 0x10
label(PRESCALER_16X):		equ 0x00
label(PRESCALER_256X):		equ 0x20
label(COUNTER_MODE):		equ 0x40
label(ENABLE_INTERRUPT):	equ 0x80

//BC: chip base addr
#if ( Z84C30XX == 1 ) && ( Z84C30XXINT == 0x1 )
#define Z84C30XXALLINTERUPTS
#elif ( Z84C30XX == 2 ) && ( Z84C30XXINT == 0x3 )
#define Z84C30XXALLINTERUPTS
#elif ( Z84C30XX == 3 ) && ( Z84C30XXINT == 0x7 )
#define Z84C30XXALLINTERUPTS
#elif ( Z84C30XX == 4 ) && ( Z84C30XXINT == 0xf )
#define Z84C30XXALLINTERUPTS
#endif

//#ifndef Z84C30XXALLINTERUPTS //if we are going to setup channels without interrupts
label(Start_Channel_A): ; This code should generate a slow pulse on channel 0 relative to CPU's clock
push af
ld a, label(CTC_CONTROL_MODE) | label(CTC_SOFT_RESET) | label(TIME_CONSTANT_FOLLOWS) | label(PRESCALER_256X)
out (c),a
ld a,0xFF
out (c),a
ld a,0x00;
out (c),a
pop af
ret
//#endif

#if Z84C30XXINT != 0  //if we are going to setup channels without interrupts
//BC: chip base addr
label(Setup_Interupts):
push af
ld a, label(CTC_CONTROL_MODE) | label(CTC_SOFT_RESET) | label(TIME_CONSTANT_FOLLOWS) | label(PRESCALER_256X) | label(ENABLE_INTERRUPT)
out (c),a
ld a,0xFF ; time constant AFh defined
out (c),a ; and loaded into channel 3
ld A, Z84C30XXitable0 & 0x00F8 ; it vector defined in bit 7­3,bit 2­1 don't care, bit 0 = 0
out (c),a ; and loaded into channel 0
ld a, Z84C30XXitable0/256
ld i, a
im 2
ei
pop af
ret
#endif


#if ( Z84C30XXINT & 0x1 ) > 0
Z84C30XX0intchA:
Z84C30XX0intchB:
Z84C30XX0intchC:
Z84C30XX0intchD:
	pop af
#if ONEBYTEDISP > 0
	ld bc,ONEBYTEDISPaddr0
	out (c),a
#if ONEBYTEDISP > 1
	ld bc,ONEBYTEDISPaddr1
	out (c),a
#if ONEBYTEDISP > 2
	ld bc,ONEBYTEDISPaddr2
	out (c),a
#if ONEBYTEDISP > 3
	ld bc,ONEBYTEDISPaddr3
	out (c),a
#endif
#endif
#endif
#endif
	inc a
	push af
	ei
	reti
#endif
