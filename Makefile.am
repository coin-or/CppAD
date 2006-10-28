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
	CppAD/local/ad.hpp \
	CppAD/local/ad_fun.hpp \
	CppAD/local/ad_tape.hpp \
	CppAD/local/ad_valued.hpp \
	CppAD/local/abs.hpp \
	CppAD/local/abs_op.hpp \
	CppAD/local/acos_op.hpp \
	CppAD/local/add.hpp \
	CppAD/local/add_eq.hpp \
	CppAD/local/add_op.hpp \
	CppAD/local/arithmetic.hpp \
	CppAD/local/asin_op.hpp \
	CppAD/local/atan2.hpp \
	CppAD/local/atan_op.hpp \
	CppAD/local/bender_quad.hpp \
	CppAD/local/bool_fun.hpp \
	CppAD/local/bool_fun_link.hpp \
	CppAD/local/bool_valued.hpp \
	CppAD/local/cap_taylor.hpp \
	CppAD/local/compare.hpp \
	CppAD/local/cond_exp.hpp \
	CppAD/local/constructor.hpp \
	CppAD/local/convert.hpp \
	CppAD/local/copy_ad.hpp \
	CppAD/local/copy_base.hpp \
	CppAD/local/cppad_error.hpp \
	CppAD/local/cppad_vector.hpp \
	CppAD/local/declare.hpp \
	CppAD/local/default.hpp \
	CppAD/local/define.hpp \
	CppAD/local/dependent.hpp \
	CppAD/local/discrete.hpp \
	CppAD/local/div.hpp \
	CppAD/local/div_eq.hpp \
	CppAD/local/div_op.hpp \
	CppAD/local/drivers.hpp \
	CppAD/local/eq.hpp \
	CppAD/local/equal_op_seq.hpp \
	CppAD/local/erf.hpp \
	CppAD/local/exp_op.hpp \
	CppAD/local/for_one.hpp \
	CppAD/local/for_two.hpp \
	CppAD/local/for_jac_sweep.hpp \
	CppAD/local/for_sparse_jac.hpp \
	CppAD/local/forward.hpp \
	CppAD/local/forward_sweep.hpp \
	CppAD/local/fun_check.hpp \
	CppAD/local/fun_eval.hpp \
	CppAD/local/fun_construct.hpp \
	CppAD/local/hessian.hpp \
	CppAD/local/identical.hpp \
	CppAD/local/independent.hpp \
	CppAD/local/integer.hpp \
	CppAD/local/jacobian.hpp \
	CppAD/local/log_op.hpp \
	CppAD/local/mul.hpp \
	CppAD/local/mul_eq.hpp \
	CppAD/local/mul_op.hpp \
	CppAD/local/op.hpp \
	CppAD/local/op_code.hpp \
	CppAD/local/ordered.hpp \
	CppAD/local/output.hpp \
	CppAD/local/par_var.hpp \
	CppAD/local/pow.hpp \
	CppAD/local/preprocessor.hpp \
	CppAD/local/print_for.hpp \
	CppAD/local/lu_ratio.hpp \
	CppAD/local/math_other.hpp \
	CppAD/local/near_equal_ext.hpp \
	CppAD/local/reverse.hpp \
	CppAD/local/rev_hes_sweep.hpp \
	CppAD/local/rev_jac_sweep.hpp \
	CppAD/local/rev_one.hpp \
	CppAD/local/rev_sparse_jac.hpp \
	CppAD/local/rev_sparse_hes.hpp \
	CppAD/local/rev_two.hpp \
	CppAD/local/reverse_sweep.hpp \
	CppAD/local/sin_cos.hpp \
	CppAD/local/sparse.hpp \
	CppAD/local/sqrt_op.hpp \
	CppAD/local/std_math_unary.hpp \
	CppAD/local/sub.hpp \
	CppAD/local/sub_eq.hpp \
	CppAD/local/sub_op.hpp \
	CppAD/local/tape_rec.hpp \
	CppAD/local/user_ad.hpp \
	CppAD/local/unary_minus.hpp \
	CppAD/local/unary_plus.hpp \
	CppAD/local/vec_ad.hpp \
	CppAD/local/undef.hpp \
	CppAD/local/value.hpp \
	CppAD/local/var2par.hpp 
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
	Build.sh \
	CheckIncludeFile.sh \
	CheckIncludeOmh.sh \
	CheckIncludeDef.sh \
	CleanCppAD.sh \
	CppAD.tex \
	cpl1.0.txt \
	Dev \
	Dev.omh \
	Doc \
	Doc.omh \
	FixAclocal.sh \
	FixMakefile.sh \
	omh \
	RunOMhelp.sh \
	UWCopy040507.html
#
dist-hook:
	rm -rf `find $(distdir)/omh -name .svn`
