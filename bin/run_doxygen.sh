#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != 'bin/run_doxygen.sh' ]
then
	echo 'bin/run_doxygen.sh: must be executed from its parent directory'
	exit 1
fi
echo_eval() {
     echo $* 
     eval $*
}
# -----------------------------------------------------------------------------
version=`bin/version.sh get`
error_file='doxygen.err'
output_directory='doxydoc'
for name in doxyfile $error_file $output_directory
do
	if [ -e $name ]
	then
		echo_eval rm -r $name
	fi
done
echo_eval mkdir doxydoc
echo_eval bin/doxyfile.sh $version $error_file $output_directory
#
echo 'doxygen doxyfile > doxygen.log'
doxygen doxyfile       > doxygen.log
#
echo_eval cat doxygen.err
echo_eval bin/check_doxygen.sh
#
echo 'OK: bin/run_doxygen.sh'
exit 0
