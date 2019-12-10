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
#BINS_O = $(patsubst %.o,%.o ,/bin/$(subst .c,.o,$(SOURCES_O)))
#BINS_O_SEPARATE = &(subst )

#all: libds_shared.so $(OUT_FILES)
all:
	echo $(filter bin%,$(subst /, bin/,$(SOURCES_O)))
#	echo $(filter test%,$(subst /, test/,$(subst .c,.out,$(SOURCES_C)))) : $(SOURCES_C) lib/libds_shared.so
#	echo $(filter test%,$(subst /, test/,$(subst .c,.out,$(SOURCES_C)))) # test/uid.out
#	echo $(filter bin% ,$(subst /, bin/,$(subst .c,.o,$(SOURCES_C)))) #bin/uid.o
#	echo $(filter bin% ,$(subst /, bin/,$(subst .c,.o,$(SOURCES_C))))
#	echo $(filter bin% ,$(subst /, bin/,$(subst .c,.o,uid/uid.c)))
#	echo $(firstword $(BINS_O))/$(lastword $(BINS_O)))
#	echo $(firstword $(subst /, ,/bin/$(subst .c,.o,$<)))/$(lastword $(subst /, ,/bin/$(subst .c,.o,$<)))
#	echo $(MAINS_C)
#	echo $(MAINS_O) #test.o files textwise
#	echo $(SOURCES_O) #regularFiles.c without test.c
#	echo $(NAMES_WITH_PATH) # pq,sortedlist,...
#	echo $(NAMES) # pq,sortedlist,...

#creating shared library from all o files (not including tests.o)
libds_shared.so : $(filter bin%,$(subst /, bin/,$(SOURCES_O)))
	$(CC) $(CSOFLAGS) $^ -o lib/$@

#compiling c files to o files (kind of loop)
%.d : %.c
	$(CC) -MM $^ -MT $(filter bin%,$(subst /, bin/,$(subst .c,.o,$<))) > $@
	echo '\t'$(CC) -c  $< -fPIC -o $(filter bin% ,$(subst /, bin/,$(subst .c,.o,$<))) >> $@
							   
-include $(DEP)

#compiling tests.c files to tests.o files
#%_tests.o : %tests.c
#	$(CC) -c $<

#Generating *.out files
%.t : %.c
	echo $(filter test%,$(subst /, test/,$(subst .c,.out,$<))) : $(subst .c,_tests.o,$<) lib/libds_shared.so > $@
	echo '\t'$(CC) $(subst .c,_tests.o,$<) lib/libds_shared.so -o $(filter test%,$(subst /, test/,$(subst .c,.out,$<))) >> $@

-include $(T_FILES)	

#$(BIN) : 
#	mkdir $(BIN)
	
.PHONY: clean

clean:
	rm $(DEP)
	rm $(filter bin% ,$(subst /, bin/,$(SOURCES_O)))
	rm $(T_FILES)
	rm $(MAINS_O)
	rm $(OUT_FILES)
	
