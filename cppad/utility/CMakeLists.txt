# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
FOREACH(old_include_file
	check_numeric_type.hpp
	check_simple_vector.hpp
	elapsed_seconds.hpp
	error_handler.hpp
	index_sort.hpp
	lu_factor.hpp
	lu_invert.hpp
	lu_solve.hpp
	memory_leak.hpp
	nan.hpp
	near_equal.hpp
	ode_err_control.hpp
	ode_gear.hpp
	ode_gear_control.hpp
	omp_alloc.hpp
	poly.hpp
	pow_int.hpp
	romberg_mul.hpp
	romberg_one.hpp
	rosen_34.hpp
	runge_45.hpp
	speed_test.hpp
	thread_alloc.hpp
	time_test.hpp
	track_new_del.hpp
	vector.hpp
	CheckNumericType.h
	CheckSimpleVector.h
	CppAD.h
	CppAD_vector.h
	ErrorHandler.h
	LuFactor.h
	LuInvert.h
	LuSolve.h
	NearEqual.h
	OdeErrControl.h
	OdeGear.h
	OdeGearControl.h
	Poly.h
	PowInt.h
	RombergMul.h
	RombergOne.h
	Rosen34.h
	Runge45.h
	SpeedTest.h
	TrackNewDel.h
)
	FILE(WRITE
		${CMAKE_BINARY_DIR}/cppad/utility/${old_include_file} 
		"# include <cppad/utility.hpp>\n"
	)
ENDFOREACH(old_include_file)
