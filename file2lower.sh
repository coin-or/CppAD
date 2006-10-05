#! /bin/bash
#
if [ "$1" == "go-for-it" ]
then
	test_mode="no"
else
	test_mode="yes"
fi
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
echo "#! /bin/bash"                                           >> file2lower.mv
echo "# move $director/*.$ext file names to lower case names" >> file2lower.mv
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
chmod +x file2lower.mv
if [ $test_mode = "yes" ]
then 
	echo "file2lower.sh: NOT executing ./file2lower.mv"
else
	echo "file2lower.sh: YES executing ./file2lower.mv"
fi
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
				echo "$file"                 >> file2lower.out
				diff $file file2lower.tmp    >> file2lower.out
if [ $test_mode = "yes" ]
then 
	echo "file2lower.sh: NOT changing $file"
else
	echo "file2lower.sh: changing $file"
	mv file2lower.tmp $file
fi
			fi
		done
	done
done
