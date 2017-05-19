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
if [ $0 != "bin/special_version.sh" ]
then
	echo "bin/special_version.sh: must be executed from its parent directory"
	exit 1
fi
# ----------------------------------------------------------------------------
version="$1"
case "$version" in
	2011 | 2012_0 | 2012_1 )
	subversion_revision='2279'
	;;

	2012_2 )
	subversion_revision='2347'
	;;

	*)
cat << EOF
	usage: bin/special_version.sh version
	where version is one of the following: 2011, 2012_0 2012_1 2012_2
EOF
	exit 1
	;;
esac
url='https://projects.coin-or.org/svn/CppAD/trunk'
# ----------------------------------------------------------------------------
if [ -e "../special-$version" ]
then
	echo "Using the existing directory ../special-$version"
	if [ -e ../special-$version/work ]
	then
		echo "rm -r ../special-$version/work"
		rm -r ../special-$version/work
	fi
else
	echo "svn checkout -r $subversion_revision $url ../special-$version"
	svn checkout -r $subversion_revision $url ../special-$version
fi
echo "cd ../special-$version"
cd ../special-$version
#
case "$1" in
	2011)
	list="bthread openmp pthread"
	for threading in $list
	do
		file="example/multi_thread/$threading/team_$threading.cpp"
		svn revert $file
		#
		echo "sed -e 's|hold_memory(true)|hold_memory(false)|;' -i $file"
		sed -e 's|hold_memory(true)|hold_memory(false)|;' -i $file
	done
	;;

	2012_0)
	file='cppad/thread_alloc.hpp'
	thread_alloc_revision='2249'
	echo "svn cat $url/$file@$thread_alloc_revision > $file"
	svn cat $url/$file@$thread_alloc_revision > $file
	#
	# Always hold onto memory
	echo "sed -e 's/if( num_threads() == 1 )/if(false)/' -i $file"
	sed -e 's/if( num_threads() == 1 )/if(false)/' -i $file
	#
	# No hold_memory function
	echo "sed -e 's|^.*void parallel_|\tstatic void hold_memory(bool) {}\n&|'"
	echo "	-i $file"
	sed -e 's|^.*void parallel_|\tstatic void hold_memory(bool) {}\n&|' \
		-i $file
	;;

	2012_1 | 2012_2 )
	;;

	*)
	echo "bin/special_version.sh: program error"
	exit 1
	;;
esac
#
file='cppad/configure.hpp'
text="# define CPPAD_PACKAGE_STRING"
svn revert $file
echo "sed -i $file -e \"s|^$text.*|\1 \"cppad-$version\"|\""
sed -i $file -e "s|^\($text\) .*|\1 \"cppad-$version\"|"
#
echo "mkdir work ; cd work"
mkdir work ; cd work
#
response='n'
while [ "$response" != 'y' ]
do
	dir=`pwd`
	echo "current working: $dir"
	echo "Enter configure options on next line (no options is default)"
	read -p "options: " options
	echo "../configure $options"
	read -p "Is this ok [y/n] ?" response
done
echo "../configure $options"
../configure $options
#
echo "cd multi_thread ; make test"
cd multi_thread ; make test
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
