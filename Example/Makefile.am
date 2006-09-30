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
EXTRA_DIST        = Example.sln Example.vcproj OneTest
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
Example_SOURCES   = \
	Example.cpp \
	Abs.cpp \
	Acos.cpp \
	Add.cpp \
	AddEq.cpp \
	Asin.cpp \
	Atan.cpp \
	Atan2.cpp \
	BenderQuad.cpp \
	BoolFun.cpp \
	vectorBool.cpp \
	CheckNumericType.cpp \
	CheckSimpleVector.cpp \
	Compare.cpp \
	CompareChange.cpp \
	ComplexPoly.cpp \
	CondExp.cpp \
	CopyAD.cpp \
	CopyBase.cpp \
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
	ErrorHandler.cpp \
	Exp.cpp \
	ForOne.cpp \
	ForTwo.cpp \
	Forward.cpp \
	ForSparseJac.cpp \
	FunCheck.cpp \
	HesLuDet.cpp \
	HesMinorDet.cpp \
	Hessian.cpp \
	HesTimesDir.cpp \
	Independent.cpp \
	Integer.cpp \
	Interface2C.cpp\
	JacLuDet.cpp \
	JacMinorDet.cpp \
	Jacobian.cpp \
	Log.cpp \
	Log10.cpp \
	LuFactor.cpp \
	LuInvert.cpp \
	LuRatio.cpp \
	LuSolve.cpp \
	LuVecAD.cpp \
	LuVecAD.h \
	LuVecADOk.cpp \
	Mul.cpp \
	MulEq.cpp \
	MulTape.cpp \
	Near_Equal.cpp \
	NearEqualExt.cpp \
	NumericType.cpp \
	OdeErrControl.cpp \
	OdeErrMaxabs.cpp \
	OdeGear.cpp \
	OdeGearControl.cpp\
	OdeStiff.cpp \
	OdeTaylor.cpp \
	Output.cpp \
	ParVar.cpp \
	Piecewise.cpp \
	Poly.cpp \
	Pow.cpp \
	PowInt.cpp \
	ReverseAny.cpp \
	ReverseOne.cpp \
	RevOne.cpp \
	RevSparseHes.cpp \
	RevSparseJac.cpp \
	RevTwo.cpp \
	RombergMul.cpp \
	RombergOne.cpp \
	Rosen34.cpp \
	Runge45.cpp \
	SeqProperty.cpp \
	SimpleVector.cpp \
	Sin.cpp \
	Sinh.cpp \
	Sqrt.cpp \
	StackMachine.cpp \
	Sub.cpp \
	SubEq.cpp \
	Tan.cpp \
	TrackNewDel.cpp \
	UnaryMinus.cpp \
	UnaryPlus.cpp \
	Value.cpp \
	VecAD.cpp 
