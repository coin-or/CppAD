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
AM_CPPFLAGS      = -I$(BOOST_DIR) \
                   -I. -I$(top_builddir) -I$(FADBAD_DIR) 
else
AM_CPPFLAGS      = -I. -I$(top_builddir) -I$(FADBAD_DIR) 
endif
#
# Fadbad will not survive some of the error and warning flags that could be
# in CPP_ERROR_WARN, so do not use them
Example_CXXFLAGS = -g -Wno-deprecated  
Speed_CXXFLAGS   = -O2 -DNDEBUG -Wno-deprecated  
#
Example_SOURCES  = \
	Example.cpp \
	DetLu.cpp \
	DetMinor.cpp 
#
Speed_SOURCES    = \
	Speed.cpp \
	DetLu.cpp \
	DetMinor.cpp 
