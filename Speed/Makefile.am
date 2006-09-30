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
# Microsoft project files
EXTRA_DIST      = Speed.sln Speed.vcproj gprof.sed
#
#
if CppAD_BOOSTVECTOR
BOOST_INCLUDE    = -I$(BOOST_DIR) 
else
BOOSE_INCLUDE    =
endif
#
AM_CPPFLAGS      = $(BOOST_INCLUDE) -I. -I$(top_builddir) -DNDEBUG
#
# Speed
#
noinst_PROGRAMS  = Speed
Speed_CXXFLAGS   = -O2 $(CPP_ERROR_WARN)
Speed_SOURCES    = \
	Speed.cpp \
	DetLu.cpp \
	DetMinor.cpp \
	LuSolve.cpp \
	Ode.cpp \
	../Example/LuVecAD.cpp
#
# Speed_p
#
if CppAD_PROFILING
noinst_PROGRAMS  += Speed_p
Speed_p_CXXFLAGS = -O2 -pg $(CPP_ERROR_WARN)
Speed_p_LDFLAGS  = -pg 
Speed_p_SOURCES  = $(Speed_SOURCES)
endif
