# ifndef CPPAD_IPOPT_CPPAD_NLP_INCLUDED
# define CPPAD_IPOPT_CPPAD_NLP_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ipopt_cppad_nlp$$
$spell
	bool
	doesn't
	nan
	inf
	naninf
	std
	maxiter
	infeasibility
	obj
	const
	optimizer
	ipopt_cppad_nlp.hpp
	fg_info.eval
	retape
	CppAD
	
$$
$section Nonlinear Programming Using Ipopt and CppAD$$

$head Syntax$$
$codei%# include "ipopt_cppad_nlp.hpp"
%$$
$codei%# ipopt_cppad_solution %solution%;
%$$
$codei%ipopt_cppad_nlp %cppad_nlp%(
	%n%, %m%, %x_i%, %x_l%, %x_u%, %g_l%, %g_u%, %fg_info%, &%solution%
)%$$

$head Purpose$$
The class $code ipopt_cppad_nlp$$ is used to solve nonlinear programming
problems of the form
$latex \[
\begin{array}{rll}
{\rm minimize}      & f(x) 
\\
{\rm subject \; to} & g^l \leq g(x) \leq g^u
\\
                    & x^l  \leq x   \leq x^u
\end{array}
\] $$
This is done using 
$href%
	https://www.coin-or.org/projects/Ipopt%
	Ipopt
%$$
optimizer and 
$href%
	http://www.coin-or.org/CppAD/%
	CppAD
%$$
Algorithmic Differentiation package.

$head Warning$$
This is only and example use of CppAD.
It is expected that this class will be improved and that
its user interface may change in ways that are not backward compatible.

$head fg(x)$$
The function $latex fg : \R^n \rightarrow \R^{m+1}$$ is defined by
$latex \[
\begin{array}{rcl}
	fg_0 (x)     & = & f(x)         \\
	fg_1 (x)     & = & g_0 (x)      \\
	             & \vdots &         \\
	fg_m (x)     & = & g_{m-1} (x)
	\end{array}
\] $$

$subhead Index Vector$$
We define an $italic index vector$$ as a vector of non-negative integers
for which none of the values are equal; i.e.,
it is both a vector and a set.
If $latex I$$ is an index vector $latex |I|$$ is used to denote the
number of elements in $latex I$$ and $latex \| I \|$$ is used
to denote the value of the maximum element in $latex I$$.

$subhead Projection$$
Given an index vector $latex J$$ and a positive integer $latex n$$
where $latex n > \| J \|$$, we use $latex J \otimes n$$ for
the mapping $latex J \otimes n : \R^n \rightarrow \R^{|J|}$$ defined by
$latex \[
	J \otimes n (x)_j = x_{J(j)}
\] $$
for $latex j = 0 , \ldots |J| - 1$$.

$subhead Injection$$
Given an index vector $latex I$$ and a positive integer $latex m$$
where $latex m > \| I \|$$, we use $latex m \otimes I$$ for
the mapping $latex m \otimes I: \R^{|I|} \rightarrow \R^m$$ defined by
$latex \[
m \otimes I (y)_i = \left\{ \begin{array}{ll}
y_k & {\rm if} \; i = I(k) \; {\rm for \; some} \; 
	k \in \{ 0 , \cdots, |I|-1 \} 
\\
0   & {\rm otherwise}
\end{array} \right.
\] $$

$subhead Representation$$
In many applications, each of the component functions of $latex fg(x)$$
only depend on a few of the components of $latex x$$.
In this case, expressing $latex fg(x)$$ in terms of simpler functions
with fewer arguments can greatly reduce the amount of work required
to compute its derivatives.
$pre

$$
We use a function $latex r : \R^q \rightarrow \R^p$$ to express our
representation of $latex fg(x)$$ in terms of simpler functions
as follows
$latex \[
fg(x) = \sum_{k=0}^{K-1} \; (m+1) \otimes I_k \; 
	\{  r [ \; J_k \otimes n \; (x) ] \} 
\] $$
where $latex K$$ is a positive integer,
and for each $latex k$$, $latex I_k$$, $latex J_k$$ are index vectors.
Note that for each $latex k$$,
$latex | J_k | = q$$ and $latex | I_k | = p$$. 

$head Simple Representation$$
In the simple representation,
$latex K = 1$$,
$latex q = n$$,
$latex p = m$$,
$latex I_0 = (0 , \ldots , m)$$,
$latex J_0 = (0 , \ldots , n-1)$$,
and $latex r(x) = fg(x)$$.

$head SizeVector$$
The type $codei SizeVector$$ is defined by the 
$codei ipopt_cppad_nlp.hpp$$ include file to be a 
$cref/SimpleVector/$$ class with elements of type
$code size_t$$.

$head NumberVector$$
The type $codei NumberVector$$ is defined by the 
$codei ipopt_cppad_nlp.hpp$$ include file to be a 
$cref/SimpleVector/$$ class with elements of type
$code Ipopt::Number$$.

$head ADNumber$$
The type $codei ADNumber$$ is defined by the 
$codei ipopt_cppad_nlp.hpp$$ include file to be a 
an AD type that can be used to compute derivatives.

$head ADVector$$
The type $codei ADVector$$ is defined by the 
$codei ipopt_cppad_nlp.hpp$$ include file to be a 
$cref/SimpleVector/$$ class with elements of type
$code ADNumber$$. 

$head n$$
The argument $icode n$$ has prototype
$codei%
	size_t %n%
%$$
It specifies the dimension of the argument space; 
i.e., $latex x \in \R^n$$.

$head m$$
The argument $icode m$$ has prototype
$codei%
	size_t %m%
%$$
It specifies the dimension of the range space for $latex g$$; 
i.e., $latex g : \R^n \rightarrow \R^m$$.

$head x_i$$
The argument $icode x_i$$ has prototype
$codei%
	const NumberVector& %x_i%
%$$
and its size is equal to $latex n$$.
It specifies the initial point where Ipopt starts the optimization process.

$head x_l$$
The argument $icode x_l$$ has prototype
$codei%
	const NumberVector& %x_l%
%$$
and its size is equal to $latex n$$.
It specifies the lower limits for the argument in the optimization problem;
i.e., $latex x^l$$.

$head x_u$$
The argument $icode x_u$$ has prototype
$codei%
	const NumberVector& %x_u%
%$$
and its size is equal to $latex n$$.
It specifies the upper limits for the argument in the optimization problem;
i.e., $latex x^u$$.

$head g_l$$
The argument $icode g_l$$ has prototype
$codei%
	const NumberVector& %g_l%
%$$
and its size is equal to $latex m$$.
It specifies the lower limits for the constraints in the optimization problem;
i.e., $latex g^l$$.

$head g_u$$
The argument $icode g_u$$ has prototype
$codei%
	const NumberVector& %g_u%
%$$
and its size is equal to $latex n$$.
It specifies the upper limits for the constraints in the optimization problem;
i.e., $latex g^u$$.

$head fg_info$$
The argument $icode fg_info$$ has prototype
$codei%
	ipopt_cppad_fg_info* %fg_info%
%$$
where the object $codei%*%fg_info%$$ is a member of
a class (referred to as $icode FG_info$$)
that is derived from the base class $code ipopt_cppad_fg_info$$.
Certain virtual member functions of $code ipopt_cppad_fg_info$$ are used to 
compute the value of $latex fg(x)$$.
The specifications for these member functions are given below:

$subhead fg_info->r_eval$$
This member function has the prototype
$codei%
	virtual ADVector ipopt_cppad_fg_info::r_eval(const ADVector& %u%) = 0;
%$$
This prototype is pure virtual and hence it must be defined in the 
derived class $icode FG_info$$.
$pre

$$
This function computes the value of $latex r(u)$$
used in the $cref/representation/ipopt_cppad_nlp/fg(x)/Representation/$$
for $latex fg(x)$$.
If $icode u$$ is an $code ADVector$$ of size $icode q$$ 
and $icode r$$ is an $code ADVector$$ of size $icode p$$
the syntax
$codei%
	%r% = %fg_info%->r_eval(%u%)
%$$
set $icode r$$ to the vector $latex r(u)$$.

$subhead fg_info->retape$$
This member function has the prototype
$codei%
	virtual bool ipopt_cppad_fg_info::retape(void)
%$$
If $icode retape$$ has type $code bool$$,
the syntax
$codei%
        %retape% = %fg_info%->retape()
%$$
sets $icode retape$$ to true or false.
If $icode retape$$ is true, 
$code ipopt_cppad_nlp$$ will retape the operation sequence for
every new $icode x$$ value. 
The program should use much less memory and run faster if $icode retape$$
is false.
You can test both the true and false cases to make sure 
the operation sequence does not depend on $icode x$$.
$pre

$$
The $code ipopt_cppad_fg_info$$ implementation of this function
sets $icode retape$$ to true.

$subhead fg_info->number_sum()$$
This member function has prototype
$codei%
	virtual size_t ipopt_cppad_fg_info::number_sum(void)
%$$
If $icode K$$ has type $code size_t$$, the syntax
$codei%
	%K% = %fg_info%->number_sum()
%$$
sets $icode K$$ to the number of terms in the summation 
used to represent $latex fg(x)$$ in term of $latex r(u)$$.
$pre

$$
The $code ipopt_cppad_fg_info$$ implementation of this function
corresponds to the simple representation mentioned above; i.e.
$icode%K% = 1%$$.

$subhead fg_info->domain_size()$$
This member function has prototype
$codei%
	virtual size_t ipopt_cppad_fg_info::domain_size(void)
%$$
If $icode q$$ has type $code size_t$$, the syntax
$codei%
	%q% = %fg_info%->domain_size()
%$$
sets $icode q$$ to the dimension of the domain space for $latex r(u)$$.
$pre

$$
The $code ipopt_cppad_h_base$$ implementation of this function
corresponds to the simple representation mentioned above; i.e.,
$latex q = n$$.

$subhead fg_info->range_size()$$
This member function has prototype
$codei%
	virtual size_t ipopt_cppad_fg_info::range_size(void)
%$$
If $icode p$$ has type $code size_t$$, the syntax
$codei%
	%p% = %fg_info%->range_size()
%$$
sets $icode p$$ to the dimension of the range space for $latex r(u)$$.
$pre

$$
The $code ipopt_cppad_h_base$$ implementation of this function
corresponds to the simple representation mentioned above; i.e.,
$latex p = m+1$$.

$subhead fg_info->domain_index$$
This member function has prototype
$codei%
	virtual void ipopt_cppad_fg_info::domain_index(
		size_t %k%, SizeVector& %J_k%
	)
%$$ 
Let $icode k$$ have type $code size_t$$ and value
between zero and $latex K - 1$$.
Let $icode J_k$$ be a $cref/SimpleVector/$$ with elements
of type $code size_t$$ and size equal to $icode q$$.
The syntax
$codei%
	%fg_info%->domain_index(%k%, %J_k%)
%$$
sets the elements of $icode J_k$$
to the corresponding elements of $latex J_k$$ in the 
representation of $latex fg(x)$$ in terms of $latex r(u)$$.
$pre

$$
The $code ipopt_cppad_h_base$$ implementation of this function
corresponds to the simple representation mentioned above; i.e.,
for $latex j = 0 , \ldots , n-1$$,
$icode%J_k%[%j%] = %j%$$.

$subhead fg_info->range_index$$
This member function has prototype
$codei%
	virtual void ipopt_cppad_fg_info::range_index(
		size_t %k%, SizeVector& %I_k%
	)
%$$ 
Let $icode k$$ have type $code size_t$$ and value
between zero and $latex K - 1$$.
Let $icode I_k$$ be a $cref/SimpleVector/$$ with elements
of type $code size_t$$ and size equal to $icode p$$.
The syntax
$codei%
	%fg_info%->domain_index(%k%, %I_k%)
%$$
sets the elements of $icode I_k$$
to the corresponding elements of $latex I_k$$ in the 
representation of $latex fg(x)$$ in terms of $latex r(u)$$.
$pre

$$
The $code ipopt_cppad_h_base$$ implementation of this function
corresponds to the simple representation mentioned above; i.e.,
for $latex i = 0 , \ldots , m$$,
$icode%I_k%[%i%] = %i%$$.

$head solution$$
After the optimization process is completed, $icode solution$$ contains
the following information:

$subhead status$$
The $icode status$$ field of $icode solution$$ has prototype
$codei%
	ipopt_cppad_solution::solution_status %solution%.status
%$$
It is the final Ipopt status for the optimizer. 
Here is a list of the possible values for the status:

$table
$icode status$$ $cnext Meaning
$rnext
not_defined $cnext
The optimizer did not return a final status to this $code ipopt_cppad_nlp$$
object.
$rnext
unknown $cnext
The status returned by the optimizer is not defined in the Ipopt
documentation for $code finalize_solution$$.
$rnext
success $cnext
Algorithm terminated successfully at a point satisfying the convergence 
tolerances (see Ipopt options).
$rnext
maxiter_exceeded $cnext
The maximum number of iterations was exceeded (see Ipopt options).
$rnext
stop_at_tiny_step $cnext
Algorithm terminated because progress was very slow.
$rnext
stop_at_acceptable_point $cnext
Algorithm stopped at a point that was converged, 
not to the 'desired' tolerances, but to 'acceptable' tolerances 
(see Ipopt options).
$rnext
local_infeasibility $cnext
Algorithm converged to a non-feasible point
(problem may have no solution).
$rnext
user_requested_stop $cnext
This return value should not happen.
$rnext
diverging_iterates $cnext
It the iterates are diverging.
$rnext
restoration_failure $cnext
Restoration phase failed, algorithm doesn't know how to proceed.
$rnext
error_in_step_computation $cnext
An unrecoverable error occurred while Ipopt tried to 
compute the search direction.
$rnext
invalid_number_detected $cnext
Algorithm received an invalid number (such as $code nan$$ or $code inf$$) 
from the users function $icode%fg_info%.eval%$$ or from the CppAD evaluations
of its derivatives
(see the Ipopt option $code check_derivatives_for_naninf$$).
$rnext
internal_error $cnext
An unknown Ipopt internal error occurred.
Contact the Ipopt authors through the mailing list.
$tend

$subhead x$$
The $code x$$ field of $icode solution$$ has prototype
$codei%
	NumberVector %solution%.x
%$$
and its size is equal to $latex n$$.
It is the final $latex x$$ value for the optimizer.

$subhead z_l$$
The $code z_l$$ field of $icode solution$$ has prototype
$codei%
	NumberVector %solution%.z_l
%$$
and its size is equal to $latex n$$.
It is the final Lagrange multipliers for the 
lower bounds on $latex x$$.

$subhead z_u$$
The $code z_u$$ field of $icode solution$$ has prototype
$codei%
	NumberVector %solution%.z_u
%$$
and its size is equal to $latex n$$.
It is the final Lagrange multipliers for the 
upper bounds on $latex x$$.

$subhead g$$
The $code g$$ field of $icode solution$$ has prototype
$codei%
	NumberVector %solution%.g
%$$
and its size is equal to $latex m$$.
It is the final value for the constraint function $latex g(x)$$.

$subhead lambda$$
The $code lambda$$ field of $icode solution$$ has prototype
$codei%
	NumberVector %solution%.lambda
%$$
and its size is equal to $latex m$$.
It is the final value for the 
Lagrange multipliers corresponding to the constraint function.

$subhead obj_value$$
The $code obj_value$$ field of $icode solution$$ has prototype
$codei%
	Number %solution%.obj_value
%$$
It is the final value of the objective function $latex f(x)$$.


$children%
	example/ipopt_cppad.cpp
%$$

$head Example$$
The file 
$cref/ipopt_cppad.cpp/$$ is an example and test of $code ipopt_cppad_nlp$$.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/


# include <cppad/cppad.hpp>
# include <coin/IpIpoptApplication.hpp>
# include <coin/IpTNLP.hpp>

typedef CppAD::AD<Ipopt::Number>       ADNumber;
typedef CppAD::vector<size_t>          SizeVector;
typedef CppAD::vector<Ipopt::Number>   NumberVector;
typedef CppAD::vector<ADNumber>        ADVector;

/*
Class for return solution values.
*/
class ipopt_cppad_fg_info
{
	friend class ipopt_cppad_nlp;
private:
	size_t n_;
	size_t m_;

	void set_n(size_t n)
	{	n_ = n; }
	void set_m(size_t m)
	{	m_ = m; }

public:
	// make destructor virtual so that derived class destructor gets called
	virtual ~ipopt_cppad_fg_info(void)
	{ }
	// r_eval: pure virtual so that it must be defined by derived class
	virtual ADVector r_eval(const ADVector& x) = 0;
	// retape: default definition 
	virtual bool retape(void)
	{	return true; }
	// number_sum: for simple representation 
	virtual size_t number_sum(void)
	{	return 1; }
	// domain_size: for simple representation 
	virtual size_t domain_size(void)
	{	return n_; }
	// range_size: for simple representation 
	virtual size_t range_size(void)
	{	return m_ + 1; }
	// domain_index: for simple representation
	virtual void domain_index(size_t k, SizeVector& index_vector)
	{	assert( index_vector.size() == n_ );
		for(size_t j = 0; j < n_; j++)
			index_vector[j] = j;
	}
	// range_index: for simple representation
	virtual void range_index(size_t k, SizeVector& index_vector)
	{	assert( index_vector.size() == m_ + 1 );
		for(size_t i = 0; i <= m_; i++)
			index_vector[i] = i;
	}
};

class ipopt_cppad_solution 
{
public:
	enum solution_status {
		not_defined,
		success,
		maxiter_exceeded,
		stop_at_tiny_step,
		stop_at_acceptable_point,
		local_infeasibility,
		user_requested_stop,
		feasible_point_found,
		diverging_iterates,
		restoration_failure,
		error_in_step_computation,
		invalid_number_detected,
		too_few_degrees_of_freedom,
		internal_error,
		unknown
	}  status;
	NumberVector      x;
	NumberVector      z_l;
	NumberVector      z_u;
	NumberVector      g;
	NumberVector      lambda;
	Ipopt::Number     obj_value;

	ipopt_cppad_solution(void)
	{	status = not_defined; }
};

/* 
Class for interfacing a problem to IPOPT and using CppAD for derivative 
and sparsity pattern calculations.
*/
class ipopt_cppad_nlp : public Ipopt::TNLP
{
	typedef Ipopt::Number                  Number;
	typedef Ipopt::Index                   Index;
	typedef Ipopt::TNLP::IndexStyleEnum    IndexStyleEnum;
	typedef CppAD::vectorBool              BoolVector;
	typedef CppAD::vector<Ipopt::Index>    IndexVector;
public:
	// constructor 
	ipopt_cppad_nlp(
		size_t n                         , 
		size_t m                         ,
		const NumberVector    &x_i       ,
		const NumberVector    &x_l       ,
		const NumberVector    &x_u       ,
		const NumberVector    &g_l       ,
		const NumberVector    &g_u       ,
		ipopt_cppad_fg_info*   fg_info   ,
		ipopt_cppad_solution*  solution
  	);


	// default destructor 
	virtual ~ipopt_cppad_nlp();

	// return info about the nlp
	virtual bool get_nlp_info(
		Index&          n           , 
		Index&          m           , 
		Index&          nnz_jac_g   ,
		Index&          nnz_h_lag   , 
		IndexStyleEnum& index_style
	);

	// return bounds for my problem 
	virtual bool get_bounds_info(
		Index           n   , 
		Number*         x_l , 
		Number*         x_u ,
		Index           m   , 
		Number*         g_l , 
		Number*         g_u   
	);

	// return the starting point for the algorithm 
	virtual bool get_starting_point(
		Index          n            , 
		bool           init_x       , 
		Number*        x            ,
		bool           init_z       , 
		Number*        z_L          , 
		Number*        z_U          ,
		Index          m            ,
		bool           init_lambda  ,
		Number*        lambda
	);

	// return the objective value 
	virtual bool eval_f(
		Index          n           , 
		const Number*  x           , 
		bool           new_x       , 
		Number&        obj_value
	);

	// Method to return the gradient of the objective 
	virtual bool eval_grad_f(
		Index          n           , 
		const Number*  x           , 
		bool           new_x       , 
		Number*        grad_f
  	);

	// return the constraint residuals
	virtual bool eval_g(
		Index          n           , 
		const Number*  x           , 
		bool           new_x       , 
		Index          m           , 
		Number*        g
	);

	// Method to return:
	// 1) The structure of the jacobian (if "values" is NULL)
	// 2) The values of the jacobian (if "values" is not NULL)
	virtual bool eval_jac_g(
		Index          n           , 
		const Number*  x           , 
		bool           new_x       ,
		Index          m           , 
		Index          nele_jac    , 
		Index*         iRow        , 
		Index*         jCol        ,
		Number*        values
	);

	// Method to return:
	//  1) structure of hessian of the lagrangian (if "values" is NULL)
	//  2) values of hessian of the lagrangian (if "values" is not NULL)
	virtual bool eval_h(
		Index          n           , 
		const Number*  x           , 
		bool           new_x       ,
		Number         obj_factor  , 
		Index          m           , 
		const Number*  lambda      ,
		bool           new_lambda  , 
		Index          nele_hess   , 
		Index*         iRow        ,
		Index*         jCol        , 
		Number*        values
	);

	// called when the algorithm is completed so the TNLP can 
	// store/write the solution 
	virtual void finalize_solution(
		Ipopt::SolverReturn       status      ,
		Index                      n          , 
		const Number*              x          , 
		const Number*              z_L        , 
		const Number*              z_U        ,
		Index                      m          , 
		const Number*              g          , 
		const Number*              lambda     ,
		Number                     obj_value  ,
		const Ipopt::IpoptData*           ip_data    ,
		Ipopt::IpoptCalculatedQuantities* ip_cq
	);
private:
	/*
 	Values passed in by user
	*/
	// dimension of the domain space
	const size_t                    n_;
	// number of components in g
	const size_t                    m_;
	// initial x
	const NumberVector              x_i_;
	// limits for x and g
	const NumberVector              x_l_;
	const NumberVector              x_u_;
	const NumberVector              g_l_;
	const NumberVector              g_u_;
	// Users function that evaluates f and g
	ipopt_cppad_fg_info* const      fg_info_;
	// does operation sequence chage with argument value
	const bool                      retape_;
	// object for storing final solution results
	ipopt_cppad_solution* const     solution_;
	// values determined by fg_info
	size_t                          K_;  // number terms in summation
	size_t                          q_;  // dimension of domain for r(u)
	size_t                          p_;  // dimension of range for r(u)
	SizeVector                    J_k_;  // index vector for domain
	SizeVector                    I_k_;  // index vector for range
	/*
 	Computed values
	*/
	// CppAD sparsity patterns
	BoolVector                       pattern_jac_r_;
	BoolVector                       pattern_r_lag_;
	BoolVector                       pattern_jac_fg_;
	BoolVector                       pattern_h_lag_;
	// Ipopt sparsity structure for Jacobian of g
	size_t                           nnz_jac_g_;
	SizeVector                       iRow_jac_g_;
	SizeVector                       jCol_jac_g_;
	// Ipopt sparsity structure for Hessian of Lagragian
	size_t                           nnz_h_lag_;
	SizeVector                       iRow_h_lag_;
	SizeVector                       jCol_h_lag_;
	// CppAD function object for both f and g as one function
	CppAD::ADFun<Number>             r_fun_;
	/*
 	Methods
	*/
	// Methods to block default compiler methods.
	ipopt_cppad_nlp(const ipopt_cppad_nlp&);
	ipopt_cppad_nlp& operator=(const ipopt_cppad_nlp&);

	// Methods used by public methods
	static void record_r_fun(
		ipopt_cppad_fg_info  *fg_info    , 
		size_t                p          ,
		size_t                q          ,
		ADVector&             u_ad       , 
		CppAD::ADFun<Number>& r_fun
	);
	static void compute_pattern_jac_fg(
		ipopt_cppad_fg_info  *fg_info        , 
		SizeVector&           I_k            ,
		SizeVector&           J_k            ,
		size_t                K              ,
		size_t                m              ,
		size_t                n              ,
		size_t                p              ,
		size_t                q              ,
		CppAD::ADFun<Number>& r_fun          ,
		BoolVector&           pattern_jac_r  ,
		BoolVector&           pattern_jac_fg 
	);
	static void compute_pattern_h_lag(
		ipopt_cppad_fg_info  *fg_info        , 
		SizeVector&           I_k            ,
		SizeVector&           J_k            ,
		size_t                K              ,
		size_t                m              ,
		size_t                n              ,
		size_t                p              ,
		size_t                q              ,
		CppAD::ADFun<Number>& r_fun          ,
		BoolVector&           pattern_r_lag  , 
		BoolVector&           pattern_h_lag 
	);
	static void compute_structure_jac_g(
		size_t                m              ,
		size_t                n              ,
		const BoolVector&     pattern_jac_fg ,
		size_t&               nnz_jac_g      ,
		SizeVector&           iRow_jac_g     ,
		SizeVector&           jCol_jac_g
	);
	static void compute_structure_h_lag(
		size_t                m              ,
		size_t                n              ,
		const BoolVector&     pattern_h_lag  ,
		size_t&               nnz_h_lag      ,
		SizeVector&           iRow_h_lag     ,
		SizeVector&           jCol_h_lag
	);

};



# endif
