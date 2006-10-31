#! /bin/bash
# file that moves a directory to a lower case name version
old="SpeedExample"
new="speed_example"
#
svn move $old $new
list="
	Build.sh
	CheckIncludeFile.sh
	file2lower.sh
	Makefile.am
	configure.ac
	$new/*
"
dollar='$'
for file in $list 
do
	echo $file
	svn revert $file
	sed < $file > dir2lower.tmp \
		-e "s|$old\([\/]\)|$new\1|g" \
		-e "s|\([= 	\/][= 	\/]*\)$old[ 	]*$|\1$new|"
	diff $file dir2lower.tmp
	mv dir2lower.tmp $file
	ext=`echo $file | sed -e 's|.*\.||'`
	if [ "$ext" = "sh" ]
	then
		chmod +x $file
	fi
done
