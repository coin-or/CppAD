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
cat << EOF > bin/commit.$$
This is a template file for making commits to the cppad repository.
Lines with no 'at' characters, are general comments not connected to 
a specifi file. Lines containing an 'at' character are "file name" 
followed by comment. Next line must be empty for commit.sh files to work.

bin/commit.sh@ For this example, bin/commit.sh is the only file committed.
EOF
# -----------------------------------------------------------------------------
if [ $0 != "bin/commit.sh" ]
then
	echo "bin/commit.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------
if [ "$1" == 'files' ]
then
	# -------------------------------------------------
	svn status | sed -n -e '/^[ADMRC] /p' | \
	sed -e 's/^[ADMRC] [+ ]*//' -e '/^bin\/commit.sh$/d' |\
	sort -u >> list.$$
	# -------------------------------------------------
	abort="no"
	list=`cat list.$$`
	for file in $list
	do
		if [ -f "$file" ]
		then
			sed -f svn_commit.sed $file > bin/commit.$$
			if ! diff $file bin/commit.$$ > /dev/null
			then
				echo "---------------------------------------"
				echo "bin/commit.sh: suggest changes to $file:"
				if diff $file bin/commit.$$
				then
					echo "bin/commit.sh: program error"
					rm bin/commit.$$
					exit 1
				fi
				rm bin/commit.$$
				if [ "$file" != "cppad/config.h" ]
				then
					abort="yes"
				fi
			fi
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
	sed list.$$ -e 's/$/@/'                             >> bin/commit.sh
	rm  list.$$
	sed -n -e '/^EOF/,$p' bin/commit.sh.old             >> bin/commit.sh
	#
	echo "diff bin/commit.sh.old bin/commit.sh"
	if diff    bin/commit.sh.old bin/commit.sh
	then
		echo "bin/commit.sh: exiting because bin/commit.sh has not changed"
		exit 1
	fi
	#
	echo "chmod +x bin/commit.sh"
	chmod +x bin/commit.sh
	#
	exit 0
fi
# -----------------------------------------------------------------------
if [ "$1" != 'run' ]
then
cat << EOF
usage: bin/commit.sh files
       bin/commit.sh run

The first from puts a list, at the beginning of bin/commit.sh, of the files
the have changed (according to svn). In addition, it displays the changes 
to bin/commit.sh. This will include the new files in the list since the last 
edit of bin/commit.sh. You should then edit bin/commit.sh by hand, to add 
comments about the changes, before running the second form.

The second form actually commits the list of files (provided that you reply
y to the [y/n] prompt that bin/commit.sh generates).
EOF
	rm bin/commit.$$
	exit 0
fi
# -----------------------------------------------------------------------
list=`sed -e '/@/! d' -e 's/@.*//' bin/commit.$$`
msg=`sed -e '/@ *$/d' -e 's|.*/\([^/]*@\)|\1|' -e 's|@|:|' bin/commit.$$`
# if cppad/config.h is in the list of files to be commited
if (echo $list | grep '^cppad/config.h@' bin/commit.$$ > /dev/null)
then
	# and CPPAD_CPPADVECTOR is not defined as one
	if ! grep '^# *define  *CPPAD_CPPADVECTOR  *1 *$' cppad/config.h \
		>  /dev/null
	then
		echo "bin/commit.sh run: CPPAD_CPPADVECTOR is not 1 in cppad/config.h"
		rm bin/commit.$$
		exit 1
	fi
fi
rm bin/commit.$$
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
