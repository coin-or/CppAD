#! /bin/bash
#
message="
usage: adolc_usrparms.sh <dir> 
       adolc_usrparms.sh <dir> <bufsize> <tbufsize> show
       adolc_usrparms.sh <dir> <bufsize> <tbufsize> modify

The frist usage prints the value of BUFSIZE and TBUFSIZE.
The second usage show how the usrparms.h file would be modified.
The third usage actually modifies the file.

<dir>: 
prefix directory corresponding to Adolc; i.e., the file adouble.h is in
	<dir>/include/adolc/usrparms.h

BUFSIZE: 
Buffer size for tapes.

TBUFSIZE: 
Buffer size for temporary Taylor store.

<bufsize>: 
the value we are changing the Adolc BUFSIZE parameter to.

<tbufsize>: 
the value we are changing the Adolc TBUFSIZE parameter to.
"
if [ "$1" == "" ]
then
	echo "$message"
	exit 1
fi
file="$1/include/adolc/usrparms.h"
if [ ! -e $file ]
then
	echo "adolc_usrparms.sh: cannot find the file $file"
	exit 1
fi 
#
# case where we print the value of BUFSIZE and  TBUFSIZE
if [ "$2" == "" ]
then
	grep "^#define T*BUFSIZE" < $file  
	exit 0
fi
prev="// Previous value: "
cmd_one="s|^#define BUFSIZE\( *\)\([0-9]*\).*|$prev &\n#define BUFSIZE\1$2|"
cmd_two="s|^#define TBUFSIZE\( *\)\([0-9]*\).*|$prev &\n#define TBUFSIZE\1$3|"
if [ "$4" == "show" ]
then
	sed < $file > adolc_usrparms.tmp \
		-e "$cmd_one" -e "$cmd_two"
	diff $file adolc_usrparms.tmp
	exit 0
fi
if [ "$4" == "modify" ]
then
	sed < $file > adolc_usrparms.tmp \
		-e "$cmd_one" -e "$cmd_two"
	diff $file adolc_usrparms.tmp
	mv adolc_usrparms.tmp $file
	exit 0
fi
echo "$message"
exit 1
