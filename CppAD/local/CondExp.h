# ifndef CppADCondExpIncluded
# define CppADCondExpIncluded

// BEGIN SHORT COPYRIGHT
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
// END SHORT COPYRIGHT

/*
-------------------------------------------------------------------------------
$begin CondExp$$
$spell
	CondExp
	Taylor
	std
	Cpp
	namespace
	inline
	const
	abs
$$

$index conditional, expression$$
$index expression, conditional$$
$index assign, conditional$$
$section The Conditional Expression Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%inline %Type% CondExp (
	const %Type% &%exp1%, const %Type%> &%exp2%, const %Type% &%exp3% )%$$
$tend

$fend 20$$

$head Description$$
Returns a object $italic result$$ that is given by
$syntax%
	if( %exp1% > %Base%(0) )
		%result% = %exp2%
	else	%result% = %exp3%
%$$
where $italic Type$$ is $code float$$ or $code double$$
or $syntax%AD<%Base%>%$$ where $italic Base$$ is a valid
type for conditional expressions.
If $italic Type$$ is $syntax%AD<%Base%>%$$, 
conditional expressions are taped and transferred to the corresponding
$xref/ADFun/$$ object $italic f$$.
The choice of expression values is made each time
$xref/Forward//f.Forward/$$ is used to evaluate the zero order Taylor
coefficients.
(See $xref/Discrete/$$ for another type of taped evaluation). 


$head Example$$
$children%
	Example/CondExp.cpp
%$$
The file
$xref/CondExp.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

inline float CondExp(const float &exp1, const float &exp2, const float &exp3)
{	float result;
	if( exp1 > 0. )
		result = exp2;
	else	result = exp3;
	return result;
}

inline double CondExp(
	const double &exp1, const double &exp2, const double &exp3)
{	double result;
	if( exp1 > 0. )
		result = exp2;
	else	result = exp3;
	return result;
}

inline std::complex<float> CondExp(
	const std::complex<float> &exp1,
	const std::complex<float> &exp2,
	const std::complex<float> &exp3 )
{	CppADUsageError(
		0,
		"Error in CondExp: cannot use CondExp with a complex type"
	);
	return std::complex<float>(0);
}

inline std::complex<double> CondExp(
	const std::complex<double> &exp1,
	const std::complex<double> &exp2,
	const std::complex<double> &exp3 )
{	CppADUsageError(
		0,
		"Error in CondExp: cannot use CondExp with a complex type"
	);
	return std::complex<double>(0);
}
	

template <class Base>
inline AD<Base> CondExp (
	const AD<Base> &exp1, const AD<Base> &exp2, const AD<Base> &exp3
)
{
	AD<Base> result;
	CppADUnknownError( result.id == 0 );

	// check first case where do not need to tape
	if( IdenticalPar(exp1) )
	{	if( exp1 > Base(0) )
			result = exp2;
		else	result = exp3;
		return result;
	}

	// must use CondExp incase Base is an AD type and recording
	result.value = CondExp(exp1.value, exp2.value, exp3.value);

	// second case where do not need to tape this operation
	if( AD<Base>::Tape()->State() == Empty ) 
		return result;

	// third case where we do not need to tape this operation
	if( Parameter(exp1) & Parameter(exp2) & Parameter(exp3) )
		return result;
	
	// add this operation to the tape
	AD<Base>::Tape()->RecordCondExp(result, exp1, exp2, exp3);

	return result;
}

template <class Base>
void ADTape<Base>::RecordCondExp(
	AD<Base>       &z,
	const AD<Base> &exp1,
	const AD<Base> &exp2,
	const AD<Base> &exp3
)
{	size_t   op_taddr;
	size_t   ind0, ind1, ind2;
	size_t   z_taddr;
	OpCode   op;

	static OpCode List[] = {
		CEpppOp,
		CEppvOp,
		CEpvpOp,
		CEpvvOp,
		CEvppOp,
		CEvpvOp,
		CEvvpOp,
		CEvvvOp
	};
	op_taddr = 0;
	if( Variable(exp1) )
		op_taddr += 4;
	if( Variable(exp2) )
		op_taddr += 2;
	if( Variable(exp3) )
		op_taddr += 1;

	CppADUnknownError( 0 < op_taddr && op_taddr < 8 ); 
	op       = List[op_taddr];

	// taddr of this variable
	z_taddr = Rec.PutOp(op);

	// Make sure z is in the list of variables and set its taddr
	if( Parameter(z) )
		z.MakeVariable( z_taddr );
	else	z.taddr = z_taddr;

	// ind for exp1
	if( Parameter(exp1) )
		ind0 = Rec.PutPar(exp1.value);
	else	ind0 = exp1.taddr;	

	// ind for exp2
	if( Parameter(exp2) )
		ind1 = Rec.PutPar(exp2.value);
	else	ind1 = exp2.taddr;	

	// ind for exp3
	if( Parameter(exp3) )
		ind2 = Rec.PutPar(exp3.value);
	else	ind2 = exp3.taddr;	

	CppADUnknownError( NumInd(op) == 3 );
	Rec.PutInd(ind0, ind1, ind2);

	// check that z is a dependent variable
	CppADUnknownError( Variable(z) );
}

} // END CppAD namespace

# endif 
