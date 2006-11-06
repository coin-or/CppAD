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
EXTRA_DIST      = speed.sln speed.vcproj gprof.sed
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
# speed
#
noinst_PROGRAMS  = speed
speed_CXXFLAGS   = -O2 $(CPP_ERROR_WARN)
speed_SOURCES    = \
	speed.cpp \
	det_lu.cpp \
	det_minor.cpp \
	lu_solve.cpp \
	ode.cpp \
	../example_/LuVecAD.cpp
#
# speed_p
#
if CppAD_PROFILING
noinst_PROGRAMS  += speed_p
speed_p_CXXFLAGS = -O2 -pg $(CPP_ERROR_WARN)
speed_p_LDFLAGS  = -pg 
speed_p_SOURCES  = $(speed_SOURCES)
endif
