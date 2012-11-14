#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/run_omhelp.sh" ]
then
	echo "bin/run_omhelp.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
#
if [ "$1" != "htm" ] && [ "$1" != "xml" ] && [ "$1" != "clean" ]
then
	echo "usage: bin/run_omhelp.sh (htm|xml|clean) [printable]"
	exit 1
fi
if [ "$2" != "" ] && [ "$2" != "printable" ]
then
	echo "usage: bin/run_omhelp.sh (htm|xml|clean) [printable]"
	exit 1
fi
if [ "$1" == "clean" ]
then
	echo "rm -rf doc"
           rm -rf doc
	exit 0
fi
ext="$1"
#
echo "Building doc/*.$ext $2"
if [ ! -e doc ]
then
	echo "mkdir doc"
	      mkdir doc
fi 
echo "cd doc"
      cd doc
cmd="omhelp ../doc.omh -noframe -debug -l http://www.coin-or.org/CppAD/"
if [ "$ext" == "xml" ]
then
	cmd="$cmd -xml"
fi
if [ "$2" == "printable" ]
then
	cmd="$cmd -printable"
fi
echo "$cmd > omhelp.$ext.log"
if !  $cmd > ../omhelp.$ext.log
then
	grep "^OMhelp Error:" ../omhelp.$ext.log
	echo "OMhelp could not build doc/*.$ext documentation."
	echo "See the complete error message in omhelp.$ext.log"
	exit 1
fi
if grep "^OMhelp Warning:" ../omhelp.$ext.log
then
	echo "See the complete warning messages in omhelp.$ext.log."
	exit 1
fi
echo "OK: omhelp $*"
exit 0
