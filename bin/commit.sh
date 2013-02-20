#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# INSTRUCTIONS:
# Directly below is where you specify which files are commited and make
# comments about the commit.  Lines with no '@' characters, are general 
# comments not connected to a specific file. Lines containing an '@' character 
# are "file name" followed by a specific comment for that file. 
cat << EOF > bin/commit.user.$$
General comments about this commit go here (delete this line).

dir/file.ext@ specific comment about this file (delete this line).
EOF
# -----------------------------------------------------------------------------
if [ ! -e "bin/commit.sh" ]
then
	echo "bin/commit.sh: must be executed from its parent directory"
	rm bin/commit.*.$$
	exit 1
fi
# -----------------------------------------------------------------------
if [ "$1" != 'list' ] && [ "$1" != 'edit' ] && [ "$1" != 'run' ]
then
cat << EOF
usage: bin/commit.sh list
       bin/commit.sh edit
       bin/commit.sh run

list:
output a list of the files that have changes svn knows about.

edit:
Edit the list of files at the top of bin/commit.sh to be the same as 
	bin/commit.sh list
would output.  In addition, display the changes to bin/commit.sh. This 
will include the new files in the list since the last edit of bin/commit.sh. 
You should then by hand edit bin/commit.sh, to add comments about the changes.
See INSTRUCTIONS, at the beginning of this file:

run:
commits changes to the list of files in bin/commit.sh 
(provided that you reply y to the [y/n] prompt that bin/commit.sh generates).
The files bin/commit.sh and bin/commit.sed cannot be commited this way; use
	svn commit -m "your log message" bin/commit.sh bin/commit.sed
for commits to these files.
EOF
	rm bin/commit.*.$$
	exit 1
fi
# -----------------------------------------------------------------------
for file  in bin/commit.sed
do
	abort=`svn status $file`
	if [ "$abort" != "" ]
	then
		echo "svn status $file"
		svn status $file
		echo "bin/commit.sh: aborting because $file changed"
		rm bin/commit.*.$$
		exit 1
	fi
done
# -----------------------------------------------------------------------
# check for abort do to unknown files
unknown=`svn status | sed -n -e '/^[?]/p' | \
	sed -e 's/^[?]//' -e "/bin\/commit.*.$$/d"`
msg="aborting because the following files are unknown to svn"
print_msg="no"
for file in $unknown
do
	if [ ! -e "$file.in" ]
	then
		if [ "$print_msg" == "no" ]
		then
			echo "bin/commit.sh: $msg"
			print_msg="yes"
		fi
		echo $file
	fi
done
if [ "$print_msg" == "yes" ]
then
	rm bin/commit.*.$$
	exit 1
fi
# -------------------------------------------------
# list of files that changed
svn status | sed -n -e '/^[ADMRC][ADMRC]* /p' -e '/^ [ADMRC] /p' | \
	sed -e 's/^[ADMRC ][ADMRC ][+ ]*//' \
		-e '/^bin\/commit.sh$/d' -e '/^bin\/commit.sed$/d' | \
	sort -u > bin/commit.list.$$
# -------------------------------------------------
if [ "$1" == 'list' ]
then
	cat bin/commit.list.$$
	rm bin/commit.*.$$
	exit 0
fi
# -------------------------------------------------
if [ "$1" == 'edit' ]
then
	abort="no"
	list=`cat bin/commit.list.$$`
	for file in $list
	do
		# exclude */makefile.in from edits in bin/commit.sed
		local_file=`echo $file | sed -e 's|.*/||'`
		file_ext=`echo $file | sed -e 's|.*\.||'`
		if [ -f "$file" ] &&  [ "$local_file" != "makefile.in" ]
		then
			sed -f bin/commit.sed $file > bin/commit.tmp.$$
			if ! diff $file bin/commit.tmp.$$ > /dev/null
			then
				echo "---------------------------------------"
				echo "bin/commit.sh: automatic changes to $file:"
				if diff $file bin/commit.tmp.$$
				then
					echo "bin/commit.sh: program error"
					rm bin/commit.*.$$
					exit 1
				fi
				abort="yes"
				if [ -x $file ]
				then
					mv bin/commit.tmp.$$ $file
					chmod +x $file
				else
					mv bin/commit.tmp.$$ $file
				fi
			fi
		fi
	done
	if [ "$abort" == "yes" ]
	then
		echo "bin/commit.sh: aborting because of automatic edits above."
		echo "If these edits are ok, rerun bin/commit.sh edit" 
		rm bin/commit.*.$$
		exit 1
	fi
	#
	# use backslash so this instruction does not edit itself
	echo 'cp bin/commit.sh bin/commit.sh.old'
	cp bin/commit.sh bin/commit.sh.old
	#
	echo "creating new bin/commit.sh"
	for file in $list
	do
		if ! grep "^$file@" bin/commit.sh > /dev/null
		then
			sed \
			-e "/^# INSTRUCTIONS/,/^EOF/s|^EOF|$file@\n&|" \
			-i bin/commit.sh
		fi
	done
	#
	# remove files that are no longer different
	list=`sed -e '/@/! d' -e 's/@.*//' bin/commit.user.$$`
	for file in $list
	do
		if ! grep "^$file\$" bin/commit.list.$$ > /dev/null
		then
			file=`echo $file | sed -e 's|/|\\\\/|g'`
			sed -e "/^$file@/d" -i bin/commit.sh
		fi
	done
	#
	echo "------------------------------------"
	echo "diff bin/commit.sh.old bin/commit.sh"
	if diff    bin/commit.sh.old bin/commit.sh
	then
		echo "bin/commit.sh edit: no changes to bin/commit.sh"
	fi
	echo "------------------------------------"
     echo "chmod +x bin/commit.sh"
           chmod +x bin/commit.sh
	#
	rm bin/commit.*.$$
	exit 0
fi
# -----------------------------------------------------------------------
if [ "$1" != 'run' ]
then
	echo "bin/commit.sh: program error"
	exit 1
fi
list=`sed -e '/@/! d' -e 's/@.*//' bin/commit.user.$$`
msg=`sed -e '/@ *$/d' -e 's|.*/\([^/]*@\)|\1|' -e 's|@|:|' bin/commit.user.$$`
#
if (echo $list | grep 'bin/commit.sh$' > /dev/null)
then
	echo "bin/commit.sh: cannot be used to commit changes to itself."
	echo "remove it from the list of files in bin/commit.sh"
	rm bin/commit.*.$$
	exit 1
fi
#
if (echo $list | grep 'bin/commit.sed$' > /dev/null)
then
	echo "bin/commit.sh: cannot be used to commit changes to bin/commit.sed"
	echo "remove it from the list of files in bin/commit.sh"
	rm bin/commit.*.$$
	exit 1
fi
#
rm bin/commit.*.$$
#
echo "svn commit -m \""
echo "$msg"
echo "\" \\"
echo "$list"
read -p "is this ok [y/n] ?" response
if [ "$response" != "y" ]
then
	exit 1
fi
for file in $list
do
	if [ -f $file ]
	then
		echo "svn propset svn:keywords \"Id\" $file"
		svn propset svn:keywords "Id" $file
	fi
done
#
if ! svn commit -m "$msg" $list
then
	echo "bin/commit.sh: commit failed"
	exit 1
fi
#
echo "mv bin/commit.sh commit.sh.old"
mv       bin/commit.sh commit.sh.old
#
echo "svn revert bin/commit.sh"
svn revert bin/commit.sh
#
exit 0
