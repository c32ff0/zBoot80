INPUT=main.c
OPTIONS=-Wall -Wextra -O2 -std=c99
CMDOPTIONS=
LIBS=
ODIRS=
COMPILER=gcc
OUTPUT=clico
INSTALL_DIR=/usr/bin/

$(OUTPUT):$(subst .c,.o,$(INPUT)) $(shell ls *.h)
	@echo CC $(OUTPUT)
	@$(COMPILER) $(LIBS) $(ODIRS) $(OPTIONS) $(CMDOPTIONS) $< -o $(OUTPUT)

%.o:%.c $(shell ls *.h)
	@echo CC $@ 
	@$(COMPILER) $(LIBS) $(ODIRS) $(OPTIONS) -c 	$(CMDOPTIONS) $< -o $@

clean:
	rm -f $(OUTPUT) $(subst .c,.o,$(INPUT))
