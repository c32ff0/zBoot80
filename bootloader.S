#include ".config"
#if NMI_HANDLER == 1
	defs 0x0066-STARTADDR //init the first 0x66 bytes to 0 in the output file (This is a directive to the z80 assembler)
	ret	//put the ret instruction.
	seek 0 
	jp code
	seek 0x0066-STARTADDR+1//we don't want to overwrite the ret
	org 0x0066-STARTADDR+1//we don't want to overwrite the ret
#else 
	jp code
#endif
#if Z84C30XXINT > 0 // if eny interupt is present
/********** THE INTERUPT TABLE **************/
#if NMI_HANDLER == 1 
	seek 4 ; skip the jp above
	org 4
#endif
#if ( Z84C30XXINT & 0x1 ) > 0
//TODO Fix that constant seek

//TODO no todo but rember that this table could go beyond 0x0066 which might cause problems
seek 0x10
org 0x10
Z84C30XXitable0:
dw	Z84C30XX0intchA
dw	Z84C30XX0intchB
dw	Z84C30XX0intchC
dw	Z84C30XX0intchD
#if ( Z84C30XXINT & 0x2 ) > 0
Z84C30XXitable1:
dw	Z84C30XX1intchA
dw	Z84C30XX1intchB
dw	Z84C30XX1intchC
dw	Z84C30XX1intchD
#if ( Z84C30XXINT & 0x4 ) > 0
Z84C30XXitable2:
dw	Z84C30XX2intchA
dw	Z84C30XX2intchB
dw	Z84C30XX2intchC
dw	Z84C30XX2intchD
#if ( Z84C30XXINT & 0x8 ) > 0
Z84C30XXitable3:
dw	Z84C30XX3intchA
dw	Z84C30XX3intchB
dw	Z84C30XX3intchC
dw	Z84C30XX3intchD
#endif
#endif
#endif
#endif
#if NMI_HANDLER == 1 
	seek 0x0066-STARTADDR+1//let's go back to safely writable part of the rom
	org 0x0066-STARTADDR+1
#endif

#endif

#if Z84C40XX > 0
#	include <drivers/serial/Z84C40XX.S>
#endif
#if Z84C30XX > 0
#	include <drivers/CounterTimers/Z84C30XX.S>
#endif
#include <drivers/sound/sound.S>
code:
	ld SP,RAMEND
#if	BEEPONSTARTUP
	call sound_beep
#endif
#define Z84C30XXSetup_Interupts Z84C30XXStart_Channel_A
#if	Z84C30XX > 0  && (Z84C30XXINT&0x1 == 0x1)
	ld bc,Z84C30XXaddr0
	call Z84C30XXSetup_Interupts
#if	Z84C30XX > 1  && (Z84C30XXINT&0x2 == 0x2)
	ld bc,Z84C30XXaddr1
	call Z84C30XXSetup_Interupts
#if	Z84C30XX > 2  && (Z84C30XXINT&0x4 == 0x4)
	ld bc,Z84C30XXaddr2
	call Z84C30XXSetup_Interupts
#if	Z84C30XX = 4  && (Z84C30XXINT&0x8 == 0x8)
	ld bc,Z84C30XXaddr3
	call Z84C30XXSetup_Interupts
#endif
#endif
#endif
#endif
//#if Z84C40XX > 0
//	call Z84C40XXputs
//#endif
	
halt
