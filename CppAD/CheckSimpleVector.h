# ifndef CppADCheckSimpleVectorIncluded
# define CppADCheckSimpleVectorIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
/*
$begin CheckSimpleVector$$
$spell
	CppAD
$$

$section Check Simple Vector Concept$$

$index simple, vector check$$
$index vector, simple check$$
$index check, simple vector$$
$index concept, check simple vector$$

$table
$bold Syntax$$ $cnext
$code # include <CppAD/CheckSimpleVector.h>$$
$rnext $cnext
$syntax%CheckSimpleVector<%Scalar%, %Vector%>()%$$
$tend

$fend 20$$

$head Description$$
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
The type $italic Scalar$$ supports the syntax
$syntax%
	%Scalar%(%i%) == %Scalar%(%j%)%
%$$
where $italic i$$ and $italic j$$ are either $code 0$$ or $code 1$$.
In addition, this syntax returns the value true (false) if and only if
$italic i$$ equals $italic j$$.
($italic i$$ does not equal $italic j$$).


$head Include$$
The file $code CppAD/CheckSimpleVector.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest
if the CppAD include files.

$head Example$$
$children%
	Example/CheckSimpleVector.cpp
%$$
The file $xref/CheckSimpleVector.cpp/$$
contains an example and a test of this function where $italic S$$
is the same as $italic T$$.
It returns true, if it succeeds an false otherwise.
The comments in this example suggest a way to change the example
so $italic S$$ is not the same as $italic T$$.

$end
---------------------------------------------------------------------------
*/



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
	{	// only need execute once per value Scalar, Vector pair
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
		CppADUsageError(
			d.size() == 0,
			"default construtor result does not have size zero"
		);

		// resize to same size as other vectors in test
		d.resize(1);

		// check sizing constructor
		Vector s(1);

		// check copy constructor
		s[0] = Scalar(0);
		const Vector c(s);
		s[0] = Scalar(1);
		CppADUsageError(
			c[0] == Scalar(0),
			"copy constructor is shallow"
		);

		// vector assignment operator
		s = c;
		s[0] = Scalar(1);
		CppADUsageError(
			c[0] == Scalar(0),
			"assignment operator is shallow"
		);

		// element access, right side const
		// element assignment, left side not const
		d[0] = s[0];
	}
# endif

} // end namespace CppAD

# endif
