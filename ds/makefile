CFLAGS := -ansi -pedantic-errors -Wall -Wextra -g
CSOFLAGS := -shared
CC := gcc
MAINS_C := $(filter %tests.c, $(wildcard */*.c)) #uid/uid_tests.c
MAINS_O := $(subst .c,.o, $(MAINS_C))
SOURCES_C := $(filter-out %tests.c, $(wildcard */*.c)) #uid/uid.c
SOURCES_O := $(subst .c,.o, $(SOURCES_C)) #uid/uid.o
OUT_FILES := $(filter test% ,$(subst /, test/,$(subst .o,.out,$(SOURCES_O)))) #test/uid.out
DEP = $(subst .o,.d, $(SOURCES_O)) #uid/uid.d	
T_FILES = $(subst .o,.t, $(SOURCES_O)) #uid/uid.t
BIN = /bin
LIBS = lib/libds_shared.so


all: lib/libds_shared.so $(OUT_FILES)
#all:
#	echo $(filter bin%,$(subst /, bin/,$(SOURCES_O)))

#creating shared library from all o files (not including tests.o)
lib/libds_shared.so : $(filter bin%,$(subst /, bin/,$(SOURCES_O)))
	$(CC) $(CSOFLAGS) $^ -o $@

#compiling c files to o files (kind of loop)
%.d : %.c
	$(CC) -MM $^ -MT $(filter bin%,$(subst /, bin/,$(subst .c,.o,$<))) > $@
	echo '\t'$(CC) -c  $< -fPIC -o $(filter bin% ,$(subst /, bin/,$(subst .c,.o,$<))) >> $@
							   
-include $(DEP)

%.t : %.c
	echo $(filter test%,$(subst /, test/,$(subst .c,.out,$<))) : $(subst .c,_tests.o,$<) lib/libds_shared.so > $@
	echo '\t'$(CC) $(subst .c,_tests.o,$<) lib/libds_shared.so -o $(filter test%,$(subst /, test/,$(subst .c,.out,$<))) >> $@

-include $(T_FILES)	
	
.PHONY: clean

clean:
	rm $(DEP)
	rm $(filter bin% ,$(subst /, bin/,$(SOURCES_O)))
	rm $(LIBS)
	rm $(T_FILES)
	rm $(MAINS_O)
	rm $(OUT_FILES)
