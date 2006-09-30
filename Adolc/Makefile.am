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
noinst_PROGRAMS  = Example Speed 
#
if CppAD_BOOSTVECTOR
Example_CPPFLAGS = -I$(BOOST_DIR) \
                   -I. -I$(top_builddir) -I$(ADOLC_DIR)/include 
Speed_CPPFLAGS   = -I$(BOOST_DIR) \
                   -I. -I$(top_builddir) -I$(ADOLC_DIR)/include 
else
Example_CPPFLAGS = -I. -I$(top_builddir) -I$(ADOLC_DIR)/include 
Speed_CPPFLAGS   = -I. -I$(top_builddir) -I$(ADOLC_DIR)/include 
endif
#
Example_CXXFLAGS = -g $(CPP_ERROR_WARN)
Speed_CXXFLAGS   = -O2 -DNDEBUG $(CPP_ERROR_WARN)
#
LDADD            = -ladolc 
AM_LDFLAGS       = -L$(ADOLC_DIR)/lib 
#
# cancel the -DHAVE_CONGIF_H flag so adolc does not try to use it
DEFS             =
#
Example_SOURCES  = Example.cpp \
	AllocMat.cpp \
	AllocMat.h \
	AllocVec.cpp \
	AllocVec.h \
	DetLu.cpp \
	DetMinor.cpp 
#
Speed_SOURCES    = Speed.cpp \
	DetLu.cpp \
	DetMinor.cpp 
