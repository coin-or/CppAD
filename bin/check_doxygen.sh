#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/check_doxygen.sh" ]
then
	echo "bin/check_doxygen.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
if [ ! -e "doxygen.err" ]
then
	bin/run_doxygen.sh
fi
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
	echo "$O: OK"
	exit 0
fi
echo bin/"check_doxygen.sh: Doxygen errors or warnings; see doxygen.err"
if [ "$USER" != "bradbell" ]
then
	exit 0
fi
echo "$O: Error"
exit 1
