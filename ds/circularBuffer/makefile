# We want to create cbuff.out

# Therefor we need to link it with cbuff_tests.o libcbuff_shared.so
cbuff : cbuff_tests.o libcbuff_shared.so
	gcc -ansi -pedantic-errors -Wall -Wextra -g cbuff_tests.o -lcbuff_shared -o cbuff.out

# Creating shared library that ,in this case, contains only one object file 
# which is cbuff.o
libcbuff_shared.so : cbuff.o
	gcc -shared cbuff.o -o /usr/lib/libcbuff_shared.so

# creating cbuff.o
cbuff.o : cbuff.c cbuff.h
	cc -c cbuff.c cbuff.h -fPIC #-c means only compile without linking

# crating cbuff_tests.o	
cbuff_tests.o : cbuff_tests.c cbuff.h
	cc -c cbuff_tests.c cbuff.h -fPIC #-c means only compile without linking
