/* $Id$ */
# ifndef CPPAD_TESTVECTOR_INCLUDED
# define CPPAD_TESTVECTOR_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin testvector$$
$spell
	cppad
	Eigen
	ifdef
	undef
	Microsofts
	CppADvector
	boostvector
	hpp
	std
	endif
	ublas
	Dir
	valarray
	stdvector
$$

$index CPPAD_TESTVECTOR$$
$index vector, test$$
$index test, vector$$

$section Choosing The Vector Testing Template Class$$

$head Syntax$$
$codei%CPPAD_TESTVECTOR(%Scalar%)
%$$

$head Introduction$$
Many of the CppAD $cref/examples/example/$$ and tests use 
the $code CPPAD_TESTVECTOR$$ template class to pass information to CppAD.
This is not a true template class because it's syntax uses
$codei%(%Scalar%)%$$ instead of $codei%<%Scalar%>%$$.
This enables us to use
$codei%
	Eigen::Matrix<%Scalar%, Eigen::Dynamic, 1>
%$$
as one of the possible cases for this 'template class'.

$head CppAD::vector$$
By default $code CPPAD_CPPADVECTOR$$ is true
and $code CPPAD_TESTVECTOR$$ is defined by the following source code:
$codep */
// The next 3 line are C++ source code.
# if CPPAD_CPPADVECTOR
# define CPPAD_TESTVECTOR(Scalar) CppAD::vector< Scalar >
# endif
/* $$

$head std::vector$$
If you specify $code --with-stdvector$$ on the 
$cref/configure/InstallUnix/Configure/$$
command line during CppAD installation,
$code CPPAD_STDVECTOR$$ is true
and $code CPPAD_TESTVECTOR$$ is defined by the following source code:
$codep */
// The next 4 lines are C++ source code.
# if CPPAD_STDVECTOR
# include <vector>
# define CPPAD_TESTVECTOR(Scalar) std::vector< Scalar >
# endif
/* $$
In this case CppAD will use $code std::vector$$ for 
many lot of its examples and tests.

$head boost::numeric::ublas::vector$$
If you specify $code --with-boostvector$$ on the configure
command line during CppAD installation,
$code CPPAD_BOOSTVECTOR$$ is true 
and $code CPPAD_TESTVECTOR$$ is defined by the following source code:
$codep */
// The next 4 lines are C++ source code.
# if CPPAD_BOOSTVECTOR
# include <boost/numeric/ublas/vector.hpp>
# define CPPAD_TESTVECTOR(Scalar) boost::numeric::ublas::vector< Scalar >
# endif
/* $$
In this case CppAD will use Ublas vectors for many of its examples and tests.

$head Eigen Vectors$$
If you specify $code --with-eigenvector$$ on the configure
command line during CppAD installation,
$code CPPAD_EIGENVECTOR$$ is true 
and $code CPPAD_TESTVECTOR$$ is defined by the following source code:
$codep */
// The next 4 lines are C++ source code.
# if CPPAD_EIGENVECTOR
# include <cppad/example/cppad_eigen.hpp>
# define CPPAD_TESTVECTOR(Scalar) Eigen::Matrix< Scalar , Eigen::Dynamic, 1>
# endif
/* $$
In this case CppAD will use Eigen vectors for many of its examples and tests.


$end
------------------------------------------------------------------------ 
*/

# endif
