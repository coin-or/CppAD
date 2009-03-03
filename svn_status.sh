# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
flag="$1"
if [ "$flag" == "" ]
then
	echo "./svn_status.sh <makefile.in flag>"
	echo "where makefile.in flag is + or -"
	exit 1
fi
svn status | > svn_status.$$ \
sed                                                           \
	-e '/^[?].*\.[0-9]*$/d'                               \
	-e '/^[?].*\.tmp$/d'                                  \
	-e '/^[?].*\.out$/d'                                  \
	-e '/^[?].*\.gz$/d'                                   \
	-e '/^[?].*\.tgz$/d'                                  \
	-e '/^[?].*\.zip$/d'                                  \
	-e '/^[?].*\.swp$/d'                                  \
	-e '/^[?].*\.cache$/d'                                \
	-e '/^[?].*\.exe$/d'                                  \
	-e '/^[?].*\.ncb$/d'                                  \
	-e '/^[?].*\.suo$/d'                                  \
	-e '/^[?].*\.deps$/d'                                 \
	-e '/^[?].*\.a$/d'                                    \
	-e '/^[?].*\.log$/d'                                  \
	-e '/^[?].*\.stackdump$/d'                            \
	-e '/^[?].*\/test_one.cpp$/d'                         \
	-e '/^[?].*\/Debug$/d'                                \
	-e '/^[?].*\/Release$/d'                              \
	-e '/^[?].*\/stamp-h1$/d'                             \
	-e '/^[?].*\/makefile$/d'                             \
	-e '/^[?].*\/_adolc-*/d'                              \
	-e '/^[?] *makefile$/d'                               \
	-e '/^[?].*\/junk$/d'                                 \
	-e '/^[?] *junk$/d'                                   \
	-e '/^[?].*\/junk\.[0-9a-zA-Z]*$/d'                   \
	-e '/^[?] *junk\.[0-9a-zA-Z]*$/d'                     \
	-e '/^[?] *dev$/d'                                    \
	-e '/^[?] *config.status$/d'                          \
	-e '/^[?] *doc$/d'                                    \
	-e '/^[?] *dev$/d'                                    \
	-e '/^[?] *aclocal.m4$/d'                             \
	-e '/^[?] *cppad-[0-9]\{8\}.*$/d'                     \
	-e '/^[?] *doc.omh$/d'                                \
	-e '/^[?] *omh\/install_unix.omh$/d'                  \
	-e '/^[?] *omh\/install_windows.omh$/d'               \
	-e '/^[?] *example\/test_one.sh$/d'                   \
	-e '/^[?] *test_more\/test_one.sh$/d'                 \
	-e '/^[?] *example\/example$/d'                       \
	-e '/^[?] *speed\/adolc\/adolc$/d'                    \
	-e '/^[?] *speed\/cppad\/cppad$/d'                    \
	-e '/^[?] *speed\/fadbad\/fadbad$/d'                  \
	-e '/^[?] *speed\/profile\/profile$/d'                \
	-e '/^[?] *ipopt_cppad\/ipopt_cppad$/d'               \
	-e '/^[?] *ipopt_cppad\/include$/d'                      \
	-e '/^[?] *ipopt_cppad\/lib$/d'                          \
	-e '/cygwin_package$/d'
#
if [ "$flag" == "+" ]
then
	cat svn_status.$$
else
	cat svn_status.$$ | sed -e '/\/makefile.in$/d' -e '/ makefile\.in/d'
fi
yyyymmdd=`date +%G%m%d`
yyyy_mm_dd=`date +%G-%m-%d`
#
svn cat configure | sed > configure.$$ \
	-e "s/CppAD [0-9]\{8\}[.0-9]*/CppAD $yyyymmdd/g" \
	-e 's/CppAD [0-9]\{8\}[.0-9]*$/&./' \
	-e "s/VERSION='[0-9]\{8\}[.0-9]*'/VERSION='$yyyymmdd'/g" \
	-e "s/configure [0-9]\{8\}[.0-9]*/configure $yyyymmdd/g" \
	-e "s/config.status [0-9]\{8\}[.0-9]*/config.status $yyyymmdd/g" \
	-e "s/\$as_me [0-9]\{8\}[.0-9]*/\$as_me $yyyymmdd/g" 
svn cat AUTHORS | sed > AUTHORS.$$ \
	-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/"
svn cat configure.ac | sed > configure.ac.$$\
	-e "s/(CppAD, [0-9]\{8\}[.0-9]* *,/(CppAD, $yyyymmdd,/" 
svn cat cppad/config.h | sed > cppad/config.h.$$ \
	-e "s/CppAD [0-9]\{8\}[.0-9]*/CppAD $yyyymmdd/g" \
	-e "s/VERSION \"[0-9]\{8\}[.0-9]*\"/VERSION \"$yyyymmdd\"/g"
list="
	configure
	AUTHORS
	configure.ac
	cppad/config.h
"
for name in $list
do
	echo "diff $name-local $name-subversion"
	diff $name $name.$$
	rm   $name.$$
done
#
rm svn_status.$$
