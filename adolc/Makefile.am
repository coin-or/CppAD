# BEGIN SHORT COPYRIGHT
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
# END SHORT COPYRIGHT
# automake input file
#
noinst_PROGRAMS  = example speed 
#
if CppAD_BOOSTVECTOR
example_CPPFLAGS = -I$(BOOST_DIR) \
                   -I. -I$(top_builddir) -I$(ADOLC_DIR)/include 
speed_CPPFLAGS   = -I$(BOOST_DIR) \
                   -I. -I$(top_builddir) -I$(ADOLC_DIR)/include 
else
example_CPPFLAGS = -I. -I$(top_builddir) -I$(ADOLC_DIR)/include 
speed_CPPFLAGS   = -I. -I$(top_builddir) -I$(ADOLC_DIR)/include 
endif
#
example_CXXFLAGS = -g $(CPP_ERROR_WARN)
speed_CXXFLAGS   = -O2 -DNDEBUG $(CPP_ERROR_WARN)
#
LDADD            = -ladolc 
AM_LDFLAGS       = -L$(ADOLC_DIR)/lib 
#
# cancel the -DHAVE_CONGIF_H flag so adolc does not try to use it
DEFS             =
#
example_SOURCES  = example.cpp \
	alloc_mat.cpp \
	alloc_mat.hpp \
	alloc_vec.cpp \
	alloc_vec.hpp \
	det_lu.cpp \
	det_minor.cpp 
#
speed_SOURCES    = speed.cpp \
	det_lu.cpp \
	det_minor.cpp 
