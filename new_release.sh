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
repository="https://projects.coin-or.org/svn/CppAD"
stable_version="20080826"
release="0"
release_version="$stable_version.$release"
# -----------------------------------------------------------------------------
rep_stable="$repository/stable/$stable_version"
rep_release="$repository/releases/$release_version"
msg="Creating releases/$release_version"
echo "svn copy $rep_stable $rep_release -m \"$msg\""
if ! svn copy $rep_stable $rep_release -m "$msg"
then
	"Cannot create $rep_release"
	exit 1
fi
