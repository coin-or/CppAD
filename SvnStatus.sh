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
# -----------------------------------------------------------------------------
#
svn status | \
sed                                                           \
	-e '/^[?].*\.out$/d'                                  \
	-e '/^[?].*\.gz$/d'                                   \
	-e '/^[?].*\.zip$/d'                                  \
	-e '/^[?].*\.swp$/d'                                  \
	-e '/^[?].*\.cache$/d'                                \
	-e '/^[?].*\.in$/d'                                   \
	-e '/^[?].*\.exe$/d'                                  \
	-e '/^[?].*\.ncb$/d'                                  \
	-e '/^[?].*\.suo$/d'                                  \
	-e '/^[?].*\.deps$/d'                                 \
	-e '/^[?].*\.a$/d'                                    \
	-e '/^[?].*\.log$/d'                                  \
	-e '/^[?].*\.stackdump$/d'                            \
	-e '/^[?].*\/TestOne.cpp$/d'                          \
	-e '/^[?].*\/Debug$/d'                                \
	-e '/^[?].*\/Release$/d'                              \
	-e '/^[?].*\/stamp-h1$/d'                             \
	-e '/^[?].*\/Makefile$/d'                             \
	-e '/^[?] *install-sh$/d'                             \
	-e '/^[?] *Makefile$/d'                               \
	-e '/^[?].*\/junk$/d'                                 \
	-e '/^[?] *junk$/d'                                   \
	-e '/^[?].*\/junk\.[0-9a-zA-Z]*$/d'                   \
	-e '/^[?] *junk\.[0-9a-zA-Z]*$/d'                     \
	-e '/^[?] *configure$/d'                              \
	-e '/^[?] *Dev$/d'                                    \
	-e '/^[?] *config.status$/d'                          \
	-e '/^[?] *Doc$/d'                                    \
	-e '/^[?] *aclocal.m4$/d'                             \
	-e '/^[?] *cppad-[0-9][0-9]-[0-9][0-9]-[0-9][0-9]$/d' 
#
# Add differences for automatically updated files
./diffToday.sh AUTHORS
./diffToday.sh Doc.omh
./diffToday.sh configure.ac
./diffToday.sh omh/InstallUnix.omh
./diffToday.sh omh/InstallWindows.omh
