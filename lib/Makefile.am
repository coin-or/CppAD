# BEGIN SHORT COPYRIGHT
# ---------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# ---------------------------------------------------------------------------
# END SHORT COPYRIGHT
#
# automake input file
#
if CppAD_BOOSTVECTOR
BoostInclude = -I$(BOOST_DIR)
else
BoostInclude =
endif
#
if CppAD_POSTFIX
mylibdir = $(libdir)/$(POSTFIX_DIR)
else
mylibdir = $(libdir)
endif
#
# libCppAD
#
mylib_LIBRARIES       = libCppAD.a 
libCppAD_a_CPPFLAGS   = -I. -I$(top_builddir) $(BoostInclude)
libCppAD_a_CXXFLAGS   = -g $(CPP_ERROR_WARN)
libCppAD_a_SOURCES    = NearEqual.cpp SpeedTest.cpp ErrFun.cpp
#
# libCppAD_p
#
if CppAD_PROFILING
mylib_LIBRARIES      += libCppAD_p.a 
libCppAD_p_a_CPPFLAGS = -I. -I$(top_builddir) $(BoostInclude) -DNDEBUG
libCppAD_p_a_CXXFLAGS = -pg -g $(CPP_ERROR_WARN)
libCppAD_p_a_SOURCES  = NearEqual.cpp SpeedTest.cpp ErrFun.cpp
endif
#
# SpeedTestExample
#
noinst_PROGRAMS           = SpeedTestExample
noinst_DATA               = SpeedTestExample.out
SpeedTestExample_CPPFLAGS = -I. -I$(top_builddir) $(BoostInclude)
SpeedTestExample_CXXFLAGS = -g $(CPP_ERROR_WARN)
SpeedTestExample_SOURCES = SpeedTestExample.cpp SpeedTest.cpp
#
SpeedTestExample.out : SpeedTestExample$(EXEEXT)
	./SpeedTestExample > SpeedTestExample.out
