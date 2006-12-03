#! /bin/bash
#
mv CppAD cppad_
mv cppad_ cppad
dir_list="
	.
	adolc
	cppad
	cppad/local
	example
	fadbad
	get_started
	introduction
	omh
	print_for
	speed_cppad
	speed_example
	test_more
"
ext_list="
	ac
	sh
	am
	omh
	cpp
	h
	hpp
"
if [ -e junk.out ]
then
	rm junk.out
fi
echo "creating junk.out"
for dir in $dir_list
do
	for ext in $ext_list
	do
		list=`ls $dir/*.$ext 2> /dev/null`
		for file in $list
		do
		if [ "$file" != "./cppad_2_lower.sh" ]
		then
			sed < $file > junk.tmp \
				-e 's|\([^_a-zA-Z:]\)CppAD/|\1cppad/|g' \
				-e 's|/CppAD\([^_a-zA-Z:]\)|/cppad\1|g' \
				-e 's|^CppAD/|cppad/|'  \
				-e 's|/CppAD$|/cppad|'  \
				-e 's|coin-or.org/cppad|coin-or.org/CppAD|g' \
				-e 's|cppad\.h|CppAD\.h|g' \
				-e 's|CppAD\.hpp|cppad.hpp|'
			if ! diff $file junk.tmp > /dev/null
			then
				echo "$file"           >> junk.out
				diff $file junk.tmp    >> junk.out
				mv junk.tmp $file
				if [ "$ext" = "sh" ]
				then
					chmod +x $file
				fi
			fi
		fi
		done
	done
done
#
list="
	cppad/local/define.hpp
	cppad/local/vec_ad.hpp
	cppad/local/cppad_vector.hpp
	omh/adolc.omh
	omh/fadbad.omh
"
for file in $list
do
	sed -e 's/$spell/&\n\tcppad.hpp/' < $file > junk.tmp
	if ! diff $file junk.tmp > /dev/null
	then
		echo "$file"           >> junk.out
		diff $file junk.tmp    >> junk.out
		echo "cppad_2_lower.sh: changing $file"
		mv junk.tmp $file
	fi
done
#
file="omh/install_subversion.omh"
sed -e 's/$spell/&\n\CppAD/' < $file > junk.tmp
echo "$file"           >> junk.out
diff $file junk.tmp    >> junk.out
echo "CppAD2lower.sh: changing $file"
mv junk.tmp $file
