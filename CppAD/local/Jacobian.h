# ifndef CppADJacobianIncluded
# define CppADJacobianIncluded

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
$begin Jacobian$$
$spell
	typename
	Taylor
	Jacobian
	DetLu
	const
$$

$index Jacobian$$
$index easy, first derivative driver$$
$index first, derivative easy driver$$
$index driver, easy first derivative$$
$index derivative, first easy$$

$section Jacobian: Easy Driver$$


$table
$bold Syntax$$
$syntax%%J% = %F%.Jacobian(%x%)%$$
$tend

$fend 25$$

$head Description$$
Given an $xref/ADFun//ADFun<Base>/$$ object $latex F : B^n \rightarrow B^m$$,
this routine returns the Jacobian of $italic F$$ at the point 
$italic x$$; i.e.,
$latex \[
	J = F^{(1)} (x)
\] $$

$head F$$
The object $italic F$$ has prototype
$syntax%
	ADFun<%Base%> %F%
%$$

$head VectorBase$$
The type $italic VectorBase$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type Base/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head x$$
The vector $italic x$$ has prototype
$syntax%
	const %VectorBase% &%x%
%$$
It must have length $italic m$$ and specifies
that point at which to evaluate the Jacobian.

$head J$$
The vector $italic J$$ has prototype
$syntax%
	%VectorBase% %J%
%$$
It must have length $syntax%%m% * %n%$$.
After the assignment to $italic J$$, 
for $latex i = 0 , \ldots , m - 1 $$ and $latex j = 0 , \ldots , n - 1$$
$latex \[.
	J[ i * n + j ] = \D{ F_i }{ x_j } ( x )
\] $$


$head Forward$$
The object $italic F$$ stores information related to previous
calls to $syntax%%F%.Forward%$$.
After this operation,
the previous calls to $xref/Forward/$$ are undefined
with the exception of the zero order call which has the form
$syntax%
	%F%.Forward(0, %x%)
%$$.

$head Examples$$
$children%
	Example/Jacobian.cpp
%$$
The routine 
$xref/Jacobian.cpp//Jacobian/$$ is both an example and a test.
It returns $code true$$, if it succeeds and $code false$$ otherwise.


$subhead Determinant Using Expansion by Minors$$
The routine 
$xref/JacMinorDet.cpp//DerMinorDet/$$ is both an example and a test.
It returns $code true$$, if it succeeds and $code false$$ otherwise.

$subhead Determinant Using Lu Factorization$$
The routine 
$xref/JacLuDet.cpp//DerLuDet/$$ is both an example and a test.
It returns $code true$$, if it succeeds and $code false$$ otherwise.

$end
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <typename Base, typename VectorBase>
void JacobianFor(ADFun<Base> &f, const VectorBase &x, VectorBase &J)
{	size_t i;
	size_t j;

	size_t m = f.Domain();
	size_t n = f.Range();

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CppADUnknownError( x.size() == f.Domain() );
	CppADUnknownError( J.size() == f.Range() * f.Domain() );

	// argument and result for forward mode calculations
	VectorBase u(m);
	VectorBase v(n);

	// initialize all the components
	for(j = 0; j < m; j++)
		u[j] = Base(0);
	
	// loop through the different coordinate directions
	for(j = 0; j < m; j++)
	{	// set u to the j-th coordinate direction
		u[j] = Base(1);

		// compute the partial of f w.r.t. this coordinate direction
		v = f.Forward(1, u);

		// reset u to vector of all zeros
		u[j] = Base(0);

		// return the result
		for(i = 0; i < n; i++)
			J[ i * m + j ] = v[i];
	}
}
template <typename Base, typename VectorBase>
void JacobianRev(ADFun<Base> &f, const VectorBase &x, VectorBase &J)
{	size_t i;
	size_t j;

	size_t m = f.Domain();
	size_t n = f.Range();

	CppADUnknownError( x.size() == f.Domain() );
	CppADUnknownError( J.size() == f.Range() * f.Domain() );

	// argument and result for reverse mode calculations
	VectorBase u(m);
	VectorBase v(n);

	// initialize all the components
	for(i = 0; i < n; i++)
		v[i] = Base(0);
	
	// loop through the different coordinate directions
	for(i = 0; i < n; i++)
	{	if( f.Parameter(i) )
		{	// return zero for this component of f
			for(j = 0; j < m; j++)
				J[ i * m + j ] = Base(0);
		}
		else
		{ 
			// set v to the i-th coordinate direction
			v[i] = Base(1);

			// compute the derivative of this component of f
			u = f.Reverse(1, v);

			// reset v to vector of all zeros
			v[i] = Base(0);

			// return the result
			for(j = 0; j < m; j++)
				J[ i * m + j ] = u[j];
		}
	}
}

template <typename Base>
template <typename VectorBase>
VectorBase ADFun<Base>::Jacobian(const VectorBase &x)
{	size_t i;
	size_t m = Domain();
	size_t n = Range();

	CppADUsageError(
		x.size() == m,
		"Jacobian: length of x not equal domain dimension for F"
	); 

	// point at which we are evaluating the Jacobian
	Forward(0, x);

	// work factor for forward mode
	size_t workForward = m; 

	// work factor for reverse mode
	size_t workReverse = 0;
	for(i = 0; i < n; i++)
	{	if( ! Parameter(i) )
			++workReverse;
	}

	// choose the method with the least work
	VectorBase J( n * m );
	if( workForward <= workReverse )
		JacobianFor(*this, x, J);
	else	JacobianRev(*this, x, J);

	return J;
}

} // END CppAD namespace

# endif
