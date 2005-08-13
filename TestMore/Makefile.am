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
EXTRA_DIST        = TestMore.sln TestMore.vcproj
#
noinst_PROGRAMS   = TestMore
#
if CppAD_BOOSTVECTOR
AM_CPPFLAGS       = -I$(BOOST_DIR) \
                    -I. -I$(top_builddir) 
else
AM_CPPFLAGS       = -I. -I$(top_builddir) 
endif
#
LDADD             = -lCppAD
AM_LDFLAGS        = -L$(top_builddir)/lib
#
AM_CXXFLAGS       = -g $(CPP_ERROR_WARN)
#
TestMore_SOURCES  = TestMore.cpp \
	Add.cpp \
	AddEq.cpp \
	AddZero.cpp \
	Atan.cpp \
	Compare.cpp \
	CompareChange.cpp \
	CondExp.cpp \
	CondExpAD.cpp \
	Div.cpp \
	DivEq.cpp \
	DivZeroOne.cpp \
	Exp.cpp \
	ForHess.cpp \
	Forward.cpp \
	Log.cpp \
	Mul.cpp \
	MulEq.cpp \
	MulZeroOne.cpp \
	Rosen34.cpp \
	Runge45.cpp \
	Reverse.cpp \
	SimpleVector.cpp \
	SinCos.cpp \
	Sqrt.cpp \
	Sub.cpp \
	SubEq.cpp \
	SubZero.cpp \
	VecADPar.cpp \
	VecUnary.cpp 
