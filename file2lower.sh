#! /bin/bash
#
if [ "$1" != "test" ] && [ "$1" != "local" ] && [ "$1" != "svn" ]
then
	echo "file2lower.sh: (test|local|svn)"
	exit 1
fi
mode="$1"
directory="CppAD"
extension="h"
dir_list="
	.
	adolc
	CppAD
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
#
# map a set of file names to lower case
if [ -e file2lower.mv ]
then
	rm file2lower.mv
fi
if [ -e file2lower.sed ]
then
	rm file2lower.sed
fi
if [ -e file2lower.out ]
then
	rm file2lower.out
fi
echo "file2lower.sh: creating file2lower.sed and file2lower.mv"
echo "#! /bin/bash"                                           >> file2lower.mv
echo "# move $director/*.$ext file names to lower case names" >> file2lower.mv
for old in $directory/*.$extension
do
if [ $old != "cppad/config.h" ]
then
	new=`echo "$old" | sed \
		-e "s/AD/Ad/g" \
		-e "s/\([a-z]\)\([A-Z]\)\([a-z]\)/\1_\2\3/" \
		-e "s/\([a-z]\)\([A-Z]\)\([a-z]\)/\1_\2\3/" \
		-e "s/\([a-z]\)\([A-Z]\)\([a-z]\)/\1_\2\3/" \
		-e "s/\([a-z]\)\([0-9]\)/\1_\2/g" \
		-e "s|/\([A-z][a-z]*\)\.|/\1_.|" | tr [A-Z] [a-z]
	`
	new=`echo "$new" | sed -e "s|cpp_ad/|cppad/|" -e "s/cpp_ad/cppad/g"`
	if [ "$extension" = "h" ]
	then
		new=`echo $new | sed -e "s/_*\.h/.hpp/"`
	fi
	# avoid ifndef conflict with cppad/local/cppad_vector.hpp
	if [ "$new" = cppad/cppad_vector.hpp ]
	then
		new="cppad/vector.hpp"
	fi
	#
	if [ "$mode" == "svn" ]
	then
		echo "svn copy $old $new" >> file2lower.mv
	else
		echo "mv $old $new"       >> file2lower.mv
	fi
	# split old file name from directory and extension
	ext=`echo $old | sed -e "s|.*\.||"`
	old=`echo $old | sed -e "s|.*/||" -e "s|\..*||"`
	#
	D="$directory"
	echo "s@$D[\\/]$old\\.$ext@$new@g"  >> file2lower.sed
	#
	# remove directory from new
	new=`echo $new | sed -e "s|.*/||"`
	#
	echo "s@\\([ 	\"]\\)$old\\.$ext@\\1$new@g"  >> file2lower.sed
	#
	echo "s@\$begin *$new@\$begin $old.$ext@"   >> file2lower.sed
	echo "s@ref\\(.\\)$new@ref\\1$old.$ext@"    >> file2lower.sed
	#
	# new in upper case and without extension
	new_up=`echo $new | sed -e "s|\..*||" | tr [a-z] [A-Z]`
	#
	echo "s@CppAD${old}Included@CPPAD_${new_up}_INCLUDED@" \
		>> file2lower.sed
	#
	if [ "$directory" = "CppAD" ] && [ "$extension" = "h" ]
	then
echo "s@nobase_myinclude_HEADERS = *\\\\@&\n\tCppAD/$old.h \\\\@" \
	>> file2lower.sed
echo "echo \"# include \\\"cppad/$new\\\"\" > cppad/$old.h" \
	>> file2lower.mv
	fi
fi
done
chmod +x file2lower.mv
if [ "$mode" = "test" ]
then 
	echo "file2lower.sh: NOT executing ./file2lower.mv"
else
	echo "./file2lower.mv"
	./file2lower.mv
fi
# list of extensions to edit with the sed script
ext_list="
	sh
	am
	omh
	cpp
	h
	hpp
"
echo "file2lower.sh: creating file2lower.out"
for dir in $dir_list
do
	for ext in $ext_list
	do
		list=`ls $dir/*.$ext 2> /dev/null`
		for file in $list
		do
			sed -f file2lower.sed < $file > file2lower.tmp
			if ! diff $file file2lower.tmp > /dev/null
			then
				echo "$file"                 >> file2lower.out
				diff $file file2lower.tmp    >> file2lower.out
if [ $mode = "test" ]
then 
	echo "file2lower.sh: NOT changing $file"
else
	echo "file2lower.sh: changing $file"
	mv file2lower.tmp $file
	if [ "$ext" = "sh" ]
	then
		chmod +x $file
	fi
fi
			fi
		done
	done
done
if [ "$extension" = "h" ]
then
	list=`ls $directory/*.h $directory/*.hpp 2> /dev/null`
	list="
		$list
		doc.omh
		get_started/get_started.cpp
		introduction/exp_apx_ad.cpp
		print_for/print_for.cpp
		cppad/local/bender_quad.hpp
		cppad/local/lu_ratio.hpp
		omh/whats_new_05.omh
		omh/whats_new_04.omh
		omh/whats_new_03.omh
		speed_example/speed_example.cpp
		example/det_of_minor.hpp
	"
	for file in $list
	do
		sed -e 's/$spell/&\n\tcppad.hpp/' < $file > file2lower.tmp
		if ! diff $file file2lower.tmp > /dev/null
		then
			echo "$file"                 >> file2lower.out
			diff $file file2lower.tmp    >> file2lower.out
			if [ $mode = "test" ]
			then 
				echo "file2lower.sh: NOT changing $file"
			else
				echo "file2lower.sh: changing $file"
				mv file2lower.tmp $file
			fi
		fi
	done
fi
