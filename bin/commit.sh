#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# replacement text for this commit
cat << EOF > bin/commit.1.$$
This is a template file for making commits to the CppAD repository.
Lines with no 'at' characters, are general comments not connected to 
a specific file. Lines containing an 'at' character are "file name" 
followed by comment. Lines before the first 'at' character are preserved
during
	bin/commit.sh edit 
for example this entire paragraph is preserved.

bin/commit.sh@ For this example, bin/commit.sh is the only file committed.
EOF
# -----------------------------------------------------------------------------
if [ $0 != "bin/commit.sh" ]
then
	echo "bin/commit.sh: must be executed from its parent directory"
	rm bin/commit.1.$$
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
Edit the file list of files at the top of bin/commit.sh to be the same as 
	bin/commit.sh list
would output.  In addition, it displays the changes to bin/commit.sh. This 
will include the new files in the list since the last edit of bin/commit.sh. 
You should then edit bin/commit.sh by hand, to add comments about the changes, 
(and remove bin/commit.sh from the list) before running the command
	bin/commit.sh run

run:
commits changes to the list of files in bin/commit.sh 
(provided that you reply y to the [y/n] prompt that bin/commit.sh generates).
The files bin/commit.sh and bin/commit.sed cannot be commited this way; use
	svn commit -m "your log message" bin/commit.sh bin/commit.sed
for commits to these files.
EOF
	rm bin/commit.1.$$
	exit 1
fi
# -----------------------------------------------------------------------
if [ "$1" == 'list' ] || [ "$1" == 'edit' ]
then
	# -------------------------------------------------
	svn status | sed -n -e '/^[ADMRC] /p' | \
		sed -e 's/^[ADMRC] [+ ]*//' \
			-e '/^bin\/commit.sh$/d' -e '/^bin\/commit.sed$/d' | \
		sort -u > bin/commit.1.$$
	# -------------------------------------------------
	if [ "$1" == 'list' ]
	then
		cat bin/commit.1.$$
		rm bin/commit.1.$$
		exit 0
	fi
	abort="no"
	list=`cat bin/commit.1.$$`
	for file in $list
	do
		if [ -f "$file" ]
		then
			sed -f bin/commit.sed $file > bin/commit.2.$$
			if ! diff $file bin/commit.2.$$ > /dev/null
			then
				echo "---------------------------------------"
				echo "bin/commit.sh: suggest changes to $file:"
				if diff $file bin/commit.2.$$
				then
					echo "bin/commit.sh: program error"
					rm bin/commit.2.$$
					exit 1
				fi
				if [ "$file" != "cppad/config.h" ]
				then
					abort="yes"
				fi
			fi
			rm bin/commit.2.$$
		fi
	done
	if [ "$abort" == "yes" ]
	then
		echo "bin/commit.sh: aborting because of suggested changes above"
		echo "with exception of changes to cppad/config.h"
		exit 1
	fi
	#
	echo "mv bin/commit.sh bin/commit.sh.old"
	mv bin/commit.sh bin/commit.sh.old
	#
	echo "creating new bin/commit.sh"
	sed -n -e '1,/@/p' bin/commit.sh.old | sed -e '/@/d' > bin/commit.sh
	sed bin/commit.1.$$ -e 's/$/@/'                     >> bin/commit.sh
	sed -n -e '/^EOF/,$p' bin/commit.sh.old             >> bin/commit.sh
	rm  bin/commit.1.$$
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
	exit 0
fi
# -----------------------------------------------------------------------
list=`sed -e '/@/! d' -e 's/@.*//' bin/commit.1.$$`
msg=`sed -e '/@ *$/d' -e 's|.*/\([^/]*@\)|\1|' -e 's|@|:|' bin/commit.1.$$`
# if cppad/config.h is in the list of files to be commited
if (echo $list | grep '^bin/commit.sh$' > /dev/null)
then
	echo "bin/commit.sh: cannot be used to commit changes to itself."
	echo "remove it from the list of files in bin/commit.sh"
	exit 1
fi
if (echo $list | grep '^bin/commit.sed$' > /dev/null)
then
	echo "bin/commit.sh: cannot be used to commit changes to bin/commit.sed"
	echo "remove it from the list of files in bin/commit.sh"
	exit 1
fi
if (echo $list | grep '^cppad/config.h$' bin/commit.1.$$ > /dev/null)
then
	# and CPPAD_CPPADVECTOR is not defined as one
	if ! grep '^# *define  *CPPAD_CPPADVECTOR  *1 *$' cppad/config.h \
		>  /dev/null
	then
		echo "bin/commit.sh run: CPPAD_CPPADVECTOR is 1 in cppad/config.h"
		rm bin/commit.1.$$
		exit 1
	fi
	if ! grep '^# *define  *CPPAD_BOOSTVECTOR  *0 *$' cppad/config.h \
		>  /dev/null
	then
		echo "bin/commit.sh run: CPPAD_BOOSTVECTOR not 0 in cppad/config.h"
		rm bin/commit.1.$$
		exit 1
	fi
fi
rm bin/commit.1.$$
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
