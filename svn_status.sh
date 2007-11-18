# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
svn status | \
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
	-e '/cygwin_package$/d'                               \
	-e '/^[?] *cppad-[0-9]\{8\}$/d'
#
yyyymmdd=`date +%G%m%d`
yyyy_mm_dd=`date +%G-%m-%d`
#
svn cat configure | sed > configure.$$ \
	-e "s/CppAD [0-9]\{8\}/CppAD $yyyymmdd/g" \
	-e "s/VERSION='[0-9]\{8\}'/VERSION='$yyyymmdd'/g" \
	-e "s/configure [0-9]\{8\}/configure $yyyymmdd/g" \
	-e "s/config.status [0-9]\{8\}/config.status $yyyymmdd/g" \
	-e "s/\$as_me [0-9]\{8\}/\$as_me $yyyymmdd/g" 
svn cat AUTHORS | sed > AUTHORS.$$ \
	-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/"
svn cat configure.ac | sed > configure.ac.$$\
	-e "s/(CppAD, [0-9]\{8\} *,/(CppAD, $yyyymmdd,/" 
svn cat omh/download.omh | sed > omh/download.omh.$$ \
	-e "s/cppad-[0-9]\{8\}/cppad-$yyyymmdd/g"
svn cat cppad/config.h | sed > cppad/config.h.$$ \
	-e "s/CppAD [0-9]\{8\}/CppAD $yyyymmdd/g" \
	-e "s/VERSION \"[0-9]\{8\}\"/VERSION \"$yyyymmdd\"/g"
list="
	configure
	AUTHORS
	configure.ac
	omh/download.omh
	cppad/config.h
"
for name in $list
do
	echo "diff $name-local $name-subversion"
	diff $name $name.$$
	rm   $name.$$
done


