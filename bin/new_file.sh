#! /bin/bash -eu
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
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
if [ "$0" != "bin/new_file.sh" ]
then
	echo "bin/new_file.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
if [ "$#" != '1' ]
then
	echo "bin/new_file.sh: file_name"
	exit 1
fi
file_name="$1"
if [ -e "$file_name" ]
then
	echo "$file_name already exists"
	exit 1
fi
ext=`echo $file_name | sed -e 's/[^.]*\.//'`
if [ "$ext" == '' ] || [ "$ext" == "$file_name" ]
then
	echo 'bin/new_file.sh: file_name does not have an extension'
	exit 1
fi
#
# -----------------------------------------------------------------------------
case $ext in
	# -------------------------------------------------------------------------
	hpp | hpp.in )
	dir=`echo $file_name | sed -e 's|/.*||'`
	if [ "$dir" != 'cppad' ] || [ "$file_name" == 'cppad' ]
	then
		echo 'new_file.sh: file_name does not begin with cppad/'
		exit 1
	fi
	upper_name=`echo $file_name | tr [a-z./] [A-Z__]`
	cat << EOF  > $file_name
# ifndef $upper_name
# define $upper_name
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# endif
EOF
	;;
	# -------------------------------------------------------------------------
	cpp  | cpp.in )
	cat << EOF  > $file_name
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
EOF
	;;
	# -------------------------------------------------------------------------
	cmake | txt )
	cat << EOF  > $file_name
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
                      Eclipse Public License Version 1.0.

# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
EOF
	;;
	# -------------------------------------------------------------------------
	omh | omh.in )
	root_name=`echo $file_name | sed -e 's|.*/||' -e 's|\.omh.*||'`
	cat << EOF  > $file_name
-------------------------------------------------------------------------------
  CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

  CppAD is distributed under multiple licenses. This distribution is under
  the terms of the
                      Eclipse Public License Version 1.0.

  A copy of this license is included in the COPYING file of this distribution.
  Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------------
\$begin $root_name\$\$
\$spell
\$\$

\$section \$\$

\$end
EOF
	;;
	# -------------------------------------------------------------------------
	sh | sh.in )
	dir=`echo $file_name | sed -e 's|/.*||'`
	if [ "$dir" != 'bin' ] || [ "$file_name" == 'bin' ]
	then
		echo 'new_file.sh: file_name does not begin with bin/'
		exit 1
	fi
	cat << EOF  > $file_name
#! /bin/bash -eu
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
# bash function that echos and executes a command
echo_eval() {
	echo \$*
	eval \$*
}
# -----------------------------------------------------------------------------
if [ "\$0" != "$file_name" ]
then
	echo "$file_name: must be executed from its parent directory"
	exit 1
fi
if [ "\$#" != '?' ]
then
	echo 'usage: $file_name.sh ?'
fi
# -----------------------------------------------------------------------------
REPLACE THIS LINE, AND ? ABOVE, BY THE CODE FOR THIS SCRIPT.
# -----------------------------------------------------------------------------
echo '$file_name: OK'
exit 0
EOF
	echo_eval chmod +x $file_name
	;;

	*)
	echo bin/"new_file.sh: extension $ext is not yet supported"
	exit 1
esac
# -----------------------------------------------------------------------------
echo_eval git add $file_name
echo 'bin/new_file.sh: OK'
exit 0
