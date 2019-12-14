CFLAGS := -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
DFLAGS := -ansi -pedantic-errors -Wall -Wextra -g
CSOFLAGS := -shared
CC := gcc
MAINS_C := $(filter %tests.c, $(wildcard */*.c)) #uid/uid_tests.c
MAINS_O := $(subst .c,.o, $(MAINS_C))
SOURCES_C := $(filter-out %tests.c, $(wildcard */*.c)) #uid/uid.c
SOURCES_O := $(subst .c,.o, $(SOURCES_C)) #uid/uid.o
OUT_FILES := $(filter test% ,$(subst /, test/,$(subst .o,.out,$(SOURCES_O)))) #test/uid.out
DEP = $(subst .o,.d, $(SOURCES_O)) #uid/uid.d	
T_FILES = $(subst .o,.t, $(SOURCES_O)) #uid/uid.t
LIBS = lib/Release/libds_shared.so lib/Debug/libds_shared.so

all: release Debug

.PHONY: release
release: XFLAGS := $(CFLAGS)
release: BIN := bin/Release/
release: LIB := lib/Release/libds_shared.so
release:
	echo $(BIN) $(LIB)
release: lib/Release/libds_shared.so 


debug: XFLAGS := $(DFLAGS)  BIN := bin/Debug/ LIB := lib/Debug/libds_shared.so $(LIB)
	
test: lib/Debug/libds_shared.so $(OUT_FILES)

#creating release shared library from all o files (not including tests.o)
lib/Release/libds_shared.so : $(filter bin%,$(subst /, $(BIN),$(SOURCES_O)))
	$(CC) $(CSOFLAGS) $^ -o $@
	
#creating debug shared library from all o files (not including tests.o)
lib/Debug/libds_shared.so : $(filter bin%,$(subst /, $(BIN),$(SOURCES_O)))
	$(CC) $(CSOFLAGS) $^ -o $@

#compiling c files to o files (kind of loop)
%.d : %.c
	$(CC) -MM $^ -MT $(filter bin%,$(subst /, $(BIN),$(subst .c,.o,$<))) > $@
	echo '\t'$(CC) $(XFLAGS) -c  $< -fPIC -o $(filter bin% ,$(subst /, $(BIN),$(subst .c,.o,$<))) >> $@
							   
-include                           (DEP)

%.t : %.c
	echo $(filter test%,$(subst /, test/,$(subst .c,.out,$<))) : $(subst .c,_tests.o,$<) $(LIB) > $@
	echo '\t'$(CC) $(XFLAGS) $(subst .c,_tests.o,$<) $(LIB) -o $(filter test%,$(subst /, test/,$(subst .c,.out,$<))) >> $@

-include $(T_FILES)
	
.PHONY: clean

clean:
	rm $(DEP)
	rm $(filter bin% ,$(subst /, bin/Release/,$(SOURCES_O)))
	rm $(filter bin% ,$(subst /, bin/Debug/,$(SOURCES_O)))
	rm $(LIBS)
	rm $(T_FILES)
	rm $(MAINS_O)
	rm $(OUT_FILES)
