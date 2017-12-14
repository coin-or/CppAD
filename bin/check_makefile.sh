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
if [ ! -e "bin/check_makefile.sh" ]
then
	echo "bin/check_makefile.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
echo 'Checking if any makfile.in has changed'
bin/autotools.sh automake >& /dev/null
list=`git ls-files '*/makefile.in'`
ok='yes'
for file in $list
do
	diff=`git diff -- $file`
	if [ "$diff" != '' ]
	then
		echo "git add $file"
		git add $file
		ok='no'
	fi
done
if [ "$ok" == 'no' ]
then
	echo '*/makefile.in has changed.'
	exit 1
fi
# -----------------------------------------------------------------------------
echo "Checking include files listed in makefile.am"
echo "-------------------------------------------------------"
bin/ls_files.sh | sed -n -e '/cppad\/.*\.hpp$/p' \
	> check_makefile.1.$$
sort -u check_makefile.1.$$ > check_makefile.2.$$
#
sed < makefile.am -n \
	-e '/^nobase_myinclude_HEADERS *=/,/^# End nobase_myinclude_HEADERS/p' | \
	sed \
		-e '/nobase_myinclude_HEADERS/d' \
		-e 's/^\t//' \
		-e 's/ *\\$//' \
		-e 's/ *$//' \
		-e '/^$/d'  |
	sort > check_makefile.3.$$
#
if diff check_makefile.2.$$ check_makefile.3.$$
then
	ok="yes"
else
	ok="no"
fi
rm check_makefile.*.$$
echo "-------------------------------------------------------"
if [ "$ok" = "no" ]
then
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
fi
# -----------------------------------------------------------------------------
echo "Checking debugging flags in all makefile.am files."
echo "-------------------------------------------------------"
list=`bin/ls_files.sh | sed -n -e '/\/makefile.am$/p'`
ok="yes"
for file in $list
do
	case $file in
		( makefile.am | work/* | svn_dist/* )
		;;

		(example/multi_thread/makefile.am)
		;;

		(speed/example/makefile.am)
		if grep '\-DNDEBUG' $file > /dev/null
		then
			echo "-DNDEBUG flag appears in $file"
			ok="no"
		fi
		if ! grep '\-g' $file > /dev/null
		then
			echo "-g flag does not appear in in $file"
			ok="no"
		fi
		;;

		(test_more/makefile.am)
		sed -e '/ndebug/d' < $file > bin/check_makefile.$$
		if grep '\-DNDEBUG' bin/check_makefile.$$ > /dev/null
		then
			echo "-DNDEBUG flag appears in $file"
			ok="no"
		fi
		if ! grep '\-g' $file > /dev/null
		then
			echo "-g flag does not appear in in $file"
			ok="no"
		fi
		rm bin/check_makefile.$$
		;;

		(speed/[^/]*/makefile.am | cppad_ipopt/speed/makefile.am)
		if ! grep '^[^#]*-DNDEBUG' $file > /dev/null
		then
			echo "Optimization flag is not defined in $file"
			ok="no"
		fi
		if ! grep '^#.*-g' $file > /dev/null
		then
			echo "Debug flag is not commented out in $file"
			ok="no"
		fi
		;;

		(test_more/compare_c/makefile.am)
		if ! grep '^[^#]*-DNDEBUG' $file > /dev/null
		then
			echo "Optimization flag is not defined in $file"
			ok="no"
		fi
		if ! grep '^#.*-g' $file > /dev/null
		then
			echo "Debug flag is not commented out in $file"
			ok="no"
		fi
		;;

		*)
		if grep '\-DNDEBUG' $file > /dev/null
		then
			echo "-DNDEBUG flag appears in $file"
			ok="no"
		fi
		if ! grep '\-g' $file > /dev/null
		then
			echo "-g flag does not appear in in $file"
			ok="no"
		fi
		;;
	esac
done
echo "-------------------------------------------------------"
if [ "$ok" = "yes" ]
then
	echo "Ok: nothing is between the two dashed lines above"
	exit 0
else
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
fi
