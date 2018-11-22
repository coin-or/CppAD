#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#            GNU General Public License, Version 3.0.
# -----------------------------------------------------------------------------
# configuration file for tools in ~/devel.
#
# web address linked by run_omhelp.sh to the Home icon on each page.
image_link='https://coin-or.github.io/CppAD'
echo "image_link=$image_link"
#
# special files, for this repository, that check_copyright.sh should ignore
copyright_ignore='
	epl-2.0.txt
'
echo "coyright_ignore=$coyright_ignore"
