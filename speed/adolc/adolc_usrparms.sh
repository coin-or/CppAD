#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
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
The distribution directory corresponding to adolc; e.g., adolc-1.10.2. 
The file where the buffer sizes are specified is <dir>/adolc/usrparms.h.

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
file="$1/adolc/usrparms.h"
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
same="/* Previous: \1\2 */ \3\n#define"
cmd_one="s|^\(#define BUFSIZE *\)\([0-9]*\)\(.*\)|$same BUFSIZE    $2|"
cmd_two="s|^\(#define TBUFSIZE *\)\([0-9]*\)\(.*\)|$same TBUFSIZE   $3|"
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
	echo "Execute the following commands for the change to take effect:"
	echo "cd $1"
	echo "make"
	echo "make install"
	exit 0
fi
echo "$message"
exit 1
