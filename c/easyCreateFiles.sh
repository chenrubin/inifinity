#!/bin/bash

#creating files
touch $1.c
touch $1_tests.c

echo "/************************************" > $1.c
echo "*		Author: ChenR				  *" >> $1.c
echo "*		Reviewer: 					  *" >> $1.c
echo "*		$1							  *" >> $1.c
echo "*		7/11/2019					  *" >> $1.c
echo "*									  *" >> $1.c
echo -e "************************************/\n" >> $1.c

echo -e "#include <stdio.h> /* printf */\n" >> $1.c
echo -e "#include \"$1.h\"\n" >> $1.c

echo -e "#include <stdio.h> /* printf */\n" > $1_tests.c
echo -e "#include \"$1.h\"\n" >> $1_tests.c

echo "#define KNRM  \"\x1B[0m\"" >> $1_tests.c
echo "#define KRED  \"\x1B[31m\"" >> $1_tests.c
echo "#define KGRN  \"\x1B[32m\"" >> $1_tests.c
echo "#define KYEL  \"\x1B[33m\"" >> $1_tests.c
echo "#define KBLU  \"\x1B[34m\"" >> $1_tests.c
echo "#define KMAG  \"\x1B[35m\"" >> $1_tests.c
echo "#define KCYN  \"\x1B[36m\"" >> $1_tests.c
echo "#define KWHT  \"\x1B[37m\"" >> $1_tests.c
echo "" >> $1_tests.c

echo "#define PRINTTESTRESULTS(func,num, res)\\" >> $1_tests.c
echo "(printf("%-55s: Test %d %s\\n" KNRM,func, num, (res) == 1 ?\\" >> $1_tests.c
echo "KGRN "\\t\\tpassed" : KRED "\\t\\tfailed"))" >> $1_tests.c

echo -e "\n\nint main()" >> $1_tests.c
echo -e "{\n\n" >> $1_tests.c
echo -e "\treturn 0;" >> $1_tests.c
echo "}" >> $1_tests.c
