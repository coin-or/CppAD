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
# automake input file
#
# Microsoft project files
EXTRA_DIST      = Speed.sln Speed.vcproj
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
