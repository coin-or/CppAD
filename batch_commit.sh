# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# ----------------------------------------------------------------------
log_entry="Add the following line to the end of the license text:

# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
"
list=`find . \
	\( -name '*.am'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.sh' \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.h'   \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.omh' \) | \
	sed -e '/ChangeCopy\.sh/d' -e '/config\.h/d'`
echo "$log_entry" > SvnCommit.log
echo "svn commit --username bradbell --file SvnCommit.log $list"
svn commit --username bradbell --file SvnCommit.log $list
