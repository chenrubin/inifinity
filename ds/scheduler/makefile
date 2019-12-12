6CFLAGS := -ansi -pedantic-errors -Wall -Wextra -g
CSOFLAGS := -shared
CC := gcc
NAME := $(subst .c,,$(filter-out %test.c, $(wildcard *.c)))

all:
	echo 'gcc -MM &(NAME).c'
	
executable: $(NAME)_test.o lib$(NAME)_shared.so
	$(CC) $(CFLAGS) $(NAME)_test.o -l$(NAME)_shared -o $(NAME).out

lib$(NAME)_shared.so: $(NAME).o
	$(CC) -shared $(NAME).o -o /usr/lib/lib$(NAME)_shared.so

$(NAME).o: $(NAME).c $(NAME).h
	$(CC) -c $(NAME).c $(NAME).h -fPIC

$(NAME)_test.o: $(NAME)_test.c $(NAME).h
	$(CC) -c $(NAME)_test.c $(NAME).h -fPIC
	
.PHONY: clean
clean:
	rm *.o
