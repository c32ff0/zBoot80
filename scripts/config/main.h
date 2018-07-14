#include <stdint.h>
#include <inttypes.h>

#define maxinputsize 20
#define VERSION "0.1"
#define minram 32
#define minramstr "32"

#define device(dev) uint8_t dev; uint16_t* dev##addr ; uint8_t dev##test

int ask(char* title,char* help,uint8_t def);
int askn(char* title,int errorret,int size);
unsigned int asknx(char* title,int errorret,int size);

struct options_t{
//	SOUND
		int atleastonesound;
		device(AY3891X);
		device(YM3812);
//	DISPLAY
		int atleastonedisplay;
		device(ONEBYTEDISP);
//  coprocessors
		int atleastonecoprocessor;
		device(AM9511);
//	Timer/counters
		int atleastonetimer;
		device(Z84C30XX);
		int Z84C30XXINT;
//	Serial I/O
		int atleastoneserial;
		device(Z84C40XX);
	uint16_t STARTADDR;
	uint16_t RAMSTART;
	uint16_t RAMEND;
	uint8_t NMI_HANDLER;
	uint8_t FUNCTION_BEEP;
	uint8_t BEEPONSTARTUP;
};


//      confdev(name in struct options_t, name to be printed out);
#define confdev(name,namestr,help,doask,atleastone) \
			options.name=0;\
			options.name##test=0;\
			if(doask)\
				while(1){\
					temp=askn("Enter the amount of "namestr" that exist in the system (0-4,?)",5,0);\
					if(temp==-3)\
						break;\
					if(temp==-2)\
						puts(help);\
					else{ \
						if(temp>=1&&temp<=4){\
							int teemp=1;\
							for(int i=0;i<temp&&teemp;i++){\
								while(1){\
									tempbuf[i]=asknx((!i)?"Enter the base address in the I/O space of one \""namestr"\" in hexadecimal":"Enter the base address of another "namestr" in hexadecimal",0x10000,54+strlen(namestr));\
									if(tempbuf[i]==0X10000){\
										tempbuf[0]=0x10000;\
										teemp=0;\
										break;\
									}else if(tempbuf[i]>=0)\
										break;\
								}\
							}\
							if(tempbuf[0]<0xFFFF){\
								options.atleastone=1;\
								options.name=temp;\
								options.name##addr=malloc(temp*2/*the address is 16bit*/);\
								for(int i=0;i<temp;i++)\
									options.name##addr[i]=tempbuf[i];\
							}else\
								puts("Aborted");\
						}\
						if(temp>=0&&temp<=4)\
							break;\
					}\
				}
#define confdevseting(name,subname,text,help,doask){\
			options.name##subname=0;\
			if(doask)\
				if(temp>=1&&temp<=4){\
					for(int i=0;i<temp;i++){\
						tempbuf[i]=ask(text,help,0);\
					}\
					if(tempbuf[0]&&temp>0)options.name##subname|=0x1;\
					if(tempbuf[1]&&temp>1)options.name##subname|=0x2;\
					if(tempbuf[2]&&temp>2)options.name##subname|=0x4;\
					if(tempbuf[3]&&temp>3)options.name##subname|=0x8;\
				}\
}

#define confsettingx(title,name,help,doask,def) \
			options.name=0;\
			if(doask)\
				while(1){\
					temp=asknx(title,0x10000,0);\
					if(temp==-3&&def>=0){\
						options.name=def;\
						break;\
					}else if(temp==-2)\
						puts(help);\
					else{ \
						if(temp>=0x0000&&temp<=0xFFFF){\
							options.name=temp;\
							break;\
						}\
					}\
				}

#define confsetting(title,name,help,doask,def){ \
			options.name=def;\
			if(doask)\
				while(1){\
					temp=ask(title,help,0);\
					if(temp==-3&&def>=0){\
						break;\
					}else if(temp==-2)\
						puts(help);\
					else{ \
						if(temp!=0x10000){\
							options.name=temp;\
							break;\
						}\
					}\
				}\
			}

#define space 25

#define printsettingx(dev,devname) \
					left=space-fprintf(stderr,"#define "devname" ");\
					while(left--)fprintf(stderr," ");\
					if(sizeof(options.dev)==1){\
						fprintf(stderr," 0x%"PRIx8 "\n",options.dev);\
					}else{\
						fprintf(stderr," 0x%"PRIx16"\n",options.dev);\
					}

#define printsetting(dev,devname) \
					left=space-fprintf(stderr,"#define "devname" ");\
					while(left--)fprintf(stderr," ");\
					if(sizeof(options.dev)==1){\
						fprintf(stderr," 0x%"PRIu8 "\n",options.dev);\
					}else{\
						fprintf(stderr," 0x%"PRIu16"\n",options.dev);\
					}

#define printdev(dev,devname)	left=space-fprintf(stderr,"#define "devname);\
								while(left--)fprintf(stderr," ");\
								fprintf(stderr," %"PRIu16"\n",options.dev);\
								left=space-fprintf(stderr,"#define "devname"test");\
								for(int i=0;i<left;i++)fprintf(stderr," ");\
								fprintf(stderr," %"PRIu8"\n",options.dev##test);\
								for(int i=0;i<options.dev;i++){\
                              		left=space-fprintf(stderr,"#define "devname"addr%d",i);\
									for(int i=0;i<left;i++)fprintf(stderr," ");\
									fprintf(stderr," 0x%"PRIx16"\n",options.dev##addr[i]);\
							  	}

#define askfortest(dev,devname,help,doask,def)\
		if(options.dev){\
			if(doask){\
				options.dev##test=ask("Test the "devname,help,0);\
			}else{\
				options.dev##test=def;\
			}\
		}
