#! /bin/bash
# file that moves a directory to a lower case name version
list="
	Build.sh
	CheckIncludeFile.sh
	file2lower.sh
	Makefile.am
	configure.ac
	omh/fadbad.omh
	fadbad/*.cpp
"
dollar='$'
for file in $list 
do
	echo $file
	svn revert $file
	sed < $file > dir2lower.tmp \
		-e 's|fadbad_\([\/]\)|fadbad\1|g' \
		-e 's|\([= 	\/][= 	\/]*\)fadbad_[ 	]*$|\1fadbad|'
	diff $file dir2lower.tmp
	mv dir2lower.tmp $file
	ext=`echo $file | sed -e 's|.*\.||'`
	if [ "$ext" = "sh" ]
	then
		chmod +x $file
	fi
done
