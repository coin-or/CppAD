# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
echo "Some packages have bad aclocal macros that generate warnings: see"
echo "http://sources.redhat.com/automake/automake.html#Extending-aclocal"
echo "This shell script will attempt to fix them."
echo
#
aclocal >& junk
list=`grep 'aclocal/.*.m4:[0-9]*: *warning' < junk | sed -e"s/\.m4:.*/.m4/"`
#
emptylist=true
for file in $list
do
    emptylist=false
    oldtext=`grep '^ *AC_DEFUN *( *AM_PATH_[A-Za-z_]* *,' $file`
    if [ "${oldtext}NOTHING" != "NOTHING" ]
    then
        sed < $file > junk -e \
        's/^ *AC_DEFUN *( *AM_PATH_\([A-Za-z_]*\) *,/AC_DEFUN([AM_PATH_\1],/' 
        newtext=`grep 'AC_DEFUN(\[AM_PATH_[A-Za-z_]*\],' junk`
        echo "$file"
        echo "$oldtext -> $newtext"
        if [ -w $file ]
        then
            mv junk $file
        else
            echo "Do not have premission to change this file."
            rm junk
        fi
    else
        echo "$file"
        echo "Do not know how to fix this file."
    fi
done
if [ $emptylist == true ]
then
    echo "No aclocal warnings to be fixed."
fi
