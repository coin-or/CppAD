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
if [ ! -e "bin/check_doxygen.sh" ]
then
	echo "bin/check_doxygen.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
if ! bin/run_doxygen.sh
then
	echo 'check_doxygen.sh: Error'
	exit 1
fi
echo 'check_doxygen.sh: OK'
exit 0
