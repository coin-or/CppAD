# ---------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
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
#
# automake input file
#
# Note that config.h is needed by VC++; i.e., for users that do not run
# the configure script but rather use the project files 
#
if CppAD_POSTFIX
myincludedir = $(includedir)/$(POSTFIX_DIR)
else
myincludedir = $(includedir)
endif
#
nobase_myinclude_HEADERS =  \
	CppAD/config.h \
	CppAD/CheckNumericType.h \
	CppAD/CheckSimpleVector.h \
	CppAD/CppAD.h \
	CppAD/CppAD_vector.h \
	CppAD/ErrorHandler.h \
	CppAD/LuFactor.h \
	CppAD/LuInvert.h \
	CppAD/LuSolve.h \
	CppAD/NearEqual.h \
	CppAD/OdeErrControl.h \
	CppAD/OdeGear.h \
	CppAD/OdeGearControl.h \
	CppAD/Poly.h \
	CppAD/PowInt.h \
	CppAD/RombergOne.h \
	CppAD/RombergMul.h \
	CppAD/Rosen34.h \
	CppAD/Runge45.h \
	CppAD/SpeedTest.h \
	CppAD/TrackNewDel.h \
	CppAD/local/AD.h \
	CppAD/local/ADFun.h \
	CppAD/local/ADTape.h \
	CppAD/local/ADValued.h \
	CppAD/local/Abs.h \
	CppAD/local/AbsOp.h \
	CppAD/local/AcosOp.h \
	CppAD/local/Add.h \
	CppAD/local/AddEq.h \
	CppAD/local/AddOp.h \
	CppAD/local/Arithmetic.h \
	CppAD/local/AsinOp.h \
	CppAD/local/Atan2.h \
	CppAD/local/AtanOp.h \
	CppAD/local/BenderQuad.h \
	CppAD/local/BoolFun.h \
	CppAD/local/BoolFunLink.h \
	CppAD/local/BoolValued.h \
	CppAD/local/CapTaylor.h \
	CppAD/local/Compare.h \
	CppAD/local/CondExp.h \
	CppAD/local/Constructor.h \
	CppAD/local/Convert.h \
	CppAD/local/CopyAD.h \
	CppAD/local/CopyBase.h \
	CppAD/local/CppADError.h \
	CppAD/local/CppADvector.h \
	CppAD/local/Declare.h \
	CppAD/local/Default.h \
	CppAD/local/Define.h \
	CppAD/local/Discrete.h \
	CppAD/local/Div.h \
	CppAD/local/DivEq.h \
	CppAD/local/DivOp.h \
	CppAD/local/Drivers.h \
	CppAD/local/Eq.h \
	CppAD/local/Erf.h \
	CppAD/local/ExpOp.h \
	CppAD/local/ForOne.h \
	CppAD/local/ForTwo.h \
	CppAD/local/ForJacSweep.h \
	CppAD/local/ForSparseJac.h \
	CppAD/local/Forward.h \
	CppAD/local/ForwardSweep.h \
	CppAD/local/FunCheck.h \
	CppAD/local/FunEval.h \
	CppAD/local/FunOpSeq.h \
	CppAD/local/Hessian.h \
	CppAD/local/Identical.h \
	CppAD/local/Independent.h \
	CppAD/local/Integer.h \
	CppAD/local/Jacobian.h \
	CppAD/local/LogOp.h \
	CppAD/local/Mul.h \
	CppAD/local/MulEq.h \
	CppAD/local/MulOp.h \
	CppAD/local/Op.h \
	CppAD/local/OpCode.h \
	CppAD/local/Ordered.h \
	CppAD/local/Output.h \
	CppAD/local/ParVar.h \
	CppAD/local/Pow.h \
	CppAD/local/PrintFor.h \
	CppAD/local/LuRatio.h \
	CppAD/local/MathOther.h \
	CppAD/local/NearEqualExt.h \
	CppAD/local/Reverse.h \
	CppAD/local/RevHesSweep.h \
	CppAD/local/RevJacSweep.h \
	CppAD/local/RevOne.h \
	CppAD/local/RevSparseJac.h \
	CppAD/local/RevSparseHes.h \
	CppAD/local/RevTwo.h \
	CppAD/local/ReverseSweep.h \
	CppAD/local/SinCos.h \
	CppAD/local/Sparse.h \
	CppAD/local/SqrtOp.h \
	CppAD/local/StdMathUnary.h \
	CppAD/local/Sub.h \
	CppAD/local/SubEq.h \
	CppAD/local/SubOp.h \
	CppAD/local/TapeRec.h \
	CppAD/local/UserAD.h \
	CppAD/local/UnaryMinus.h \
	CppAD/local/UnaryPlus.h \
	CppAD/local/VecAD.h \
	CppAD/local/Undef.h \
	CppAD/local/Value.h 
#
# Did user specify --with-Example
if CppAD_GETSTARTED
GETSTARTED = GetStarted
else
GETSTARTED =
endif
#
# Did user specify --with-Introduction
if CppAD_INTRODUCTION
INTRODUCTION = Introduction
else
INTRODUCTION =
endif
#
# Did user specify --with-Example
if CppAD_EXAMPLE
EXAMPLE = Example
else
EXAMPLE =
endif
#
# Did user specify --with-TestMore
if CppAD_TESTMORE
TESTMORE = TestMore
else
TESTMORE =
endif
#
# Did user specify --with-Speed
if CppAD_SPEED
SPEED = Speed
else
SPEED =
endif
#
# Did user specify --with-PrintFor
if CppAD_PRINTFOR
PRINTFOR = PrintFor
else
PRINTFOR =
endif
#
# Did user specify --with-SpeedExample
if CppAD_SPEEDEXAMPLE
SPEEDEXAMPLE = SpeedExample
else
SPEEDEXAMPLE =
endif
#
# Did user specify a value for ADOLC_DIR in configure command line
if CppAD_ADOLC
ADOLC = Adolc
else
ADOLC = 
endif
#
# Did user specify a value for FADBAD_DIR in configure command line
if CppAD_FADBAD
FADBAD = Fadbad
else
FADBAD = 
endif
#
SUBDIRS = \
	$(ADOLC) \
	$(EXAMPLE) \
	$(INTRODUCTION) \
	$(FADBAD) \
	$(GETSTARTED) \
	$(PRINTFOR) \
	$(SPEEDEXAMPLE) \
	$(SPEED) \
	$(TESTMORE) 
#
EXTRA_DIST = \
	Build \
	CheckIncludeFile \
	CheckIncludeOmh \
	CheckIncludeDef \
	CleanCppAD \
	CppAD.tex \
	Dev \
	Dev.omh \
	Doc \
	Doc.omh \
	Example/OneTest \
	FixAclocal \
	FixMakefile \
	omh \
	RunOMhelp \
	UWCopy040507.html
#
dist-hook:
	rm -rf `find $(distdir)/omh -name .svn`
