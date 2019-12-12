# We want to create slist.out

# Therefor we need to link it with slist_tests.o libslist_shared.so
slist : slist_tests.o libslist_shared.so
	gcc -ansi -pedantic-errors -Wall -Wextra -g slist_tests.o -lslist_shared -o slist.out

# Creating shared library that ,in this case, contains only one object file 
# which is slist.o
libslist_shared.so : slist.o
	gcc -shared slist.o -o /usr/lib/libslist_shared.so

# creating slist.o
slist.o : slist.c slist.h
	cc -c slist.c slist.h -fPIC #-c means only compile without linking

# crating slist_tests.o	
slist_tests.o : slist_tests.c slist.h
	cc -c slist_tests.c slist.h -fPIC #-c means only compile without linking
