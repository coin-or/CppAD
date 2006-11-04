#! /bin/bash
# file that moves a directory to a lower case name version
old="Speed"
new="speed_"
#
if [ ! -e $new ]
then
	svn move $old $new
fi
list="
	Build.sh
	CheckIncludeFile.sh
	file2lower.sh
	Makefile.am
	configure.ac
	omh/install_unix.omh
	omh/install_windows.omh
	omh/speed.omh
	$new/*
"
dollar='$'
for file in $list 
do
if [ ! -d "$file" ] && [ "$file" != "$new/Makefile.am" ]
then
	echo $file
	svn revert $file
	sed < $file > dir2lower.tmp \
		-e "s|\([^-]\)$old\([\/]\)|\1$new\2|g" \
		-e "s|^$old\([\/]\)|$new\1|g" \
		-e "s|\([= 	\/][= 	\/]*\)$old[ 	]*$|\1$new|"
	diff $file dir2lower.tmp
	mv dir2lower.tmp $file
	ext=`echo $file | sed -e 's|.*\.||'`
	if [ "$ext" = "sh" ]
	then
		chmod +x $file
	fi
	if [ "$ext" = "sln" ] || [ "$ext" = "vcproj" ]
	then
		unix2dos $file
	fi
fi
done
