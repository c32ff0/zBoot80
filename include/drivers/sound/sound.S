#if AY3891X > 0 
#include <drivers/sound/AY3891X.S>
#endif

#if YM3812 > 0 
#warning Support for the YM3812 is not supported yet
#endif

#undef label
#define label(a) sound_##a

#if ( AY3891X > 0 ) || ( YM3812 > 0 )
#	define Sound_is_enabled 1
#else
#	define Sound_is_enabled 0
#endif

#if ( FUNCTION_BEEP == 1 ) && ( Sound_is_enabled == 1)
label(beep):
#if AY3891X > 0
	ld bc,AY3891Xaddr0
	call AY3891Xbeep_start
#if AY3891X > 1
	ld bc,AY3891Xaddr1
	call AY3891Xbeep_start
#if AY3891X > 2
	ld bc,AY3891Xaddr2
	call AY3891Xbeep_start
#if AY3891X > 3
	ld bc,AY3891Xaddr3
	call AY3891Xbeep_start
#endif
#endif
#endif
#endif
	ret 

label(delay_loop):
	nop
	nop
	nop
	inc bc
	ld a,b
	xor 0x00
	jr nz,label(delay_loop)
	ld a,c
	xor 0x00
	jr nz,label(delay_loop)
#if AY3891X > 0
	ld bc,AY3891Xaddr0
	call AY3891Xmute
#if AY3891X > 1
	ld bc,AY3891Xaddr1
	call AY3891Xmute
#if AY3891X > 2
	ld bc,AY3891Xaddr2
	call AY3891Xmute
#if AY3891X > 3
	ld bc,AY3891Xaddr3
	call AY3891Xmute
#endif
#endif
#endif
#endif
	ret

#endif
