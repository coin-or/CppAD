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
# automake input file
#
# Microsoft project files
EXTRA_DIST        = TestMore.sln TestMore.vcproj OneTest.sh
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
AM_CXXFLAGS       = -g $(CPP_ERROR_WARN)
#
TestMore_SOURCES  = TestMore.cpp \
	Abs.cpp \
	Acos.cpp \
	Asin.cpp \
	Add.cpp \
	AddEq.cpp \
	AddZero.cpp \
	Atan.cpp \
	Atan2.cpp \
	Compare.cpp \
	CompareChange.cpp \
	CondExp.cpp \
	CondExpAD.cpp \
	Copy.cpp \
	Cos.cpp \
	Cosh.cpp \
	Div.cpp \
	DivEq.cpp \
	DivZeroOne.cpp \
	Erf.cpp \
	Exp.cpp \
	ForHess.cpp \
	ForSparseJac.cpp \
	Forward.cpp \
	FromBase.cpp \
	Log.cpp \
	Log10.cpp \
	Mul.cpp \
	MulEq.cpp \
	MulZeroOne.cpp \
	NearEqualExt.cpp \
	Neg.cpp \
	OdeErrControl.cpp \
	Poly.cpp \
	Pow.cpp \
	PowInt.cpp \
	Rosen34.cpp \
	Runge45.cpp \
	Reverse.cpp \
	RevSparseHes.cpp \
	RevSparseJac.cpp \
	RevTwo.cpp \
	SimpleVector.cpp \
	Sin.cpp \
	SinCos.cpp \
	Sinh.cpp \
	SparseVecAD.cpp \
	Sqrt.cpp \
	Sub.cpp \
	SubEq.cpp \
	SubZero.cpp \
	Value.cpp \
	VecAD.cpp \
	VecADPar.cpp \
	VecUnary.cpp 
