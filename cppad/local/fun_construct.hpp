# ifndef CPPAD_FUN_CONSTRUCT_INCLUDED
# define CPPAD_FUN_CONSTRUCT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin FunConstruct$$
$spell 
	taylor_
	var
	ADvector
	const
$$

$spell
$$

$section Construct an ADFun Object and Stop Recording$$

$index ADFun, construct$$
$index construct, ADFun$$
$index tape, stop recording$$
$index recording, stop tape$$

$head Syntax$$
$codei%ADFun<%Base%> %f%
%$$
$codei%ADFun<%Base%> %f%(%x%, %y%)
%$$


$head Purpose$$
The $codei%AD<%Base%>%$$ object $icode f$$ can 
store an AD of $icode Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.
It can then be used to calculate derivatives of the corresponding
$xref/glossary/AD Function/AD function/$$
$latex \[
	F : B^n \rightarrow B^m
\] $$
where $latex B$$ is the space corresponding to objects of type $icode Base$$.

$head x$$
If the argument $icode x$$ is present, it has prototype
$codei%
	const %VectorAD% &%x%
%$$
It must be the vector argument in the previous call to
$cref/Independent/$$.
Neither its size, or any of its values, are allowed to change
between calling
$codei%
	Independent(%x%)
%$$
and 
$codei%
	ADFun<%Base%> %f%(%x%, %y%)
%$$.

$head y$$
If the argument $icode y$$ is present, it has prototype
$codei%
	const %VectorAD% &%y%
%$$
The sequence of operations that map $icode x$$
to $icode y$$ are stored in the AD function object $icode f$$.

$head VectorAD$$
The type $icode VectorAD$$ must be a $cref/SimpleVector/$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$ 
$codei%AD<%Base%>%$$.
The routine $cref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Default Constructor$$
The default constructor 
$codei%
	ADFun<%Base%> %f%
%$$
creates an 
$codei%AD<%Base%>%$$ object with no corresponding operation sequence; i.e.,
$codei%
	%f%.size_var()
%$$
returns the value zero (see $xref/SeqProperty/size_var/size_var/$$).

$head Sequence Constructor$$
The sequence constructor 
$codei%
	ADFun<%Base%> %f%(%x%, %y%)
%$$
creates the $codei%AD<%Base%>%$$ object $icode f$$,
stops the recording of AD of $icode Base$$ operations
corresponding to the call
$codei%
	Independent(%x%)
%$$
and stores the corresponding operation sequence in the object $icode f$$.
It then stores the first order taylor_ coefficients 
(corresponding to the value of $icode x$$) in $icode f$$.
This is equivalent to the following steps using the default constructor:
$list number$$
Create $icode f$$ with the default constructor
$codei%
	ADFun<%Base%> %f%;
%$$
$lnext
Stop the tape and storing the operation sequence using
$codei%
	%f%.Dependent(%x%, %y%);
%$$
(see $xref/Dependent/$$).
$lnext
Calculating the first order taylor_ coefficients for all 
the variables in the operation sequence using
$codei%
	%f%.Forward(%p%, %x_p%)
%$$
with $icode p$$ equal to zero and the elements of $icode x_p$$
equal to the corresponding elements of $icode x$$
(see $xref/Forward/$$).
$lend

$head OpenMP$$
$index OpenMP, Dependent$$
$index Dependent, OpenMP$$
$index OpenMP, ADFun$$
$index ADFun, OpenMP$$
In the case of multi-threading with OpenMP,
the call to $code Independent$$
and the corresponding call to
$codei%
	ADFun<%Base%> %f%( %x%, %y%)
%$$
or 
$codei%
	%f%.Dependent( %x%, %y%)
%$$
must be preformed by the same thread.


$head Example$$

$subhead Sequence Constructor$$
The file
$xref/Independent.cpp/$$ 
contains an example and test of the sequence constructor.
It returns true if it succeeds and false otherwise.

$subhead Default Constructor$$
The files
$xref/FunCheck.cpp/$$ 
and
$xref/HesLagrangian.cpp/$$
contain an examples and tests using the default constructor.
They return true if they succeed and false otherwise.

$end
*/


// BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename VectorAD>
ADFun<Base>::ADFun(const VectorAD &x, const VectorAD &y)
: total_num_var_(0), taylor_(CPPAD_NULL), for_jac_(CPPAD_NULL)
{	size_t i, j, m, n;

	CPPAD_ASSERT_KNOWN(
		x.size() > 0,
		"ADFun<Base>: independent variable vector has size zero."
	);
	CPPAD_ASSERT_KNOWN(
		Variable(x[0]),
		"ADFun<Base>: independent variable vector has been changed."
	);
	ADTape<Base> *tape = AD<Base>::tape_ptr(x[0].id_);
	CPPAD_ASSERT_KNOWN(
		tape->size_independent_ == x.size(),
		"ADFun<Base>: independent variable vector has been changed."
	);
# ifndef NDEBUG
	for(j = 0; j < x.size(); j++)
	{	CPPAD_ASSERT_KNOWN(
		x[j].taddr_ == (j+1),
		"ADFun<Base>: independent variable vector has been changed."
		);
		CPPAD_ASSERT_KNOWN(
		x[j].id_ == x[0].id_,
		"ADFun<Base>: independent variable vector has been changed."
		);
	}
	for(i = 0; i < y.size(); i++)
	{	CPPAD_ASSERT_KNOWN(
		CppAD::Parameter( y[i] ) | (y[i].id_ == x[0].id_) ,
		"ADFun<Base>: dependent vector contains variables for"
		"\na different tape than the independent variables."
		);
	}
# endif

	// stop the tape and store the operation sequence
	Dependent(tape, y);

	// allocate memory for one zero order taylor_ coefficient
	taylor_per_var_= 1;
	taylor_col_dim_  = 1;
	taylor_        = CPPAD_TRACK_NEW_VEC(total_num_var_, taylor_);

	// set zero order coefficients corresponding to indpendent variables
	n = ind_taddr_.size();
	CPPAD_ASSERT_UNKNOWN( n == x.size() );
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == (j+1) );
		CPPAD_ASSERT_UNKNOWN( x[j].taddr_  == (j+1) );
		taylor_[ ind_taddr_[j] ]  = x[j].value_;
	}

	// use independent variable values to fill in values for others
	compare_change_ = forward0sweep(
		false, total_num_var_, &play_, taylor_col_dim_, taylor_
	);
	CPPAD_ASSERT_UNKNOWN( compare_change_ == 0 );

	// check the dependent variable values
	m = dep_taddr_.size();
	for(i = 0; i < m; i++) CPPAD_ASSERT_KNOWN(
		taylor_[dep_taddr_[i]] == y[i].value_,
		"An independent variable is not equal its tape evaluation"
		", it may be nan."
	);
}

} // END CppAD namespace

# endif
