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
if [ ! -e "bin/run_omhelp.sh" ]
then
	echo "bin/run_omhelp.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
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
	echo_eval rm -rf doc
	exit 0
fi
ext="$1"
if [ "$2" == 'printable' ]
then
	printable="yes"
else
	printable='no'
fi
#
echo "Building doc/*.$ext printable=$printable"
if [ ! -e doc ]
then
	echo_eval mkdir doc
fi 
echo_eval cd doc
cmd="omhelp ../doc.omh -noframe -debug -l http://www.coin-or.org/CppAD/"
if [ "$ext" == "xml" ]
then
	cmd="$cmd -xml"
fi
if [ $printable == 'yes' ]
then
	cmd="$cmd -printable"
fi
echo "$cmd >& omhelp.$ext.log"
if !  $cmd >& ../omhelp.$ext.log
then
	cat ../omhelp.$ext.log
	echo "OMhelp could not build doc/*.$ext documentation."
	grep "^OMhelp Error:" ../omhelp.$ext.log
	exit 1
fi
if grep "^OMhelp Warning:" ../omhelp.$ext.log
then
	echo "See the complete warning messages in omhelp.$ext.log."
	exit 1
fi
if [ "$printable" == 'yes' ]
then
	root_name='_printable'
else
	root_name='cppad'
fi
if [ ! -e "$root_name.$ext" ]
then
	echo "run_omhelp.sh: Can't make $root_name.$ext the default page."
	exit 1
fi
if [ -e 'index.htm' ]
then
	echo_eval rm index.htm
fi
cat << EOF > index.html
<html><head><script>
window.location.href="$root_name.$ext";
</script></head></html>
EOF
#
echo "OK: omhelp $*"
exit 0
