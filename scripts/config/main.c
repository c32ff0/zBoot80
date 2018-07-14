#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

//TODO phrase the questions and help better

int main(){
	int temp;
	int tempbuf[4];
	struct options_t options;
	int left;
	//confdev is a macro in main.h
	int doask;//if doask is 0 (user sed no) confdev() will just clear the regiter
	puts("zBoot80 Configuration program V"VERSION" .");
	puts("You can press Enter in an option for the default answer.");
	puts("in parenthesis, there are comma spereated avilable options");
	puts("Basic system settings:");
	confsettingx("What is going to be the address of the first byte of the bootloader ? (0x0000-0xFFFF,0x?) ",STARTADDR,"The z80 instruction set doesn't allow for Potition independant code and as such, the potiton where the output binary is going to be placed matters. This option is about where the bootloader binary is going to be placed. The default value of 0x0000 is because the z80 starts executing at 0x0000",1,0x0000);
configram:
	confsettingx("What is the start address of unbanked RAM (start<end) (0x0000-0xFFFF,0x?) ",RAMSTART,"This is the Starting address of a location in memmory that can be safely overwritten and read back with the data intact. The next question will be the end address and the start address must be a lower number than the end address. ex start:0xEFFF end:0xFFFF. This region must be at least "minramstr"bytes.",1,-1);

	confsettingx("What is the end address of unbanked RAM (start<end) (0x0000-0xFFFF,0x?) ",RAMEND,"This is the end address of a location in memmory that can be safely overwritten and read back with the data intact. The start address must be a lower number than the end address. ex start:0xEFFF end:0xFFFF. This region must be at least "minramstr"bytes.",1,-1);

	if(options.RAMSTART+minram>options.RAMEND){
		printf("Invalid RAM address values.%s\n",(options.RAMSTART>options.RAMEND)?"start RAM address can't be higher than the end RAM address":"Ram size isn't at least"minramstr"Bytes");
		goto configram;
	}

	confsetting("Add support for an NMI handler ",NMI_HANDLER,"since when the NMI pin goes low the CPU jumps to a specific non changable location, this option puts a RET instruction in there so code execution can continue. This opsion, dpending on the location of the bootloader could increase the size of the binary quite a bit.",options.STARTADDR+2<=0x66,0);
	
	doask=ask("Enable Sound","This option will make the script ask for the specific sound chip(s) used (up to 4 identical) in the system.",1);

	confdev(AY3891X,"AY-3-891x","The AY-3-8910 AY-3-8912 and AY-3-8913 sound chips ",doask,atleastonesound);
	confdev(YM3812,"YM3812(OPL2)","The Yamaha YM3812, aka OPL2 . Used in the adlib and Sound Blaster cards",doask,atleastonesound);

	doask=ask("Enable Disply","This option will make the script ask for the kind of displays used in the system",1);

	confdev(ONEBYTEDISP,"One byte display","This is a byte in IO space that is displayed in a (usualy) hexadecimal display like a doule til311.",doask,atleastonedisplay);
	
	doask=ask("Enable coprocessors","This option will make the script ask for which coprocessors are present ike math coprocessors",1);

	confdev(AM9511,"AM9511 Math coprocessor","No help available",doask,atleastonecoprocessor);

	doask=ask("Enable Timers","This option will make the script ask for which time chips are present like the Z80 CTC.It enables also some commands related to Timers and Counters",1);

	confdev(Z84C30XX,"Z80 CTC (Z84C30xx)","This enables support for the Zilog Z84C30xx IC (the Z80 CTC)",doask,atleastonetimer);
	confdevseting(Z84C30XX,INT,"enable interrupts for the acording CTC ","the first time you answer this question, you will be turning off or on the interrupts for the first the CTC in the first address you entered in the previous questions. for example if you entered 2 CTCs and the first one you set addr 2 and the second on addr 3 , the first time you get asked, it will be about the one in addr 2 and the second time it will be about the one in addr 3. i hope this that makes sense",doask);

	doask=ask("Enable serial I/O","This option will make the script ask for which time chips are present for Serial comunication.",1);

	confdev(Z84C40XX,"Z80 SIO (Z84C40XX)","This enables support for the Zilog Z84C40xx IC (the Z80 SIO)",doask,atleastoneserial);

	doask=ask("Manually disable some functions (by default all enabled )","You can disable some functions that might be useless to you like beep. Note that in this example if no audio devices are present you won't be asked about the beep function because it can't be enabled.",0);

	confsetting("Have beep function enabled",FUNCTION_BEEP,"The beep function plays a tone in all available channels of all availables sound chips, waits a bit and mutes everything!.",doask&&options.atleastonesound,options.atleastonesound);

	if(options.AM9511||options.FUNCTION_BEEP)
		doask=ask("Perform self check startup","Following will be asked which devices should be tested",0);
	askfortest(AM9511,"AM9511","perform simple math calculations and compare the resaults to the expected answers.",doask,0);
	doask=ask("Modify startup behaviour","Change things like wether the computer should beep on startup",0);
	confsetting( "execute the beep function on startup",BEEPONSTARTUP,"No help available ",options.FUNCTION_BEEP&&doask,0);


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;/*print output*/;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	fprintf(stderr,";This is a file generated by zBoot80 configurator V"VERSION"\n");	
	printdev(AY3891X,"AY3891X");
	printdev(YM3812,"YM3812");
	printdev(ONEBYTEDISP,"ONEBYTEDISP");
	printdev(AM9511,"AM9511");
	printdev(Z84C30XX,"Z84C30XX");
	printsettingx(Z84C30XXINT,"Z84C30XXINT");
	printdev(Z84C40XX,"Z84C40XX");
	printsettingx(STARTADDR,"STARTADDR");
	printsettingx(RAMSTART,"RAMSTART");
	printsettingx(RAMEND,"RAMEND");
	printsetting(NMI_HANDLER,"NMI_HANDLER");
	printsetting(FUNCTION_BEEP,"FUNCTION_BEEP");
	printsetting(BEEPONSTARTUP,"BEEPONSTARTUP");

}

int ask(char* title,char* help,uint8_t def){
	char res[maxinputsize+2];
	while(1){
		printf("%s?  %s ",title,def?"(Y,n,?)":"(y,N,?)");
		if(fgets((char*)&res,maxinputsize,stdin)!=NULL){
			if(res[0]=='y'||res[0]=='y'||res[0]=='n'||res[0]=='n'||res[0]=='\n'||res[0]==EOF)
				break;
			else if(res[0]=='?')
				puts(help);
		}
	}
	if(res[0]==EOF)
		return 0;
	return (res[0]=='\n')?def:res[0]=='Y'||res[0]=='y';
}

int askn(char* title,int errorret,int size){
	char* buf=malloc(maxinputsize+2);
	int res;
	size-=strlen(title)+2;
	printf("%s ",title);
	for(int i=0;i<size;i++)
		putchar(' ');
	if(fgets(buf,maxinputsize,stdin)==NULL)
		return errorret;
	if(buf[0]=='?')
		return -2;
	if(buf[0]=='\n')
		return -3;
	if(sscanf(buf,"%x",&res)!=1)
		return errorret;
	return res;
}

unsigned int asknx(char* title,int errorret,int size){
	char* buf=malloc(maxinputsize+2);
	int res;
	size-=strlen(title)+2;
	printf("%s? ",title);
	for(int i=0;i<size-2;i++)
		putchar(' ');
	printf("0x");
	if(fgets(buf,maxinputsize,stdin)==NULL)
		return errorret;
	if(buf[0]=='?')
		return -2;
	if(buf[0]=='\n')
		return -3;
	if(sscanf(buf,"%x",&res)!=1)
		return errorret;
	return res;
}
