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
EXTRA_DIST        = Example.sln Example.vcproj
#
noinst_PROGRAMS   = Example 
#
if CppAD_BOOSTVECTOR
AM_CPPFLAGS       = -I$(BOOST_DIR) \
                    -I. -I$(top_builddir) 
else
AM_CPPFLAGS       = -I. -I$(top_builddir)
endif
#
AM_CXXFLAGS       = -g $(CPP_ERROR_WARN)
#
LDADD             = -lCppAD
AM_LDFLAGS        = -L$(top_builddir)/lib
#
Example_SOURCES   = \
	Example.cpp \
	Abs.cpp \
	Acos.cpp \
	Add.cpp \
	AddEq.cpp \
	Asin.cpp \
	Atan.cpp \
	Atan2.cpp \
	BoolFun.cpp \
	Compare.cpp \
	CompareChange.cpp \
	ComplexPoly.cpp \
	CondExp.cpp \
	Copy.cpp \
	Cos.cpp \
	Cosh.cpp \
	CppAD_vector.cpp \
	Default.cpp \
	DetByLu.cpp \
	DetByLu.h \
	DetByMinor.cpp \
	DetByMinor.h \
	DetOfMinor.cpp \
	DetOfMinor.h \
	Discrete.cpp \
	Div.cpp \
	DivEq.cpp \
	Eq.cpp \
	Erf.cpp \
	Exp.cpp \
	ForOne.h \
	ForTwo.h \
	Forward.h \
	FromBase.cpp \
	Fun.h \
	HesLuDet.cpp \
	HesMinorDet.cpp \
	Hessian.h \
	HesTimesDir.cpp \
	Integer.cpp \
	JacLuDet.cpp \
	JacMinorDet.cpp \
	Jacobian.h \
	Log.cpp \
	Log10.cpp \
	LuFactor.cpp \
	LuInvert.cpp \
	LuSolve.cpp \
	LuVecAD.cpp \
	LuVecAD.h \
	LuVecADOk.cpp \
	Mul.cpp \
	MulEq.cpp \
	MulTape.cpp \
	Near_Equal.cpp \
	NearEqualExt.h \
	Neg.cpp \
	OdeErrControl.cpp \
	OdeErrMaxabs.cpp \
	OdeTaylor.cpp \
	Output.cpp \
	Poly.cpp \
	Pow.cpp \
	PowInt.cpp \
	RevOne.h \
	RevTwo.h \
	Reverse.h \
	RombergMul.cpp \
	RombergOne.cpp \
	Rosen34.cpp \
	Runge45.cpp \
	CheckSimpleVector.cpp \
	SimpleVector.cpp \
	Sin.cpp \
	Sinh.cpp \
	Sqrt.cpp \
	Sub.cpp \
	SubEq.cpp \
	Value.cpp \
	Vec.cpp 
