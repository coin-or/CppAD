#! /bin/bash
if [ "$1" == "" ]
then
	echo "Search CppAD source code for multiple text strings."
	echo "usage: grep_source.sh string_1 string_2 ..."
	exit 1
fi
if [ -e grep_source.tmp ]
then
	rm grep_source.tmp
fi
dir_list="
	.
	cppad
	cppad/local
	cppad/speed
	example
	introduction
	introduction/get_started
	introduction/exp_apx
	omh
	print_for
	speed/adolc
	speed/cppad
	speed/example
	speed/fadbad
	speed/profile
	test_more
"
for dir in $dir_list
do
	for string in $*
	do
		grep -s "$string" \
			$dir/*.hpp $dir/*.cpp $dir/*.omh $dir/*.sh  \
			>> grep_source.tmp
	done
done
sed -e 's/:.*//' < grep_source.tmp | sort -u
