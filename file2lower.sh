#! /bin/bash
#
if [ "$1" != "test" ] && [ "$1" != "local" ] && [ "$1" != "svn" ]
then
	echo "file2lower.sh: (test|local|svn)"
	exit 1
fi
mode="$1"
directory="example"
extension="cpp"
dir_list="
	.
	fadbad
	adolc
	speed
	omh
	CppAD
	CppAD/local
	example
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
	new=`echo "$old" | sed \
		-e "s/AD/Ad/g" \
		-e "s/\([a-z]\)\([A-Z]\)\([a-z]\)/\1_\2\3/" \
		-e "s/\([a-z]\)\([A-Z]\)\([a-z]\)/\1_\2\3/" \
		-e "s/\([a-z]\)\([A-Z]\)\([a-z]\)/\1_\2\3/" \
		-e "s/\([a-z]\)\([0-9]\)/\1_\2/g" \
		-e "s|/\([A-z][a-z]*\)\.|/\1_.|" | tr [A-Z] [a-z]
	`
	if [ "$extension" = "h" ]
	then
		new=`echo $new | sed -e "s/_*\.h/.hpp/"`
	fi
	if [ "$mode" == "svn" ]
	then
		echo "svn move $old $new" >> file2lower.mv
	else
		echo "mv $old $new"       >> file2lower.mv
	fi
	old=`echo $old | sed -e 's|/|[\\\\/]|'`
	echo "s@$old@$new@g"  >> file2lower.sed
	#
	old=`echo $old | sed -e 's|.*\]||'`
	new=`echo $new | sed -e 's|.*/||'`
	echo "s@\\([ 	\"]\\)$old@\\1$new@g"  >> file2lower.sed
	#
	echo "s@\$begin *$new@\$begin $old@"   >> file2lower.sed
	echo "s@ref\\(.\\)$new@ref\\1$old@"    >> file2lower.sed
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
