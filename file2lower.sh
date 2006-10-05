#! /bin/bash
#
directory="omh"
extension="omh"
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
for old in $directory/*.$extension
do
	new=`echo "$old" | sed \
		-e "s/\([a-z]\)\([A-Z]\)\([a-z]\)/\1_\2\3/g" \
		-e "s/\([a-z]\)\([0-9]\)/\1_\2/g" | tr [A-Z] [a-z]
	`
	echo "mv $old $new" >> file2lower.mv
	#
	old_name=`echo $old | sed -e "s|.*/||" -e "s|\.$extension||"`
	new_name=`echo $new | sed -e "s|.*/||" -e "s|\.$extension||"`
	d='$'
	echo "s|$d\(.\)ref\\(.\\)$old_name|$d\1ref\\2$new_name|g" >> file2lower.sed
done
#
# list of directories to edit with the sed script
dir_list="
	.
	Adolc
	CppAD
	CppAD/local
	Example
	Fadbad
	GetStarted
	Introduction
	omh
	PrintFor
	Speed
	SpeedExample
	TestMore
"
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
				echo $file
				echo "$file"                 >> file2lower.out
				diff $file file2lower.tmp    >> file2lower.out
			fi
		done
	done
done
