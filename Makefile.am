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
	CppAD/CppAD.h \
	CppAD/CppADError.h \
	CppAD/CppAD_vector.h \
	CppAD/LuFactor.h \
	CppAD/LuInvert.h \
	CppAD/LuSolve.h \
	CppAD/NearEqual.h \
	CppAD/OdeErrControl.h \
	CppAD/OdeGear.h \
	CppAD/OdeGearControl.h \
	CppAD/Poly.h \
	CppAD/RombergOne.h \
	CppAD/RombergMul.h \
	CppAD/Rosen34.h \
	CppAD/Runge45.h \
	CppAD/SpeedTest.h \
	CppAD/CheckNumericType.h \
	CppAD/CheckSimpleVector.h \
	CppAD/local/AD.h \
	CppAD/local/ADBoolFun.h \
	CppAD/local/ADDefine.h \
	CppAD/local/ADForward.h \
	CppAD/local/ADReverse.h \
	CppAD/local/ADTape.h \
	CppAD/local/ADUndef.h \
	CppAD/local/Abs.h \
	CppAD/local/AbsOp.h \
	CppAD/local/Acos.h \
	CppAD/local/AcosOp.h \
	CppAD/local/Add.h \
	CppAD/local/AddEq.h \
	CppAD/local/AddOp.h \
	CppAD/local/Asin.h \
	CppAD/local/AsinOp.h \
	CppAD/local/Atan.h \
	CppAD/local/Atan2.h \
	CppAD/local/AtanOp.h \
	CppAD/local/BinaryOp.h \
	CppAD/local/BoolFun.h \
	CppAD/local/CompEq.h \
	CppAD/local/Compare.h \
	CppAD/local/CondExp.h \
	CppAD/local/Copy.h \
	CppAD/local/Cos.h \
	CppAD/local/Cosh.h \
	CppAD/local/CppADvector.h \
	CppAD/local/Declare.h \
	CppAD/local/Default.h \
	CppAD/local/Discrete.h \
	CppAD/local/Div.h \
	CppAD/local/DivEq.h \
	CppAD/local/DivOp.h \
	CppAD/local/Eq.h \
	CppAD/local/Erf.h \
	CppAD/local/Exp.h \
	CppAD/local/ExpOp.h \
	CppAD/local/ExtendBuffer.h \
	CppAD/local/ForOne.h \
	CppAD/local/ForTwo.h \
	CppAD/local/Forward.h \
	CppAD/local/FromBase.h \
	CppAD/local/Fun.h \
	CppAD/local/Hessian.h \
	CppAD/local/Identical.h \
	CppAD/local/Independent.h \
	CppAD/local/Integer.h \
	CppAD/local/Jacobian.h \
	CppAD/local/Log.h \
	CppAD/local/Log10.h \
	CppAD/local/LogOp.h \
	CppAD/local/Mul.h \
	CppAD/local/MulEq.h \
	CppAD/local/MulOp.h \
	CppAD/local/Op.h \
	CppAD/local/Operator.h \
	CppAD/local/Ordered.h \
	CppAD/local/Output.h \
	CppAD/local/Parameter.h \
	CppAD/local/Pow.h \
	CppAD/local/PrintFor.h \
	CppAD/local/RevOne.h \
	CppAD/local/RevTwo.h \
	CppAD/local/Reverse.h \
	CppAD/local/Sin.h \
	CppAD/local/Sinh.h \
	CppAD/local/SinCos.h \
	CppAD/local/Sqrt.h \
	CppAD/local/SqrtOp.h \
	CppAD/local/StdMathLib.h \
	CppAD/local/Sub.h \
	CppAD/local/SubEq.h \
	CppAD/local/SubOp.h \
	CppAD/local/TapeRec.h \
	CppAD/local/UserAD.h \
	CppAD/local/UnaryMinus.h \
	CppAD/local/UnaryPlus.h \
	CppAD/local/Value.h \
	CppAD/local/Vec.h
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
	lib \
	$(ADOLC) \
	$(EXAMPLE) \
	$(FADBAD) \
	GetStarted \
	PrintFor \
	$(SPEED) \
	$(TESTMORE) 
#
EXTRA_DIST = \
	Build \
	CheckIncludeFile \
	CppAD.tex \
	Developer \
	Developer.omh \
	Doc \
	Doc.omh \
	Example/OneTest \
	FixMakefile \
	omh \
	RunOMhelp \
	UWCopy040507.html
#
dist-hook:
	rm -rf `find $(distdir)/omh -name .svn`
