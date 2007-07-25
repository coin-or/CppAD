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
	-e '/^[?].*\.tmp$/d'                                  \
	-e '/^[?].*\.out$/d'                                  \
	-e '/^[?].*\.gz$/d'                                   \
	-e '/^[?].*\.tgz$/d'                                  \
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
	-e '/^[?].*\/test_one.cpp$/d'                         \
	-e '/^[?].*\/Debug$/d'                                \
	-e '/^[?].*\/Release$/d'                              \
	-e '/^[?].*\/stamp-h1$/d'                             \
	-e '/^[?].*\/makefile$/d'                             \
	-e '/^[?].*\/_adolc-*/d'                              \
	-e '/^[?] *install-sh$/d'                             \
	-e '/^[?] *makefile$/d'                               \
	-e '/^[?] *depcomp$/d'                                \
	-e '/^[?] *missing$/d'                                \
	-e '/^[?].*\/junk$/d'                                 \
	-e '/^[?] *junk$/d'                                   \
	-e '/^[?].*\/junk\.[0-9a-zA-Z]*$/d'                   \
	-e '/^[?] *junk\.[0-9a-zA-Z]*$/d'                     \
	-e '/^[?] *configure$/d'                              \
	-e '/^[?] *dev$/d'                                    \
	-e '/^[?] *config.status$/d'                          \
	-e '/^[?] *Doc$/d'                                    \
	-e '/^[?] *doc$/d'                                    \
	-e '/^[?] *dev$/d'                                    \
	-e '/^[?] *aclocal.m4$/d'                             \
	-e '/cygwin_package$/d'                               \
	-e '/^[?] *cppad-[0-9]\{8\}$/d'                       \
	-e '/^M *AUTHORS$/d'                                  \
	-e '/^M *configure.ac$/d'                             \
	-e '/^M *doc.omh$/d'                                  \
	-e '/^M *omh[/]install_unix.omh$/d'                   \
	-e '/^M *omh[/]install_windows.omh$/d'
#
# Add differences for automatically updated files
./diff_today.sh AUTHORS
./diff_today.sh doc.omh
./diff_today.sh configure.ac
./diff_today.sh omh/install_unix.omh
./diff_today.sh omh/install_windows.omh
