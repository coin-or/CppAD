# ifndef CPPAD_CORE_CHECKPOINT_HPP
# define CPPAD_CORE_CHECKPOINT_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/local/sparse_list.hpp>
# include <cppad/local/sparse_pack.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file checkpoint.hpp
defining checkpoint functions.
*/

/*
$begin checkpoint$$
$spell
	sv
	var
	cppad.hpp
	CppAD
	checkpoint
	checkpointing
	algo
	atom_fun
	const
	enum
	bool
	recomputed
$$

$section Checkpointing Functions$$

$head Syntax$$
$codei%checkpoint<%Base%> %atom_fun%(
	%name%, %algo%, %ax%, %ay%, %sparsity%, %optimize%
)
%sv% = %atom_fun%.size_var()
%atom_fun%.option(%option_value%)
%algo%(%ax%, %ay%)
%atom_fun%(%ax%, %ay%)
checkpoint<%Base%>::clear()%$$

$head See Also$$
$cref reverse_checkpoint.cpp$$

$head Purpose$$

$subhead Reduce Memory$$
You can reduce the size of the tape and memory required for AD by
checkpointing functions of the form $latex y = f(x)$$ where
$latex f : B^n \rightarrow B^m$$.

$subhead Faster Recording$$
It may also reduce the time to make a recording the same function
for different values of the independent variable.
Note that the operation sequence for a recording that uses $latex f(x)$$
may depend on its independent variables.

$subhead Repeating Forward$$
Normally, CppAD store $cref forward$$ mode results until they freed
using $cref capacity_order$$ or the corresponding $cref ADFun$$ object is
deleted. This is not true for checkpoint functions because a checkpoint
function may be used repeatedly with different arguments in the same tape.
Thus, forward mode results are recomputed each time a checkpoint function
is used during a forward or reverse mode sweep.

$subhead Restriction$$
The $cref/operation sequence/glossary/Operation/Sequence/$$
representing $latex f(x)$$ cannot depend on the value of $latex x$$.
The approach in the $cref reverse_checkpoint.cpp$$ example case be applied
when the operation sequence depends on $latex x$$.

$subhead Multiple Level AD$$
If $icode Base$$ is an AD type, it is possible to record $icode Base$$
operations.
Note that $icode atom_fun$$ will treat $icode algo$$ as an atomic
operation while recording $codei%AD%<%Base%>%$$ operations, but not while
recording $icode Base$$ operations.
See the $cref atomic_mul_level.cpp$$ example.


$head Method$$
The $code checkpoint$$ class is derived from $code atomic_base$$
and makes this easy.
It implements all the $code atomic_base$$
$cref/virtual functions/atomic_base/Virtual Functions/$$
and hence its source code $code cppad/core/checkpoint.hpp$$
provides an example implementation of $cref atomic_base$$.
The difference is that $code checkpoint.hpp$$ uses AD
instead of user provided derivatives.

$head constructor$$
The syntax for the checkpoint constructor is
$codei%
	checkpoint<%Base%> %atom_fun%(%name%, %algo%, %ax%, %ay%)
%$$
$list number$$
This constructor cannot be called in $cref/parallel/ta_in_parallel/$$ mode.
$lnext
You cannot currently be recording
$codei%AD<%Base%>%$$ operations when the constructor is called.
$lnext
This object $icode atom_fun$$ must not be destructed for as long
as any $codei%ADFun<%Base%>%$$ object uses its atomic operation.
$lnext
This class is implemented as a derived class of
$cref/atomic_base/atomic_ctor/atomic_base/$$ and hence
some of its error message will refer to $code atomic_base$$.
$lend

$head Base$$
The type $icode Base$$ specifies the base type for AD operations.

$head ADVector$$
The type $icode ADVector$$ must be a
$cref/simple vector class/SimpleVector/$$ with elements of type
$codei%AD<%Base%>%$$.

$head name$$
This $icode checkpoint$$ constructor argument has prototype
$codei%
	const char* %name%
%$$
It is the name used for error reporting.
The suggested value for $icode name$$ is $icode atom_fun$$; i.e.,
the same name as used for the object being constructed.

$head ax$$
This argument has prototype
$codei%
	const %ADVector%& %ax%
%$$
and size must be equal to $icode n$$.
It specifies vector $latex x \in B^n$$
at which an $codei%AD<%Base%>%$$ version of
$latex y = f(x)$$ is to be evaluated.

$head ay$$
This argument has prototype
$codei%
	%ADVector%& %ay%
%$$
Its input size must be equal to $icode m$$ and does not change.
The input values of its elements do not matter.
Upon return, it is an $codei%AD<%Base%>%$$ version of
$latex y = f(x)$$.

$head sparsity$$
This argument has prototype
$codei%
	atomic_base<%Base%>::option_enum %sparsity%
%$$
It specifies $cref/sparsity/atomic_ctor/atomic_base/sparsity/$$
in the $code atomic_base$$ constructor and must be either
$codei%atomic_base<%Base%>::pack_sparsity_enum%$$,
$codei%atomic_base<%Base%>::bool_sparsity_enum%$$, or
$codei%atomic_base<%Base%>::set_sparsity_enum%$$.
This argument is optional and its default value is unspecified.

$head optimize$$
This argument has prototype
$codei%
	bool %optimize%
%$$
It specifies if the recording corresponding to the atomic function
should be $cref/optimized/optimize/$$.
One expects to use a checkpoint function many times, so it should
be worth the time to optimize its operation sequence.
For debugging purposes, it may be useful to use the
original operation sequence (before optimization)
because it corresponds more closely to $icode algo$$.
This argument is optional and its default value is true.


$head size_var$$
This $code size_var$$ member function return value has prototype
$codei%
	size_t %sv%
%$$
It is the $cref/size_var/seq_property/size_var/$$ for the
$codei%ADFun<%Base%>%$$ object is used to store the operation sequence
corresponding to $icode algo$$.

$head option$$
The $code option$$ syntax can be used to set the type of sparsity
pattern used by $icode atom_fun$$.
This is an $codei%atomic_base<%Base%>%$$ function and its documentation
can be found at $cref atomic_option$$.

$head algo$$
The type of $icode algo$$ is arbitrary, except for the fact that
the syntax
$codei%
	%algo%(%ax%, %ay%)
%$$
must evaluate the function $latex y = f(x)$$ using
$codei%AD<%Base%>%$$ operations.
In addition, we assume that the
$cref/operation sequence/glossary/Operation/Sequence/$$
does not depend on the value of $icode ax$$.

$head atom_fun$$
Given $icode ax$$ it computes the corresponding value of $icode ay$$
using the operation sequence corresponding to $icode algo$$.
If $codei%AD<%Base%>%$$ operations are being recorded,
it enters the computation as single operation in the recording
see $cref/start recording/Independent/Start Recording/$$.
(Currently each use of $icode atom_fun$$ actually corresponds to
$icode%m%+%n%+2%$$ operations and creates $icode m$$ new variables,
but this is not part of the CppAD specifications and my change.)

$head clear$$
The $code atomic_base$$ class holds onto static work space in order to
increase speed by avoiding system memory allocation calls.
This call makes to work space $cref/available/ta_available/$$ to
for other uses by the same thread.
This should be called when you are done using the
user atomic functions for a specific value of $icode Base$$.

$subhead Restriction$$
The $code clear$$ routine cannot be called
while in $cref/parallel/ta_in_parallel/$$ execution mode.

$children%example/atomic/checkpoint.cpp
	%example/atomic/mul_level.cpp
	%example/atomic/ode.cpp
	%example/atomic/extended_ode.cpp
%$$
$head Example$$
The file $cref checkpoint.cpp$$ contains an example and test
of these operations.
It returns true if it succeeds and false if it fails.

$end
*/

# define CPPAD_MULTI_THREAD_TMB 0
# ifdef _OPENMP
# ifdef CPPAD_FOR_TMB
# undef  CPPAD_MULTI_THREAD_TMB
# define CPPAD_MULTI_THREAD_TMB 1
# endif
# endif

// ============================================================================
# if ! CPPAD_MULTI_THREAD_TMB
// ============================================================================

template <class Base>
class checkpoint : public atomic_base<Base> {
// ---------------------------------------------------------------------------
private:
	/// same as option_enum in base class
	typedef typename atomic_base<Base>::option_enum option_enum;
	//
	/// AD function corresponding to this checkpoint object
	ADFun<Base> f_;
	//
	/// sparsity for entire Jacobian f(x)^{(1)} does not change so can cache it
	local::sparse_list         jac_sparse_set_;
	vectorBool                 jac_sparse_bool_;
	//
	/// sparsity for sum_i f_i(x)^{(2)} does not change so can cache it
	local::sparse_list         hes_sparse_set_;
	vectorBool                 hes_sparse_bool_;
	// ------------------------------------------------------------------------
	option_enum sparsity(void)
	{	return static_cast< atomic_base<Base>* >(this)->sparsity(); }
	// ------------------------------------------------------------------------
	/// set jac_sparse_set_
	void set_jac_sparse_set(void);
	/// set jac_sparse_bool_
	void set_jac_sparse_bool(void);
	// ------------------------------------------------------------------------
	/// set hes_sparse_set_
	void set_hes_sparse_set(void);
	/// set hes_sparse_bool_
	void set_hes_sparse_bool(void);
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to forward sparse Jacobian pack and bool

	\copydetails atomic_base::for_sparse_jac
	*/
	template <class sparsity_type>
	bool for_sparse_jac(
		size_t                                  q  ,
		const sparsity_type&                    r  ,
		      sparsity_type&                    s  ,
		const vector<Base>&                     x
	);
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to reverse sparse Jacobian pack and bool

	\copydetails atomic_base::rev_sparse_jac
	*/
	template <class sparsity_type>
	bool rev_sparse_jac(
		size_t                                  q  ,
		const sparsity_type&                    rt ,
		      sparsity_type&                    st ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to reverse sparse Hessian  bools

	\copydetails atomic_base::rev_sparse_hes
	*/
	template <class sparsity_type>
	bool rev_sparse_hes(
		const vector<bool>&                     vx ,
		const vector<bool>&                     s  ,
		      vector<bool>&                     t  ,
		size_t                                  q  ,
		const sparsity_type&                    r  ,
		const sparsity_type&                    u  ,
		      sparsity_type&                    v  ,
		const vector<Base>&                     x
	);
public:
	// ------------------------------------------------------------------------
	/*!
	Constructor of a checkpoint object

	\param name [in]
	is the user's name for the AD version of this atomic operation.

	\param algo [in/out]
	user routine that compute AD function values
	(not const because state may change during evaluation).

	\param ax [in]
	argument value where algo operation sequence is taped.

	\param ay [out]
	function value at specified argument value.

	\param sparsity [in]
	what type of sparsity patterns are computed by this function,
	pack_sparsity_enum bool_sparsity_enum, or set_sparsity_enum.
	The default value is unspecified.

	\param optimize [in]
	should the operation sequence corresponding to the algo be optimized.
	The default value is true, but it is
	sometimes useful to use false for debugging purposes.
	*/
	template <class Algo, class ADVector>
	checkpoint(
		const char*                    name            ,
		Algo&                          algo            ,
		const ADVector&                ax              ,
		ADVector&                      ay              ,
		option_enum                    sparsity =
				atomic_base<Base>::pack_sparsity_enum  ,
		bool                           optimize = true
	) : atomic_base<Base>(name, sparsity)
	{	CheckSimpleVector< CppAD::AD<Base> , ADVector>();

		// make a copy of ax because Independent modifies AD information
		ADVector x_tmp(ax);
		// delcare x_tmp as the independent variables
		Independent(x_tmp);
		// record mapping from x_tmp to ay
		algo(x_tmp, ay);
		// create function f_ : x -> y
		f_.Dependent(ay);
		if( optimize )
		{	// suppress checking for nan in f_ results
			// (see optimize documentation for atomic functions)
			f_.check_for_nan(false);
			//
			// now optimize
			f_.optimize();
		}
		// now disable checking of comparison operations
		// 2DO: add a debugging mode that checks for changes and aborts
		f_.compare_change_count(0);
	}
	// ------------------------------------------------------------------------
	/*!
	Implement the user call to <tt>atom_fun.size_var()</tt>.
	*/
	size_t size_var(void)
	{	return f_.size_var(); }
	// ------------------------------------------------------------------------
	/*!
	Implement the user call to <tt>atom_fun(ax, ay)</tt>.

	\tparam ADVector
	A simple vector class with elements of type <code>AD<Base></code>.

	\param id
	optional parameter which must be zero if present.

	\param ax
	is the argument vector for this call,
	<tt>ax.size()</tt> determines the number of arguments.

	\param ay
	is the result vector for this call,
	<tt>ay.size()</tt> determines the number of results.
	*/
	template <class ADVector>
	void operator()(const ADVector& ax, ADVector& ay, size_t id = 0)
	{	CPPAD_ASSERT_KNOWN(
			id == 0,
			"checkpoint: id is non-zero in atom_fun(ax, ay, id)"
		);
		this->atomic_base<Base>::operator()(ax, ay, id);
	}
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to forward mode

	\copydetails atomic_base::forward
	*/
	virtual bool forward(
		size_t                    p ,
		size_t                    q ,
		const vector<bool>&      vx ,
		      vector<bool>&      vy ,
		const vector<Base>&      tx ,
		      vector<Base>&      ty )
	{	size_t n = f_.Domain();
		size_t m = f_.Range();
		//
		CPPAD_ASSERT_UNKNOWN( f_.size_var() > 0 );
		CPPAD_ASSERT_UNKNOWN( tx.size() % (q+1) == 0 );
		CPPAD_ASSERT_UNKNOWN( ty.size() % (q+1) == 0 );
		CPPAD_ASSERT_UNKNOWN( n == tx.size() / (q+1) );
		CPPAD_ASSERT_UNKNOWN( m == ty.size() / (q+1) );
		bool ok  = true;
		//
		if( vx.size() == 0 )
		{	// during user forward mode
			if( jac_sparse_set_.n_set() != 0 )
				jac_sparse_set_.resize(0,0);
			if( jac_sparse_bool_.size() != 0 )
				jac_sparse_bool_.clear();
			//
			if( hes_sparse_set_.n_set() != 0 )
				hes_sparse_set_.resize(0,0);
			if( hes_sparse_bool_.size() != 0 )
				hes_sparse_bool_.clear();
		}
		if( vx.size() > 0 )
		{	// need Jacobian sparsity pattern to determine variable relation
			// during user recording using checkpoint functions
			if( sparsity() == atomic_base<Base>::set_sparsity_enum )
			{	if( jac_sparse_set_.n_set() == 0 )
					set_jac_sparse_set();
				CPPAD_ASSERT_UNKNOWN( jac_sparse_set_.n_set() == m );
				CPPAD_ASSERT_UNKNOWN( jac_sparse_set_.end()   == n );
				//
				for(size_t i = 0; i < m; i++)
				{	vy[i] = false;
					local::sparse_list::const_iterator set_itr(
						jac_sparse_set_, i
					);
					size_t j = *set_itr;
					while(j < n )
					{	// y[i] depends on the value of x[j]
						// cast avoid Microsoft warning (should not be needed)
						vy[i] |= static_cast<bool>( vx[j] );
						j = *(++set_itr);
					}
				}
			}
			else
			{	if( jac_sparse_set_.n_set() != 0 )
					jac_sparse_set_.resize(0, 0);
				if( jac_sparse_bool_.size() == 0 )
					set_jac_sparse_bool();
				CPPAD_ASSERT_UNKNOWN( jac_sparse_set_.n_set() == 0 );
				CPPAD_ASSERT_UNKNOWN( jac_sparse_bool_.size() == m * n );
				//
				for(size_t i = 0; i < m; i++)
				{	vy[i] = false;
					for(size_t j = 0; j < n; j++)
					{	if( jac_sparse_bool_[ i * n + j ] )
						{	// y[i] depends on the value of x[j]
							// cast avoid Microsoft warning
							vy[i] |= static_cast<bool>( vx[j] );
						}
					}
				}
			}
		}
		// compute forward results for orders zero through q
		ty = f_.Forward(q, tx);

		// no longer need the Taylor coefficients in f_
		// (have to reconstruct them every time)
		// Hold onto sparsity pattern because it is always good.
		size_t c = 0;
		size_t r = 0;
		f_.capacity_order(c, r);
		return ok;
	}
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to reverse mode

	\copydetails atomic_base::reverse
	*/
	virtual bool reverse(
		size_t                    q  ,
		const vector<Base>&       tx ,
		const vector<Base>&       ty ,
		      vector<Base>&       px ,
		const vector<Base>&       py )
	{
# ifndef NDEBUG
		size_t n = f_.Domain();
		size_t m = f_.Range();
# endif
		CPPAD_ASSERT_UNKNOWN( n == tx.size() / (q+1) );
		CPPAD_ASSERT_UNKNOWN( m == ty.size() / (q+1) );
		CPPAD_ASSERT_UNKNOWN( f_.size_var() > 0 );
		CPPAD_ASSERT_UNKNOWN( tx.size() % (q+1) == 0 );
		CPPAD_ASSERT_UNKNOWN( ty.size() % (q+1) == 0 );
		bool ok  = true;

		// put proper forward mode coefficients in f_
# ifdef NDEBUG
		// compute forward results for orders zero through q
		f_.Forward(q, tx);
# else
		CPPAD_ASSERT_UNKNOWN( px.size() == n * (q+1) );
		CPPAD_ASSERT_UNKNOWN( py.size() == m * (q+1) );
		size_t i, j, k;
		//
		// compute forward results for orders zero through q
		vector<Base> check_ty = f_.Forward(q, tx);
		for(i = 0; i < m; i++)
		{	for(k = 0; k <= q; k++)
			{	j = i * (q+1) + k;
				CPPAD_ASSERT_UNKNOWN( check_ty[j] == ty[j] );
			}
		}
# endif
		// now can run reverse mode
		px = f_.Reverse(q+1, py);

		// no longer need the Taylor coefficients in f_
		// (have to reconstruct them every time)
		size_t c = 0;
		size_t r = 0;
		f_.capacity_order(c, r);
		return ok;
	}
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to forward sparse Jacobian pack

	\copydetails atomic_base::for_sparse_jac
	*/
	virtual bool for_sparse_jac(
		size_t                                  q  ,
		const vectorBool&                       r  ,
		      vectorBool&                       s  ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to forward sparse Jacobian bool

	\copydetails atomic_base::for_sparse_jac
	*/
	virtual bool for_sparse_jac(
		size_t                                  q  ,
		const vector<bool>&                     r  ,
		      vector<bool>&                     s  ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to forward sparse Jacobian sets

	\copydetails atomic_base::for_sparse_jac
	*/
	virtual bool for_sparse_jac(
		size_t                                  q  ,
		const vector< std::set<size_t> >&       r  ,
		      vector< std::set<size_t> >&       s  ,
		const vector<Base>&                     x
	);
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to reverse sparse Jacobian pack

	\copydetails atomic_base::rev_sparse_jac
	*/
	virtual bool rev_sparse_jac(
		size_t                                  q  ,
		const vectorBool&                       rt ,
		      vectorBool&                       st ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to reverse sparse Jacobian bool

	\copydetails atomic_base::rev_sparse_jac
	*/
	virtual bool rev_sparse_jac(
		size_t                                  q  ,
		const vector<bool>&                     rt ,
		      vector<bool>&                     st ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to reverse Jacobian sets

	\copydetails atomic_base::rev_sparse_jac
	*/
	virtual bool rev_sparse_jac(
		size_t                                  q  ,
		const vector< std::set<size_t> >&       rt ,
		      vector< std::set<size_t> >&       st ,
		const vector<Base>&                     x
	);
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to reverse sparse Hessian pack

	\copydetails atomic_base::rev_sparse_hes
	*/
	virtual bool rev_sparse_hes(
		const vector<bool>&                     vx ,
		const vector<bool>&                     s  ,
		      vector<bool>&                     t  ,
		size_t                                  q  ,
		const vectorBool&                       r  ,
		const vectorBool&                       u  ,
		      vectorBool&                       v  ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to reverse sparse Hessian bool

	\copydetails atomic_base::rev_sparse_hes
	*/
	virtual bool rev_sparse_hes(
		const vector<bool>&                     vx ,
		const vector<bool>&                     s  ,
		      vector<bool>&                     t  ,
		size_t                                  q  ,
		const vector<bool>&                     r  ,
		const vector<bool>&                     u  ,
		      vector<bool>&                     v  ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to reverse sparse Hessian sets

	\copydetails atomic_base::rev_sparse_hes
	*/
	virtual bool rev_sparse_hes(
		const vector<bool>&                     vx ,
		const vector<bool>&                     s  ,
		      vector<bool>&                     t  ,
		size_t                                  q  ,
		const vector< std::set<size_t> >&       r  ,
		const vector< std::set<size_t> >&       u  ,
		      vector< std::set<size_t> >&       v  ,
		const vector<Base>&                     x
	);
};
// ============================================================================
# else
// ============================================================================
# define THREAD omp_get_thread_num()
template <class Base>
class checkpoint : public atomic_base<Base> {
// ---------------------------------------------------------------------------
private:
	/// same as option_enum in base class
	typedef typename atomic_base<Base>::option_enum option_enum;
	//
	/// AD function corresponding to this checkpoint object
	vector< ADFun<Base> > f_;
	//
	/// sparsity for entire Jacobian f(x)^{(1)} does not change so can cache it
	vector<local::sparse_list> jac_sparse_set_;
	vector<vectorBool>         jac_sparse_bool_;
	//
	/// sparsity for sum_i f_i(x)^{(2)} does not change so can cache it
	vector<local::sparse_list> hes_sparse_set_;
	vector<vectorBool>         hes_sparse_bool_;
	// ------------------------------------------------------------------------
	option_enum sparsity(void)
	{	return static_cast< atomic_base<Base>* >(this)->sparsity(); }
	// ------------------------------------------------------------------------
	/// set jac_sparse_set_
	void set_jac_sparse_set(void);
	/// set jac_sparse_bool_
	void set_jac_sparse_bool(void);
	// ------------------------------------------------------------------------
	/// set hes_sparse_set_
	void set_hes_sparse_set(void);
	/// set hes_sparse_bool_
	void set_hes_sparse_bool(void);
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to forward sparse Jacobian pack and bool

	\copydetails atomic_base::for_sparse_jac
	*/
	template <class sparsity_type>
	bool for_sparse_jac(
		size_t                                  q  ,
		const sparsity_type&                    r  ,
		      sparsity_type&                    s  ,
		const vector<Base>&                     x
	);
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to reverse sparse Jacobian pack and bool

	\copydetails atomic_base::rev_sparse_jac
	*/
	template <class sparsity_type>
	bool rev_sparse_jac(
		size_t                                  q  ,
		const sparsity_type&                    rt ,
		      sparsity_type&                    st ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to reverse sparse Hessian  bools

	\copydetails atomic_base::rev_sparse_hes
	*/
	template <class sparsity_type>
	bool rev_sparse_hes(
		const vector<bool>&                     vx ,
		const vector<bool>&                     s  ,
		      vector<bool>&                     t  ,
		size_t                                  q  ,
		const sparsity_type&                    r  ,
		const sparsity_type&                    u  ,
		      sparsity_type&                    v  ,
		const vector<Base>&                     x
	);
public:
	// ------------------------------------------------------------------------
	/*!
	Constructor of a checkpoint object

	\param name [in]
	is the user's name for the AD version of this atomic operation.

	\param algo [in/out]
	user routine that compute AD function values
	(not const because state may change during evaluation).

	\param ax [in]
	argument value where algo operation sequence is taped.

	\param ay [out]
	function value at specified argument value.

	\param sparsity [in]
	what type of sparsity patterns are computed by this function,
	pack_sparsity_enum bool_sparsity_enum, or set_sparsity_enum.
	The default value is unspecified.

	\param optimize [in]
	should the operation sequence corresponding to the algo be optimized.
	The default value is true, but it is
	sometimes useful to use false for debugging purposes.
	*/
	template <class Algo, class ADVector>
	checkpoint(
		const char*                    name            ,
		Algo&                          algo            ,
		const ADVector&                ax              ,
		ADVector&                      ay              ,
		option_enum                    sparsity =
				atomic_base<Base>::pack_sparsity_enum  ,
		bool                           optimize = true
	) :
	atomic_base<Base>(name  , sparsity)        ,
	f_( size_t( omp_get_max_threads() ) )                ,
	jac_sparse_set_( size_t( omp_get_max_threads() ) )   ,
	jac_sparse_bool_( size_t( omp_get_max_threads() ) )  ,
	hes_sparse_set_( size_t( omp_get_max_threads() ) )   ,
	hes_sparse_bool_( size_t( omp_get_max_threads() ) )
	{
		CheckSimpleVector< CppAD::AD<Base> , ADVector>();

		// make a copy of ax because Independent modifies AD information
		ADVector x_tmp(ax);
		// delcare x_tmp as the independent variables
		Independent(x_tmp);
		// record mapping from x_tmp to ay
		algo(x_tmp, ay);
		// create function f_ : x -> y
		f_[0].Dependent(ay);
		if( optimize )
		{	// suppress checking for nan in f_ results
			// (see optimize documentation for atomic functions)
			f_[0].check_for_nan(false);
			//
			// now optimize
			f_[0].optimize();
		}
		// now disable checking of comparison operations
		// 2DO: add a debugging mode that checks for changes and aborts
		f_[0].compare_change_count(0);
		//
		// copy for use during multi-threading
		for(int i = 1; i < omp_get_max_threads(); ++i)
			f_[i] = f_[0];
	}
	// ------------------------------------------------------------------------
	/*!
	Implement the user call to <tt>atom_fun.size_var()</tt>.
	*/
	size_t size_var(void)
	{	return f_[THREAD].size_var(); }
	// ------------------------------------------------------------------------
	/*!
	Implement the user call to <tt>atom_fun(ax, ay)</tt>.

	\tparam ADVector
	A simple vector class with elements of type <code>AD<Base></code>.

	\param id
	optional parameter which must be zero if present.

	\param ax
	is the argument vector for this call,
	<tt>ax.size()</tt> determines the number of arguments.

	\param ay
	is the result vector for this call,
	<tt>ay.size()</tt> determines the number of results.
	*/
	template <class ADVector>
	void operator()(const ADVector& ax, ADVector& ay, size_t id = 0)
	{	CPPAD_ASSERT_KNOWN(
			id == 0,
			"checkpoint: id is non-zero in atom_fun(ax, ay, id)"
		);
		this->atomic_base<Base>::operator()(ax, ay, id);
	}
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to forward mode

	\copydetails atomic_base::forward
	*/
	virtual bool forward(
		size_t                    p ,
		size_t                    q ,
		const vector<bool>&      vx ,
		      vector<bool>&      vy ,
		const vector<Base>&      tx ,
		      vector<Base>&      ty )
	{	size_t n = f_[THREAD].Domain();
		size_t m = f_[THREAD].Range();
		//
		CPPAD_ASSERT_UNKNOWN( f_[THREAD].size_var() > 0 );
		CPPAD_ASSERT_UNKNOWN( tx.size() % (q+1) == 0 );
		CPPAD_ASSERT_UNKNOWN( ty.size() % (q+1) == 0 );
		CPPAD_ASSERT_UNKNOWN( n == tx.size() / (q+1) );
		CPPAD_ASSERT_UNKNOWN( m == ty.size() / (q+1) );
		bool ok  = true;
		//
		if( vx.size() == 0 )
		{	// during user forward mode
			if( jac_sparse_set_[THREAD].n_set() != 0 )
				jac_sparse_set_[THREAD].resize(0,0);
			if( jac_sparse_bool_[THREAD].size() != 0 )
				jac_sparse_bool_[THREAD].clear();
			//
			if( hes_sparse_set_[THREAD].n_set() != 0 )
				hes_sparse_set_[THREAD].resize(0,0);
			if( hes_sparse_bool_[THREAD].size() != 0 )
				hes_sparse_bool_[THREAD].clear();
		}
		if( vx.size() > 0 )
		{	// need Jacobian sparsity pattern to determine variable relation
			// during user recording using checkpoint functions
			if( sparsity() == atomic_base<Base>::set_sparsity_enum )
			{	if( jac_sparse_set_[THREAD].n_set() == 0 )
					set_jac_sparse_set();
				CPPAD_ASSERT_UNKNOWN( jac_sparse_set_[THREAD].n_set() == m );
				CPPAD_ASSERT_UNKNOWN( jac_sparse_set_[THREAD].end()   == n );
				//
				for(size_t i = 0; i < m; i++)
				{	vy[i] = false;
					local::sparse_list::const_iterator set_itr(
						jac_sparse_set_[THREAD], i
					);
					size_t j = *set_itr;
					while(j < n )
					{	// y[i] depends on the value of x[j]
						// cast avoid Microsoft warning (should not be needed)
						vy[i] |= static_cast<bool>( vx[j] );
						j = *(++set_itr);
					}
				}
			}
			else
			{	if( jac_sparse_set_[THREAD].n_set() != 0 )
					jac_sparse_set_[THREAD].resize(0, 0);
				if( jac_sparse_bool_[THREAD].size() == 0 )
					set_jac_sparse_bool();
				CPPAD_ASSERT_UNKNOWN( jac_sparse_set_[THREAD].n_set() == 0 );
				CPPAD_ASSERT_UNKNOWN( jac_sparse_bool_[THREAD].size() == m * n );
				//
				for(size_t i = 0; i < m; i++)
				{	vy[i] = false;
					for(size_t j = 0; j < n; j++)
					{	if( jac_sparse_bool_[THREAD][ i * n + j ] )
						{	// y[i] depends on the value of x[j]
							// cast avoid Microsoft warning
							vy[i] |= static_cast<bool>( vx[j] );
						}
					}
				}
			}
		}
		// compute forward results for orders zero through q
		ty = f_[THREAD].Forward(q, tx);

		// no longer need the Taylor coefficients in f_
		// (have to reconstruct them every time)
		// Hold onto sparsity pattern because it is always good.
		size_t c = 0;
		size_t r = 0;
		f_[THREAD].capacity_order(c, r);
		return ok;
	}
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to reverse mode

	\copydetails atomic_base::reverse
	*/
	virtual bool reverse(
		size_t                    q  ,
		const vector<Base>&       tx ,
		const vector<Base>&       ty ,
		      vector<Base>&       px ,
		const vector<Base>&       py )
	{
# ifndef NDEBUG
		size_t n = f_[THREAD].Domain();
		size_t m = f_[THREAD].Range();
# endif
		CPPAD_ASSERT_UNKNOWN( n == tx.size() / (q+1) );
		CPPAD_ASSERT_UNKNOWN( m == ty.size() / (q+1) );
		CPPAD_ASSERT_UNKNOWN( f_[THREAD].size_var() > 0 );
		CPPAD_ASSERT_UNKNOWN( tx.size() % (q+1) == 0 );
		CPPAD_ASSERT_UNKNOWN( ty.size() % (q+1) == 0 );
		bool ok  = true;

		// put proper forward mode coefficients in f_
# ifdef NDEBUG
		// compute forward results for orders zero through q
		f_[THREAD].Forward(q, tx);
# else
		CPPAD_ASSERT_UNKNOWN( px.size() == n * (q+1) );
		CPPAD_ASSERT_UNKNOWN( py.size() == m * (q+1) );
		size_t i, j, k;
		//
		// compute forward results for orders zero through q
		vector<Base> check_ty = f_[THREAD].Forward(q, tx);
		for(i = 0; i < m; i++)
		{	for(k = 0; k <= q; k++)
			{	j = i * (q+1) + k;
				CPPAD_ASSERT_UNKNOWN( check_ty[j] == ty[j] );
			}
		}
# endif
		// now can run reverse mode
		px = f_[THREAD].Reverse(q+1, py);

		// no longer need the Taylor coefficients in f_
		// (have to reconstruct them every time)
		size_t c = 0;
		size_t r = 0;
		f_[THREAD].capacity_order(c, r);
		return ok;
	}
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to forward sparse Jacobian pack

	\copydetails atomic_base::for_sparse_jac
	*/
	virtual bool for_sparse_jac(
		size_t                                  q  ,
		const vectorBool&                       r  ,
		      vectorBool&                       s  ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to forward sparse Jacobian bool

	\copydetails atomic_base::for_sparse_jac
	*/
	virtual bool for_sparse_jac(
		size_t                                  q  ,
		const vector<bool>&                     r  ,
		      vector<bool>&                     s  ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to forward sparse Jacobian sets

	\copydetails atomic_base::for_sparse_jac
	*/
	virtual bool for_sparse_jac(
		size_t                                  q  ,
		const vector< std::set<size_t> >&       r  ,
		      vector< std::set<size_t> >&       s  ,
		const vector<Base>&                     x
	);
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to reverse sparse Jacobian pack

	\copydetails atomic_base::rev_sparse_jac
	*/
	virtual bool rev_sparse_jac(
		size_t                                  q  ,
		const vectorBool&                       rt ,
		      vectorBool&                       st ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to reverse sparse Jacobian bool

	\copydetails atomic_base::rev_sparse_jac
	*/
	virtual bool rev_sparse_jac(
		size_t                                  q  ,
		const vector<bool>&                     rt ,
		      vector<bool>&                     st ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to reverse Jacobian sets

	\copydetails atomic_base::rev_sparse_jac
	*/
	virtual bool rev_sparse_jac(
		size_t                                  q  ,
		const vector< std::set<size_t> >&       rt ,
		      vector< std::set<size_t> >&       st ,
		const vector<Base>&                     x
	);
	// ------------------------------------------------------------------------
	/*!
	Link from user_atomic to reverse sparse Hessian pack

	\copydetails atomic_base::rev_sparse_hes
	*/
	virtual bool rev_sparse_hes(
		const vector<bool>&                     vx ,
		const vector<bool>&                     s  ,
		      vector<bool>&                     t  ,
		size_t                                  q  ,
		const vectorBool&                       r  ,
		const vectorBool&                       u  ,
		      vectorBool&                       v  ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to reverse sparse Hessian bool

	\copydetails atomic_base::rev_sparse_hes
	*/
	virtual bool rev_sparse_hes(
		const vector<bool>&                     vx ,
		const vector<bool>&                     s  ,
		      vector<bool>&                     t  ,
		size_t                                  q  ,
		const vector<bool>&                     r  ,
		const vector<bool>&                     u  ,
		      vector<bool>&                     v  ,
		const vector<Base>&                     x
	);
	/*!
	Link from user_atomic to reverse sparse Hessian sets

	\copydetails atomic_base::rev_sparse_hes
	*/
	virtual bool rev_sparse_hes(
		const vector<bool>&                     vx ,
		const vector<bool>&                     s  ,
		      vector<bool>&                     t  ,
		size_t                                  q  ,
		const vector< std::set<size_t> >&       r  ,
		const vector< std::set<size_t> >&       u  ,
		      vector< std::set<size_t> >&       v  ,
		const vector<Base>&                     x
	);
};
# undef THREAD
// ============================================================================
# endif
// ============================================================================

} // END_CPPAD_NAMESPACE

// functions implemented in cppad/core/checkpoint files
# include <cppad/core/checkpoint/rev_sparse_hes.hpp>
# include <cppad/core/checkpoint/rev_sparse_jac.hpp>
# include <cppad/core/checkpoint/for_sparse_jac.hpp>
# include <cppad/core/checkpoint/set_hes_sparse_bool.hpp>
# include <cppad/core/checkpoint/set_hes_sparse_set.hpp>
# include <cppad/core/checkpoint/set_jac_sparse_bool.hpp>
# include <cppad/core/checkpoint/set_jac_sparse_set.hpp>

# undef CPPAD_MULTI_THREAD_TMB
# endif
