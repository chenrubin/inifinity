CFLAGS := -ansi -pedantic-errors -Wall -Wextra -g
CSOFLAGS := -shared
CC := gcc
MAINS_C := $(filter %tests.c, $(wildcard */*.c))
MAINS_O := $(subst .c,.o, $(MAINS_C))
SOURCES_C := $(filter-out %tests.c, $(wildcard */*.c)) #uid/uid.c
SOURCES_O := $(subst .c,.o, $(SOURCES_C)) #uid/uid.o
NAMES_WITH_PATH := $(subst .c,,$(SOURCES_C))
DEP = $(subst .o,.d, $(SOURCES_O)) #uid/uid.d
BIN = /bin
#BINS_O = $(patsubst %.o,%.o ,/bin/$(subst .c,.o,$(SOURCES_O)))
#BINS_O_SEPARATE = &(subst )

#all: libds_shared.so $(MAINS_O)
#all:
#	echo $(filter bin%,$(subst /, bin/,u))
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
	echo '\t'$(CC) -c -fPIC -o $(filter bin% ,$(subst /, bin/,$(subst .c,.o,$<))) -c $< >> $@
							   
-include $(DEP)

#$(BIN) : 
#	mkdir $(BIN)
	
#%.o : %.c
#	$(CC) -c $(CFLAGS) $< -fPIC
	
.PHONY: clean
clean:
	rm $(DEP)
	rm $(filter bin% ,$(subst /, bin/,$(SOURCES_O)))
	
# | $(BIN)	
