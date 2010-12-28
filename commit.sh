#! /bin/bash -e
#
# replacement text for this commit
cat << EOF > commit.$$
This is a template file for making commits to the ckbs repository.
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
		sed -e 's/^[ADMRC] [+ ]*//' -e '/^commit.sh$/d' | \
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
	echo "cp commit.sh commit.old"
	cp commit.sh commit.old
	#
	echo "svn revert commit.sh"
	svn revert commit.sh
	#
	sed -n -e '1,/^$/p'   < commit.sh >  commit.$$ 
	sed list.$$ -e 's/$/@/'           >> commit.$$ 
	rm  list.$$
	sed -n -e '/^EOF/,$p' < commit.sh >> commit.$$ 
	#
	echo "diff commit.sh commit.$$"
	if diff commit.sh commit.$$
	then
		echo "commit.sh: exiting because commit.sh has not changed"
		rm commit.$$
		exit 1
	fi
	#
	echo "mv commit.$$ commit.sh"
	mv commit.$$ commit.sh
	#
	echo "chmod +x commit.sh"
	chmod +x commit.sh
	#
	sed -f svn_commit.sed cppad/config.h > commit.$$
	if ! diff cppad/config.h commit.$$ > /dev/null
	then
		echo "------------------------------"
		echo "commit.sh: either remove cppad/config.h from list in commit.sh"
		echo "or make the following change to cppad/config.h"
		diff cppad/config.h commit.$$
		exit 1
	fi
	exit 0
fi
# -----------------------------------------------------------------------
if [ "$1" != 'run' ]
then
cat << EOF
usage: ./commit.sh files
       ./commit.sh run

The first from changes the list of files at the beginning of commit.sh 
so that it all the files that have changed status.
You should then edit commit.sh by hand (as per the instructions at the 
beginning of commit.sh) before running the second form.

The second form actually commits the list of files (provided that you reply
y to the [y/n] prompt that commit.sh generates).
EOF
	rm commit.$$
	exit 0
fi
# -----------------------------------------------------------------------
echo "mv cppad/config.h cppad/config.h.old"
mv cppad/config.h cppad/config.h.old
#
echo "sed -f svn_commit.sed cppad/config.h.old > cppad/config.h"
sed -f svn_commit.sed cppad/config.h.old > cppad/config.h
#
if ! diff cppad/config.h.old cppad/config.h > /dev/null
then
	echo "commit.sh: automatically made following changes to cppad/config.h"
	echo "-----------------------------------------------------------------"
	if diff cppad/config.h.old cppad/config.h
	then
		echo "commit.sh: unexpected return from diff"
		exit 1
	fi
	echo "-----------------------------------------------------------------"
	echo "to complete this change, rexecute the command: ./commit.sh run"
	exit 1
fi
list=`sed -e '/@/! d' -e 's/@.*//' commit.$$`
msg=`sed -e '/@ *$/d' -e 's|.*/\([^/]*@\)|\1|' -e 's|@|:|' commit.$$`
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
