# We want to create stack.out

# Therefor we need to link it with stack_tests.o libstack_shared.so
stack : stack_tests.o libstack_shared.so
	gcc -ansi -pedantic-errors -Wall -Wextra -g stack_tests.o -lstack_shared -o stack.out

# Creating shared library that ,in this case, contains only one object file 
# which is stack.o
libstack_shared.so : stack.o
	gcc -shared stack.o -o /usr/lib/libstack_shared.so

# creating stack.o
stack.o : stack.c stack.h
	cc -c stack.c stack.h -fPIC #-c means only compile without linking

# crating stack_tests.o	
stack_tests.o : stack_tests.c stack.h
	cc -c stack_tests.c stack.h -fPIC #-c means only compile without linking
