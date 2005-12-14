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
