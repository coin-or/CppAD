#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
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
# run doxygen
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
# -----------------------------------------------------------------------------
# check for warnings and errors
#
doxygen_version=`doxygen --version  |
	sed -e 's|\.|*100+|' -e 's|\.|*10+|' -e 's|\..*||'`
let doxygen_version=$doxygen_version
if (( $doxygen_version <= 155 ))
then
	doxygen_version=`doxygen --version`
	echo "doxygen version $doxygen_version is <= 1.5.6"
	echo "Hence it is to old to check for warnings or errors."
	exit 0
fi
if (( $doxygen_version == 163 ))
then
	doxygen_version=`doxygen --version`
	echo "doxygen version $doxygen_version is == 1.6.3"
	echo "Hence it has a problem with warnings about missing # defines;"
	echo "see http://comments.gmane.org/gmane.text.doxygen.general/8594"
	exit 0
fi
list=`head doxygen.err`
if [ "$list" == "" ]
then
	echo 'run_doxygen.sh OK'
	exit 0
fi
echo 'bin/run_doxygen.sh: Doxygen errors or warnings; see doxygen.err'
echo 'run_doxygen.sh: Error'
exit 1
