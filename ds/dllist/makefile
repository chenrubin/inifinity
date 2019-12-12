# We want to create dllist.out.

# Therefor we need to link it with dllist_tests.o libdllist_shared.so
dllist : dllist_tests.o libdllist_shared.so
	gcc -ansi -pedantic-errors -Wall -Wextra -g dllist_tests.o -ldllist_shared -o dllist.out

# Creating shared library that ,in this case, contains only one object file 
# which is dllist.o
libdllist_shared.so : dllist.o
	gcc -shared dllist.o -o /usr/lib/libdllist_shared.so

# creating dllist.o
dllist.o : dllist.c dllist.h
	cc -c dllist.c dllist.h -fPIC #-c means only compile without linking

# crating dllist_tests.o	
dllist_tests.o : dllist_tests.c dllist.h
	cc -c dllist_tests.c dllist.h -fPIC #-c means only compile without linking
