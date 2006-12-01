#! /bin/bash
# file that moves a directory to a lower case name version
old="example_"
new="example"
list="
	Build.sh
	CheckIncludeFile.sh
	file2lower.sh
	makefile.am
	configure.ac
	fadbad/*
	adolc/*
	speed/*
	omh/*
	CppAD/*
	CppAD/local/*
	$new/*
"
#
if [ ! -e $new ]
then
	svn move $old $new
fi
if [ -e dir2lower.out ]
then
	rm dir2lower.out
fi
dollar='$'
for file in $list 
do
if [ ! -d "$file" ] && [ "$file" != "$new/makefile.am" ]
then
	ext=`echo $file | sed -e 's|.*\.||'`
	if [ "$ext" != "sln" ] && [ "$ext" != "vcproj" ]
	then
		echo $file >> dir2lower.out
		svn revert $file
		sed < $file > dir2lower.tmp \
			-e "s|\([^-]\)$old\([\/]\)|\1$new\2|g" \
			-e "s|^$old\([\/]\)|$new\1|g" \
			-e "s|\([= 	\/][= 	\/]*\)$old[ 	]*$|\1$new|"
		diff $file dir2lower.tmp >> dir2lower.out 
		mv dir2lower.tmp $file
		if [ "$ext" = "sh" ]
		then
			chmod +x $file
		fi
	fi
fi
done
