#! /bin/bash -e
#
# replacement text for this commit
cat << EOF > commit.$$
This is a template file for making commits to the cppad repository.
Lines with no 'at' characters, are general comments not connected to 
a specifi file. Lines containing an 'at' character are "file name" 
followed by comment. Next line must be empty for ./commit.sh files to work.

commit.sh@ For this example, commit.sh is the only file committed.
EOF
# -----------------------------------------------------------------------
if [ "$1" == 'files' ]
then
	# -------------------------------------------------
	svn status | sed -n -e '/^[ADMRC] /p' | \
	sed -e 's/^[ADMRC] [+ ]*//' -e '/^commit.sh$/d' |\
	sort -u >> list.$$
	# -------------------------------------------------
	abort="no"
	list=`cat list.$$`
	for file in $list
	do
		if [ -e "$file" ]
		then
			sed -f svn_commit.sed $file > commit.$$
			if ! diff $file commit.$$ > /dev/null
			then
				echo "---------------------------------------"
				echo "commit.sh: suggest changes to $file:"
				if diff $file commit.$$
				then
					echo "commit.sh: program error"
					exit 1
				fi
				rm commit.$$
				if [ "$file" != "cppad/config.h" ]
				then
					abort="yes"
				fi
			fi
		fi
	done
	if [ "$abort" == "yes" ]
	then
		echo "commit.sh: aborting because of suggested changes above"
		exit 1
	fi
	#
	echo "mv commit.sh commit.sh.old"
	mv commit.sh commit.sh.old
	#
	echo "creating new ./commit.sh"
	sed -n -e '1,/@/p' commit.sh.old | sed -e '/@/d' > commit.sh
	sed list.$$ -e 's/$/@/'                         >> commit.sh
	rm  list.$$
	sed -n -e '/^EOF/,$p' commit.sh.old             >> commit.sh
	#
	echo "diff commit.sh.old commit.sh"
	if diff commit.sh.old commit.sh
	then
		echo "commit.sh: exiting because commit.sh has not changed"
		exit 1
	fi
	#
	echo "chmod +x commit.sh"
	chmod +x commit.sh
	#
	exit 0
fi
# -----------------------------------------------------------------------
if [ "$1" != 'run' ]
then
cat << EOF
usage: ./commit.sh files
       ./commit.sh run

The first from puts a list, at the beginning of commit.sh, of the files
the have changed (according to svn). In addition, it displays the changes 
to commit.sh. This will include the new files in the list since the last 
edit of commit.sh. You should then edit commit.sh by hand, to add comments
about the changes, before running the second form.

The second form actually commits the list of files (provided that you reply
y to the [y/n] prompt that commit.sh generates).
EOF
	rm commit.$$
	exit 0
fi
# -----------------------------------------------------------------------
list=`sed -e '/@/! d' -e 's/@.*//' commit.$$`
msg=`sed -e '/@ *$/d' -e 's|.*/\([^/]*@\)|\1|' -e 's|@|:|' commit.$$`
# if cppad/config.h is in the list of files to be commited
if (echo $list | grep '^cppad/config.h@' commit.$$ > /dev/null)
then
	# and CPPAD_CPPADVECTOR is not defined as one
	if ! grep '^# *define  *CPPAD_CPPADVECTOR  *1 *$' cppad/config.h \
		>  /dev/null
	then
		echo "commit.sh run: CPPAD_CPPADVECTOR is not 1 in cppad/config.h"
		exit 1
	fi
fi
rm commit.$$
echo "svn commit -m \""
echo "$msg"
echo "\" \\"
echo "$list"
read -p "is this ok [y/n] ?" response
if [ "$response" != "y" ]
then
	exit 1
fi
#
if ! svn commit -m "$msg" $list
then
	echo "commit.sh: commit failed"
	exit 1
fi
#
echo "mv commit.sh commit.old"
mv commit.sh commit.old
#
svn revert commit.sh
