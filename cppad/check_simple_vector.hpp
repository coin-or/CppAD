# ifndef CPPAD_CHECK_SIMPLE_VECTOR_INCLUDED
# define CPPAD_CHECK_SIMPLE_VECTOR_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin CheckSimpleVector$$
$spell
	cppad.hpp
	CppAD
$$

$section Check Simple Vector Concept$$

$index simple, vector check$$
$index vector, simple check$$
$index check, simple vector$$
$index concept, check simple vector$$

$head Syntax$$
$code # include <cppad/check_simple_vector.hpp>$$
$pre
$$
$syntax%CheckSimpleVector<%Scalar%, %Vector%>()%$$


$head Purpose$$
The syntax 
$syntax%
	CheckSimpleVector<%Scalar%, %Vector%>()
%$$
preforms compile and run time checks that the type specified
by $italic Vector$$ satisfies all the requirements for 
a $xref/SimpleVector/$$ class with 
$xref/SimpleVector/Elements of Specified Type/elements of type Scalar/$$.
If a requirement is not satisfied,
a an error message makes it clear what condition is not satisfied.

$head Restrictions$$
The following extra assumption is made by $code CheckSimpleVector$$:
If $italic x$$ is a $italic Scalar$$ object
and $italic i$$ is an $code int$$,
$syntax%
	%x% = %i%
%$$
assigns the object $italic x$$ the value of the
value of $italic i$$.  
If $italic y$$ is another $italic Scalar$$ object,
$syntax%
	%x% = %y%
%$$
assigns the object $italic x$$ the value of $italic y$$.


$head Include$$
The file $code cppad/check_simple_vector.hpp$$ is included by $code cppad/cppad.hpp$$
but it can also be included separately with out the rest
if the CppAD include files.

$head Example$$
$children%
	example/check_simple_vector.cpp
%$$
The file $xref/CheckSimpleVector.cpp/$$
contains an example and test of this function where $italic S$$
is the same as $italic T$$.
It returns true, if it succeeds an false otherwise.
The comments in this example suggest a way to change the example
so $italic S$$ is not the same as $italic T$$.

$end
---------------------------------------------------------------------------
*/

# include <cppad/local/cppad_assert.hpp>

namespace CppAD {

# ifdef NDEBUG
	template <class Scalar, class Vector>
	inline void CheckSimpleVector(void)
	{ }
# else
	template <class S, class T>
	struct ok_if_S_same_as_T { };

	template <class T>
	struct ok_if_S_same_as_T<T,T> { typedef T ok; };

	template <class Scalar, class Vector>
	void CheckSimpleVector(void)
	{	Scalar zero;
		Scalar one;

		// use assignment and not constructor
		zero = 0;
		one  = 1;

		// only need execute once per value Scalar, Vector pair
		static bool runOnce = false;
		if( runOnce )
			return;
		runOnce = true;

		// value_type must be type of elements of Vector
		typedef typename Vector::value_type value_type;

		// check that elements of Vector have type Scalar
		typedef typename ok_if_S_same_as_T<Scalar, value_type>::ok ok;

		// check default constructor
		Vector d;

		// size member function
		CPPAD_ASSERT_KNOWN(
			d.size() == 0,
			"default construtor result does not have size zero"
		);

		// resize to same size as other vectors in test
		d.resize(1);

		// check sizing constructor
		Vector s(1);

		// check element assignment
		s[0] = one;
		CPPAD_ASSERT_KNOWN(
			s[0] == one,
			"element assignment failed"
		);

		// check copy constructor
		s[0] = zero;
		const Vector c(s);
		s[0] = one;
		CPPAD_ASSERT_KNOWN(
			c[0] == zero,
			"copy constructor is shallow"
		);

		// vector assignment operator
		s = c;
		s[0] = one;
		CPPAD_ASSERT_KNOWN(
			c[0] == zero,
			"assignment operator is shallow"
		);

		// element access, right side const
		// element assignment, left side not const
		d[0] = s[0];
	}
# endif

} // end namespace CppAD

# endif
