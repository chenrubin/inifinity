# We want to create uid.out.

# Therefor we need to link it with uid_tests.o libuid_shared.so
uid : uid_tests.o libuid_shared.so
	gcc -ansi -pedantic-errors -Wall -Wextra -g uid_tests.o -luid_shared -o uid.out

# Creating shared library that ,in this case, contains only one object file 
# which is uid.o
libuid_shared.so : uid.o
	gcc -shared uid.o -o /usr/lib/libuid_shared.so

# creating uid.o
uid.o : uid.c uid.h
	cc -c uid.c uid.h -fPIC #-c means only compile without linking

# crating uid_tests.o	
uid_tests.o : uid_tests.c uid.h
	cc -c uid_tests.c uid.h -fPIC #-c means only compile without linking
