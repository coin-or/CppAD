# ifndef CPPAD_DET_BY_LU_INCLUDED 
# define CPPAD_DET_BY_LU_INCLUDED 

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin DetByLu$$
$spell
	hpp
	Leq
	Geq
	typedef
	std
	Lu
	Det
	CppADvector
	namespace
$$

$section Determinant Using Lu Factorization$$
$index determinant, Lu factor$$
$index Lu factor, determinant$$
$index factor, Lu determinant$$

$head Syntax$$
$syntax%# include <speed/det_by_lu.hpp>
%$$
$syntax%DetByLu<%Type%> %Det%(size_t %n%)
%$$
$syntax%%Type% %Det%(CppADvector<%Type%> &%A%)
%$$
$syntax%typedef std::complex<double>     Complex;
%$$
$syntax%typedef CppAD::AD<Complex>     ADComplex;
%$$

$fend 25$$

$head Inclusion$$
The template class $code DetByLu$$ is defined in the $code CppAD$$
namespace by including 
the file $code speed/det_by_lu.hpp$$ 
(relative to the CppAD distribution directory).
It is only intended for example and testing purposes, 
so it is not automatically included by
$xref/cppad//CppAD.h/$$.

$head Constructor$$
The syntax
$syntax%
	DetByLu<%Type%> %Det%(size_t %n%)
%$$
constructs the object $italic Det$$ which can be used for 
evaluating the determinant of $italic n$$ by $italic n$$ matrices
using Lu factorization.

$head Evaluation$$
The syntax
$syntax%
	%Type% %Det%(CppADvector<%Type%> &%A%)
%$$
returns the determinant of $italic A$$ using expansion by minors.

$children%
	speed/example/det_by_lu.cpp
%$$

$head Complex Types$$
The complex data types $code Complex$$ and $code ADComplex$$
are defined by
$syntax%
	typedef std::complex<double>     Complex;
	typedef CppAD::AD<Complex>     ADComplex;
%$$
These are used to specialize the template functions
$code LeqZero$$ and $code AbsGeq$$ so they can be used
by $xref/LuSolve/$$ with the $code Complex$$ and $code ADComplex$$
types.

$head Restrictions$$
This routine assumes that the matrix $italic A$$ is not singular.

$head Example$$
The file
$xref/DetByLu.cpp/$$ 
contains an example and test of $code det_by_lu.hpp$$.
It returns true if it succeeds and false otherwise.

$head Source Code$$
The file
$xref/DetByLu.h/$$ 
contains the source for this template function.


$end
---------------------------------------------------------------------------
$begin DetByLu.h$$
$spell
	Lu
	Cpp
	ifndef
	endif
	Det
	const
$$

$index DetByLu$$
$section Determinant using Lu Factorization: Source Code$$
$index complex, Lu factor$$
$index Lu factor, complex$$
$index factor, Lu complex$$

$code
# ifndef CPPAD_DET_BY_LU_INCLUDED
$pre
$$
# define CPPAD_DET_BY_LU_INCLUDED

$verbatim%speed/det_by_lu.hpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$

# endif
$$

$end
---------------------------------------------------------------------------

*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <complex>

typedef std::complex<double>     Complex;
typedef CppAD::AD<Complex>     ADComplex;


// BEGIN CppAD namespace
namespace CppAD {

// specializatgion of LeqZero and AbsGeq for ADComplex case
CppADCreateUnaryBool(Complex,  LeqZero )
CppADCreateBinaryBool(Complex, AbsGeq )

template <class Type>
class DetByLu {
public:
	DetByLu(size_t n_) : m(0), n(n_), A(n_ * n_)
	{	}

	inline Type operator()(const CppADvector<Type> &x)
	{
		using CppAD::exp;

		Type         logdet;
		Type         det;
		int          signdet;
		size_t       i;

		// copy matrix so it is not overwritten
		for(i = 0; i < n * n; i++)
			A[i] = x[i];
 
		// comput log determinant
		signdet = CppAD::LuSolve(
			n, m, A, B, X, logdet);

		// make sure the martix is not singular
		CppADUsageError( 
			signdet != 0,
			"DetByLu: matrix is singular"
		);

		// convert to determinant
		det     = Type( signdet ) * exp( logdet ); 

		// FADBAD requires tempories to be set to constants
		for(i = 0; i < n * n; i++)
			A[i] = 0;

		return det;
	}
private:
	const size_t m;
	const size_t n;
	CppADvector<Type> A;
	CppADvector<Type> B;
	CppADvector<Type> X;
};


} // END CppAD namespace

// END PROGRAM
# endif
