// $Id$
# ifndef CPPAD_CORE_OPTIMIZE_HPP
# define CPPAD_CORE_OPTIMIZE_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin optimize$$
$spell
	enum
	jac
	bool
	Taylor
	var
	CppAD
	cppad
	std
	CondExpEq
	const
	op
$$

$section Optimize an ADFun Object Tape$$
$mindex sequence operations speed memory NDEBUG$$


$head Syntax$$
$icode%f%.optimize()
%$$
$icode%f%.optimize(%options%)
%$$

$head Purpose$$
The operation sequence corresponding to an $cref ADFun$$ object can
be very large and involve many operations; see the
size functions in $cref seq_property$$.
The $icode%f%.optimize%$$ procedure reduces the number of operations,
and thereby the time and the memory, required to
compute function and derivative values.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$

$head options$$
This argument has prototype
$codei%
	const std::string& %options%
%$$

$subhead conditional_skip$$
If the sub-string $code conditional_skip$$ appears in $icode options$$,
conditional skip operations will be generated; $cref number_skip$$.
This may make the optimize routine use significantly more memory
and take significantly more time to optimize $icode f$$.
On the other, it may save a significant amount of time when
using $icode f$$ for $cref forward$$ or $cref reverse$$ mode calculations.
The default for $icode options$$ (when it is not present)
includes $code conditional_skip$$.

$subhead compare_op$$
If the sub-string $code compare_op$$ appears in $icode options$$,
comparison operators will be left in the optimized function.
These operators are necessary for the
$cref compare_change$$ functions to be meaningful.
On the other hand, they are not necessary, and take extra time,
when the compare_change functions are not used.
The default for $icode options$$ (when it is not present)
includes $code compare_op$$.


$head Improvements$$
You can see the reduction in number of variables in the operation sequence
by calling the function $cref/f.size_var()/seq_property/size_var/$$
before and after the optimization procedure.
Given that the optimization procedure takes time,
it may be faster to skip this optimize procedure and just compute
derivatives using the original operation sequence.

$subhead Testing$$
You can run the CppAD $cref/speed/speed_main/$$ tests and see
the corresponding changes in number of variables and execution time;
see $cref cmake_check$$.

$head Efficiency$$
The $code optimize$$ member function
may greatly reduce the number of variables
in the operation sequence; see $cref/size_var/seq_property/size_var/$$.
If a $cref/zero order forward/forward_zero/$$ calculation is done during
the construction of $icode f$$, it will require more memory
and time than required after the optimization procedure.
In addition, it will need to be redone.
For this reason, it is more efficient to use
$codei%
	ADFun<%Base%> %f%;
	%f%.Dependent(%x%, %y%);
	%f%.optimize();
%$$
instead of
$codei%
	ADFun<%Base%> %f%(%x%, %y%)
	%f%.optimize();
%$$
See the discussion about
$cref/sequence constructors/FunConstruct/Sequence Constructor/$$.

$head Atomic Functions$$
There are some subtitle issue with optimized $cref atomic$$ functions
$latex v = g(u)$$:

$subhead rev_sparse_jac$$
The $cref atomic_rev_sparse_jac$$ function is be used to determine
which components of $icode u$$ affect the dependent variables of $icode f$$.
For each atomic operation, the current
$cref/atomic_sparsity/atomic_option/atomic_sparsity/$$ setting is used
to determine if $code pack_sparsity_enum$$, $code bool_sparsity_enum$$,
or $code set_sparsity_enum$$ is used to determine dependency relations
between argument and result variables.

$subhead nan$$
If $icode%u%[%i%]%$$ does not affect the value of
the dependent variables for $icode f$$,
the value of $icode%u%[%i%]%$$ is set to $cref nan$$.


$head Checking Optimization$$
If $cref/NDEBUG/Faq/Speed/NDEBUG/$$ is not defined,
and $cref/f.size_order()/size_order/$$ is greater than zero,
a $cref forward_zero$$ calculation is done using the optimized version
of $icode f$$ and the results are checked to see that they are
the same as before.
If they are not the same, the
$cref ErrorHandler$$ is called with a known error message
related to $icode%f%.optimize()%$$.

$head Example$$
$children%
	example/optimize.cpp
%$$
The file
$cref optimize.cpp$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/
# include <cppad/local/optimize/optimize_run.hpp>
/*!
\file optimize.hpp
Optimize a player object operation sequence
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
Optimize a player object operation sequence

The operation sequence for this object is replaced by one with fewer operations
but the same funcition and derivative values.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type
\a Base.

\param options
\li
If the sub-string "conditional_skip" appears,
conditional skip operations will be generated.
This may make the optimize routine use significantly more memory
and take significantly more time.
\li
If the sub-string "compare_op" appears,
then comparison operators will left in the optimized tape.
These operators are necessary for the compare_change function to be
be meaningful in the resulting recording.
On the other hand, they are not necessary and take extra time
when compare_change is not used.
*/
template <class Base>
void ADFun<Base>::optimize(const std::string& options)
{	// place to store the optimized version of the recording
	local::recorder<Base> rec;

	// number of independent variables
	size_t n = ind_taddr_.size();

# ifndef NDEBUG
	size_t i, j, m = dep_taddr_.size();
	CppAD::vector<Base> x(n), y(m), check(m);
	Base max_taylor(0);
	bool check_zero_order = num_order_taylor_ > 0;
	if( check_zero_order )
	{	// zero order coefficients for independent vars
		for(j = 0; j < n; j++)
		{	CPPAD_ASSERT_UNKNOWN( play_.GetOp(j+1) == local::InvOp );
			CPPAD_ASSERT_UNKNOWN( ind_taddr_[j]    == j+1   );
			x[j] = taylor_[ ind_taddr_[j] * cap_order_taylor_ + 0];
		}
		// zero order coefficients for dependent vars
		for(i = 0; i < m; i++)
		{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_  );
			y[i] = taylor_[ dep_taddr_[i] * cap_order_taylor_ + 0];
		}
		// maximum zero order coefficient not counting BeginOp at beginning
		// (which is correpsonds to uninitialized memory).
		for(i = 1; i < num_var_tape_; i++)
		{	if(  abs_geq(taylor_[i*cap_order_taylor_+0] , max_taylor) )
				max_taylor = taylor_[i*cap_order_taylor_+0];
		}
	}
# endif

	// create the optimized recording
	local::optimize::optimize_run<Base>(options, n, dep_taddr_, &play_, &rec);

	// number of variables in the recording
	num_var_tape_  = rec.num_var_rec();

	// now replace the recording
	play_.get(rec);

	// set flag so this function knows it has been optimized
	has_been_optimized_ = true;

	// free memory allocated for sparse Jacobian calculation
	// (the results are no longer valid)
	for_jac_sparse_pack_.resize(0, 0);
	for_jac_sparse_set_.resize(0,0);

	// free old Taylor coefficient memory
	taylor_.free();
	num_order_taylor_     = 0;
	cap_order_taylor_     = 0;

	// resize and initilaize conditional skip vector
	// (must use player size because it now has the recoreder information)
	cskip_op_.erase();
	cskip_op_.extend( play_.num_op_rec() );

# ifndef NDEBUG
	if( check_zero_order )
	{
		// zero order forward calculation using new operation sequence
		check = Forward(0, x);

		// check results
		Base eps = 10. * CppAD::numeric_limits<Base>::epsilon();
		for(i = 0; i < m; i++) CPPAD_ASSERT_KNOWN(
			abs_geq( eps * max_taylor , check[i] - y[i] ) ,
			"Error during check of f.optimize()."
		);

		// Erase memory that this calculation was done so NDEBUG gives
		// same final state for this object (from users perspective)
		num_order_taylor_     = 0;
	}
# endif
}

} // END_CPPAD_NAMESPACE
# endif
