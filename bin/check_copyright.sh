#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/check_copyright.sh" ]
then
	echo "bin/check_copyright.sh: must be executed from its parent directory"
	exit 1
fi
if [ ! -e .git ]
then
	echo 'This is not a git repository so cannot check copyright.'
	echo 'check_copyright.sh: skipped'
	exit 0
fi
list=`git status | sed -n \
        -e '/^[#\t ]*deleted:/p' \
        -e '/^[#\t ]*modified:/p' \
        -e '/^[#\t ]*both modified:/p' \
        -e '/^[#\t ]*renamed:/p' \
        -e '/^[#\t ]*new file:/p' | \
            sed -e 's/^.*: *//' -e 's/ -> /\n/' | \
			sed -e '/^makefile.in$/d' \
				-e '/^.gitignore$/d' \
				-e '/^authors$/d' \
				-e '/^readme.md$/d' \
				-e '/^COPYING$/d' \
				-e '/^compile$/d' \
				-e '/^config.guess$/d' \
				-e '/^config.sub$/d' \
				-e '/^configure$/d' \
				-e '/\/check_copyright.sh$/d' \
				-e '/\/makefile.in$/d' |
                sort -u`
cat << EOF > check_copyright.1.$$
# Change copyright second year to current year
s/Copyright (C) \\([0-9]*\\)-[0-9][0-9] Bradley M. Bell/Copyright (C) \\1-17 Bradley M. Bell/
s/Copyright (C) \\([0-9]*\\)-20[0-9][0-9] Bradley M. Bell/Copyright (C) \\1-17 Bradley M. Bell/
EOF
ok=true
for file in $list
do
	if [ -e $file ]
	then
		if ! grep 'Copyright (C) [0-9]*-[0-9][0-9]' $file > /dev/null
		then
			echo "Cannot find copyright message in $file"
			exit 1
		fi
		sed -f check_copyright.1.$$ $file > check_copyright.2.$$
		if ! diff $file check_copyright.2.$$ > /dev/null
		then
			echo '----------------------------------------------------'
			echo "check_copyright.sh: automatic changes to $file:"
			if diff $file check_copyright.2.$$
			then
				echo 'bin/check_copyright.sh: program error'
				rm check_copyright.*.$$
				exit 1
			fi
			ok='no'
			if [ -x $file ]
			then
				mv check_copyright.2.$$ $file
				chmod +x $file
			else
				mv check_copyright.2.$$ $file
			fi
		fi
	fi
done
if [ "$ok" == 'no' ]
then
	rm check_copyright.*.$$
	exit 1
fi
rm check_copyright.*.$$
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
