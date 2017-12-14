#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
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
clean='no'
htm='no'
xml='no'
printable='no'
while [ "$1" != '' ]
do
	case "$1" in

		clean)
		clean='yes'
		;;

		htm)
		htm='yes'
		;;

		xml)
		xml='yes'
		;;

		printable)
		printable='yes'
		;;

		*)
		echo "$1 is not a valid bin/run_omhelp.sh option"
		exit 1
	esac
	shift
done
if [ "$htm" == "$xml" ]
then
	echo 'usage: bin/run_omhelp.sh [clean] [htm] [xml] [printable]'
	echo 'order does not matter and htm or xml is present (but not both).'
	exit 1
fi
if [ "$htm" == 'yes' ]
then
	ext='htm'
else
	ext='xml'
fi
# -----------------------------------------------------------------------------
if [ "$clean" == 'yes' ]
then
	echo_eval rm -rf doc
fi
#
if [ ! -e doc ]
then
	echo_eval mkdir doc
fi
echo_eval cd doc
# -----------------------------------------------------------------------------
cmd='omhelp ../doc.omh -noframe -debug'
cmd="$cmd -image_link http://www.coin-or.org/CppAD/"
#
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
