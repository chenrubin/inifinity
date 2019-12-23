#!/bin/bash

mkdir $1
cd $1 
#creating files
touch $1.c
touch $1_tests.c
#cp /home/student/chen-rubin/chen/MyUtils.h

echo "/************************************" > $1.c
echo "*		Author: ChenR				  *" >> $1.c
echo "*		Reviewer: 					  *" >> $1.c
echo "*		$1							  *" >> $1.c
echo "*		7/11/2019					  *" >> $1.c
echo "*									  *" >> $1.c
echo -e "************************************/\n" >> $1.c

#echo -e "#include <stdio.h> /* printf */\n" >> $1.c
echo "#include \"$1.h\"" >> $1.c
echo -e "#include \"../../chen/MyUtils.h\" /* MAX2,MIN2 */\n" >> $1.c

echo -e "#include <stdio.h> /* printf */\n" > $1_tests.c
echo "#include \"$1.h\"" >> $1_tests.c
echo -e "#include \"../../chen/MyUtils.h\" /* MAX2,MIN2 */\n" >> $1_tests.c

echo -e "\n\nint main()" >> $1_tests.c
echo -e "{\n\n" >> $1_tests.c
echo -e "\treturn 0;" >> $1_tests.c
echo "}" >> $1_tests.c
