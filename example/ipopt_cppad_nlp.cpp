/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include "ipopt_cppad_nlp.hpp"

// define as 0 for false or 1 for true
# define  CPPAD_NLP_TRACE 0

# if CPPAD_NLP_TRACE
# include <cstdio>
# endif

/* Constructor. */
ipopt_cppad_nlp::ipopt_cppad_nlp(
	Index                      n           , 
	Index                      m           , 
	const NumberVector&        x_i         ,
	const NumberVector&        x_l         ,
	const NumberVector&        x_u         ,
	const NumberVector&        g_l         ,
	const NumberVector&        g_u         ,
	FgPointer                  fg_ad       ,
	bool                       retape      ,
	ipopt_cppad_solution*      solution    )
	: n_ ( n ),
	  m_ ( m ),
	  x_i_ ( x_i ),
	  x_l_ ( x_l ),
	  x_u_ ( x_u ),
	  g_l_ ( g_l ),
	  g_u_ ( g_u ),
	  fg_ad_  ( fg_ad ) ,
	  retape_ (retape),
	  solution_ (solution)
{	Index i, j, k;

	pattern_jac_fg_.resize( (m + 1) * n );
	pattern_h_lag_.resize( n * n );

	if ( retape )
	{	// true sparsity pattern valid for all x is unknown
		for(i = 0; i <= m; i++)
		{	for(j = 0; j < n; j++)
				pattern_jac_fg_[i * n + j] = true;
		}
		for(j = 0; j < n; j++)
		{	for(k = 0; k < n; k++)
				pattern_h_lag_[j * n + k] = true;
		}
	}
	else
	{	// Record both f and g in one AD function object
		// (operation sequence does not depend on value of x).
		ADVector x_ad_vec(n);
		for(j = 0; j < n; j++)
			x_ad_vec[0] = 0.;
		record_fg_fun(
			m, n, x_ad_vec, fg_ad_,               // inputs
			fg_fun_                               // outputs
		);

		// compute CppAD sparsity partern for Jacobian of fg
		compute_pattern_jac_fg(
			m, n, fg_fun_,                        // inputs
			pattern_jac_fg_                       // outputs
		);

		// compute CppAD sparsity partern for Hessian of Lagragian
		compute_pattern_h_lag(
			m, n, fg_fun_,                         // inputs
			pattern_h_lag_                         // outputs
		);
	}

	// Compute Ipopt sparsity structure for Jacobian of g 
	compute_structure_jac_g(
		m, n, pattern_jac_fg_,                // inputs
		nnz_jac_g_, iRow_jac_g_, jCol_jac_g_  // outputs
	);

	// Compute Ipopt sparsity structure for Hessian of Lagragian
	compute_structure_h_lag(
		m, n, pattern_h_lag_,                 // inputs
		nnz_h_lag_, iRow_h_lag_, jCol_h_lag_  // outputs
	);

}

// static member function that records operation sequence
void ipopt_cppad_nlp::record_fg_fun( 
	Index                  m        ,
	Index                  n        ,
	ADVector&              x_ad_vec ,
	FgPointer              fg_ad    , 
	CppAD::ADFun<Number>&  fg_fun   )
/*
m: input
number of components in the constraint function g.

n: input
number of independent variables.

x_ad_vec: input
vector of independent variable values. This is an input except for 
the fact that its CppAD private data changes.

fg_ad: input
the FgPointer that is used to compute the value of f and g.

fg_fun: output 
The CppAD operation sequence corresponding to the value of x_ad_vec,
and the algorithm used by fg_ad, is stored in fg_fun. (Any operation
seqeunce that was previously in fg_fun is deleted.)
*/
{	assert( x_ad_vec.size() == size_t(n) );
	// vector of dependent variables during function recording
	ADVector fg_ad_vec(1 + m);
	// start the recording
	CppAD::Independent(x_ad_vec);
	// record operations for f(x)
	fg_ad_vec = (*fg_ad)(x_ad_vec);
	// stop the resording and store it in fg_fun
	fg_fun.Dependent(x_ad_vec, fg_ad_vec);
}

// static member function that computes CppAD sparsity pattern for 
// Jacobian of fg
void ipopt_cppad_nlp::compute_pattern_jac_fg(
	Index                 m              ,
	Index                 n              ,
	CppAD::ADFun<Number>& fg_fun         ,
	BoolVector&           pattern_jac_fg )
/*
m: input
The number of components in the constraint function g.

n: input
Number of indpendent variables.

fg_fun: input
the CppAD function object that is used to compute the sparsity patterns.
The state of this object actually changes because some forward mode
routines are used.
The operation sequence correspopnding to this object does not change.

pattern_jac_fg: output
On input, this must be a vector of length (m+1) * n.
On output it is the CppAD sparsity pattern for the Jacobian of fg_fun.
*/
{	assert( fg_fun.Range() == size_t(m+1) );
	assert( fg_fun.Domain() == size_t(n) );
	assert( pattern_jac_fg.size() == size_t( (m+1) * n ) );

	Index i, j, k;
	if( n < m + 1 )
	{	// use forward mode
		BoolVector pattern_domain(n * n);
		for(i = 0; i < n; i++)
		{	for(j = 0; j < n; j++) 
				pattern_domain[ i * n + j ] = false;
			pattern_domain[ i * n + i ] = true;
		}
		pattern_jac_fg = fg_fun.ForSparseJac(n, pattern_domain);
	}
	else
	{	// use reverse mode
		BoolVector pattern_range( (m + 1) * (m + 1) );
		for(i = 0; i <= m; i++)
		{	for(k = 0; k <= m; k++) 
				pattern_range[ i * (m + 1) + k ] = false;
			pattern_range[ i * (m + 1) + i ] = true;
		}
		pattern_jac_fg = fg_fun.RevSparseJac(m + 1, pattern_range);
	}
}

// static member function that computes CppAD sparsity pattern for 
// Hessian of Lagragian
void ipopt_cppad_nlp::compute_pattern_h_lag(
	Index                 m              ,
	Index                 n              ,
	CppAD::ADFun<Number>& fg_fun         ,
	BoolVector&           pattern_h_lag  )
/*
m: input
The number of components in the constraint function g.

n: input
Number of indpendent variables.

fg_fun: input
the CppAD function object that is used to compute the sparsity patterns.
The state of this object actually changes because some forward mode
routines are used.
The operation sequence correspopnding to this object does not change.

pattern_h_lag: output
On input, this must be a vector of length n * n.
On output it is the CppAD sparsity pattern for the Hessian of 
the Lagragian.
*/
{	assert( fg_fun.Range() == size_t(m+1) );
	assert( fg_fun.Domain() == size_t(n) );
	assert( pattern_h_lag.size() == size_t(n*n) );
	Index i, j;
	BoolVector pattern_domain(n * n);
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++) 
			pattern_domain[ i * n + j ] = false;
		pattern_domain[ i * n + i ] = true;
	}
	fg_fun.ForSparseJac(n, pattern_domain);
	BoolVector pattern_ones(m + 1);
	for(i = 0; i <= m; i++)
		pattern_ones[i] = true;
	pattern_h_lag = fg_fun.RevSparseHes(n, pattern_ones);
}

// static member function that computes the Ipopt sparsity structure for 
// Jacobian of g
void ipopt_cppad_nlp::compute_structure_jac_g(
	Index m                          ,
	Index n                          ,
	const BoolVector& pattern_jac_fg ,
	Index&            nnz_jac_g      ,
	IndexVector&      iRow_jac_g     ,
	IndexVector&      jCol_jac_g     )
/*
m: input
The number of components in the constraint function g.

n: input
Number of indpendent variables.

pattern_jac_fg: input
The CppAD sparsity pattern for the Jacobian of fg.

nnz_jac_g: output
The number of possibly non-zero entries in the Jacobian of g.

iRow_jac_g: output
The input size of this vector does not matter.
On output it has size nnz_jac_g.
It specifies the Fortran row index (i.e. base one) 
corresponding to each non-zero entry in the Jacobian of g.

jCol_jac_g: output
The input size of this vector does not matter.
On output it has size nnz_jac_g.
It specifies the Fortran column index (i.e. base one) 
corresponding to each non-zero entry in the Jacobian of g.
*/
{	Index i, j, k;

	nnz_jac_g = 0;
	for(i = 1; i <= m; i++)
	{	for(j = 0; j < n; j++)
			if( pattern_jac_fg[ i * n + j ] )
				++nnz_jac_g;
	}
	iRow_jac_g.resize( nnz_jac_g );
	jCol_jac_g.resize( nnz_jac_g );
	k = 0;
	for(i = 1; i <= m; i++)
	{	for(j = 0; j < n; j++)
			if( pattern_jac_fg[ i * n + j ] )
			{	iRow_jac_g[k] = i;
				jCol_jac_g[k] = j + 1;
				k++;
			}
	}
}

// static member function that computes the Ipopt sparsity structure for 
// Hessian of Lagragian
void ipopt_cppad_nlp::compute_structure_h_lag(
	Index m                          ,
	Index n                          ,
	const BoolVector& pattern_h_lag  ,
	Index&            nnz_h_lag      ,
	IndexVector&      iRow_h_lag     ,
	IndexVector&      jCol_h_lag     )
/*
m: input
The number of components in the constraint function g.

n: input
Number of indpendent variables.

pattern_jac_fg: input
The CppAD sparsity pattern for the Hessian of the Lagragian.

nnz_h_lag: output
The number of possibly non-zero entries in the Hessian of the Lagragian.

iRow_h_lag: output
The input size of this vector does not matter.
On output it has size nnz_h_lag.
It specifies the Fortran row index (i.e. base one) 
corresponding to each non-zero entry in the Hessian of the Lagragian.

jCol_h_lag: output
The input size of this vector does not matter.
On output it has size nnz_h_lag.
It specifies the Fortran column index (i.e. base one) 
corresponding to each non-zero entry in the Hessian of the Lagragian.
*/
{	Index i, j, k;

	nnz_h_lag = 0;
	for(i = 0; i < n; i++)
	{	for(j = 0; j <= i; j++)
			if( pattern_h_lag[ i * n + j ] )
				++nnz_h_lag;
	}
	iRow_h_lag.resize( nnz_h_lag );
	jCol_h_lag.resize( nnz_h_lag );
	k = 0;
	for(i = 0; i < n; i++)
	{	for(j = 0; j <= i; j++)
			if( pattern_h_lag[ i * n + j ] )
			{	iRow_h_lag[k] = i + 1;
				jCol_h_lag[k] = j + 1;
				k++;
			}
	}
}

ipopt_cppad_nlp::~ipopt_cppad_nlp()
{}

bool ipopt_cppad_nlp::get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
                         Index& nnz_h_lag, IndexStyleEnum& index_style)
{
	n = n_;
	m = m_;
	nnz_jac_g = nnz_jac_g_;
	nnz_h_lag = nnz_h_lag_;

  	// use the fortran index style for row/col entries
	index_style = FORTRAN_STYLE;

	return true;
}

bool ipopt_cppad_nlp::get_bounds_info(Index n, Number* x_l, Number* x_u,
                            Index m, Number* g_l, Number* g_u)
{	Index i, j;
	// here, the n and m we gave IPOPT in get_nlp_info are passed back 
	assert(n == n_);
	assert(m == m_);

	// pass back bounds
	for(j = 0; j < n; j++)
	{	x_l[j] = x_l_[j];
		x_u[j] = x_u_[j];
	}
	for(i = 0; i < m; i++)
	{	g_l[i] = g_l_[i];
		g_u[i] = g_u_[i];
	}
	
	return true;
}

bool ipopt_cppad_nlp::get_starting_point(Index n, bool init_x, Number* x,
                               bool init_z, Number* z_L, Number* z_U,
                               Index m, bool init_lambda,
                               Number* lambda)
{	Index j;

	assert( n == n_ );
	assert( m == m_ );
	assert(init_x == true);
	assert(init_z == false);
	assert(init_lambda == false);

	for(j = 0; j < n; j++)
		x[j] = x_i_[j];

	return true;
}

bool ipopt_cppad_nlp::eval_f(
	Index n, const Number* x, bool new_x, Number& obj_value
)
{	Index j;
	assert( n == n_ );

	if( new_x && retape_ )
	{	// Record fg for the current value of x
		ADVector x_ad_vec(n);
		for(j = 0; j < n; j++)
			x_ad_vec[0] = x[j];
		record_fg_fun(
			m_, n, x_ad_vec, fg_ad_,              // inputs
			fg_fun_                               // outputs
		);
	}

	NumberVector x_vec(n);
	NumberVector fg_vec(1 + m_);
	for(j = 0; j < n; j++)
		x_vec[j]   = x[j];
	fg_vec    = fg_fun_.Forward(0, x_vec);
	obj_value = fg_vec[0];
# if CPPAD_NLP_TRACE
	using std::printf;
	for(j = 0; j < n; j++)
		printf("ipopt_cppad_nlp::eval_f::x[%d] = %20.14g\n", j, x[j]);
	printf("ipopt_cppad_nlp::eval_f::obj_value = %20.14g\n", obj_value);
# endif
	return true;
}

bool ipopt_cppad_nlp::eval_grad_f(
	Index n, const Number* x, bool new_x, Number* grad_f
)
{	Index j;
	assert( n == n_ );

	if( new_x && retape_ )
	{	// Record fg for the current value of x
		ADVector x_ad_vec(n);
		for(j = 0; j < n; j++)
			x_ad_vec[0] = x[j];
		record_fg_fun(
			m_, n, x_ad_vec, fg_ad_,              // inputs
			fg_fun_                               // outputs
		);
	}

	NumberVector x_vec(n);
	NumberVector fg_grad_vec((1 + m_) * n);
	for(Index j = 0; j < n; j++)
		x_vec[j]   = x[j];
	fg_grad_vec = fg_fun_.Jacobian(x_vec);
	for(j = 0; j < n; j++)
		grad_f[j] = fg_grad_vec[0 * n + j];
# if CPPAD_NLP_TRACE
	using std::printf;
	for(j = 0; j < n; j++) printf(
	"ipopt_cppad_nlp::eval_grad_f::x[%d] = %20.14g\n", j, x[j]
	);
	for(j = 0; j < n; j++) printf(
	"ipopt_cppad_nlp::eval_grad_f::grad_f[%d] = %20.14g\n", j, grad_f[j]
	);
# endif
	return true;
}

bool ipopt_cppad_nlp::eval_g(
	Index n, const Number* x, bool new_x, Index m, Number* g
)
{	Index i, j;
	assert( n == n_);
	assert( m == m_);

	if( new_x && retape_ )
	{	// Record fg for the current value of x
		ADVector x_ad_vec(n);
		for(j = 0; j < n; j++)
			x_ad_vec[0] = x[j];
		record_fg_fun(
			m, n, x_ad_vec, fg_ad_,               // inputs
			fg_fun_                               // outputs
		);
	}

	NumberVector x_vec(n);
	NumberVector fg_vec(1 + m);
	for(j = 0; j < n; j++)
		x_vec[j]   = x[j];
	fg_vec = fg_fun_.Forward(0, x_vec);
	for(i = 0; i < m; i++)
		g[i] = fg_vec[1+i];
# if CPPAD_NLP_TRACE
	using std::printf;
	for(j = 0; j < n; j++)
		printf("ipopt_cppad_nlp::eval_g::x[%d] = %20.14g\n", j, x[j]);
	for(i = 0; i < m; i++)
		printf("ipopt_cppad_nlp::eval_g::g[%d] = %20.14g\n", i, g[i]);
# endif
	return true;
}

bool ipopt_cppad_nlp::eval_jac_g(Index n, const Number* x, bool new_x,
                       Index m, Index nele_jac, Index* iRow, Index *jCol,
                       Number* values)
{	assert( n == n_ );
	assert( m == m_ );

	if( new_x && retape_ )
	{	// Record fg for the current value of x
		ADVector x_ad_vec(n);
		Index j;
		for(j = 0; j < n; j++)
			x_ad_vec[0] = x[j];
		record_fg_fun(
			m, n, x_ad_vec, fg_ad_,                // inputs
			fg_fun_                               // outputs
		);
	}

	Index k;
	if (values == NULL) 
	{	for(k = 0; k < nnz_jac_g_; k++)
		{	iRow[k] = iRow_jac_g_[k];
			jCol[k] = jCol_jac_g_[k];
		}
	}
	else 
	{	// return the values of the jacobian of the constraints
		NumberVector x_vec(n);
		NumberVector jac_fg_vec((1 + m) * n);
		for(Index j = 0; j < n; j++)
			x_vec[j]   = x[j];
		if( retape_ )
			jac_fg_vec = fg_fun_.Jacobian(x_vec);
		else	jac_fg_vec = fg_fun_.SparseJacobian(
				x_vec, pattern_jac_fg_
			);

		for(k = 0; k < nnz_jac_g_; k++)
		{	Index i = iRow_jac_g_[k];
			Index j = jCol_jac_g_[k] - 1;	
			values[k] = jac_fg_vec[ i * n + j ];
		} 
	}
  	return true;
}

bool ipopt_cppad_nlp::eval_h(Index n, const Number* x, bool new_x,
                   Number obj_factor, Index m, const Number* lambda,
                   bool new_lambda, Index nele_hess, Index* iRow,
                   Index* jCol, Number* values)
{	Index j, k;
	assert( n == n_ );
	assert( m == m_ );

	if( new_x && retape_ )
	{	// Record fg for the current value of x
		ADVector x_ad_vec(n);
		for(j = 0; j < n; j++)
			x_ad_vec[0] = x[j];
		record_fg_fun(
			m, n, x_ad_vec, fg_ad_,               // inputs
			fg_fun_                               // outputs
		);
	}

	if (values == NULL) 
	{	for(k = 0; k < nnz_h_lag_; k++)
		{	iRow[k] = iRow_h_lag_[k];
			jCol[k] = jCol_h_lag_[k];
		}
	}
	else 
	{	// return the values of the Hessian of the Lagragina
		NumberVector x_vec(n);
		NumberVector w_vec(1 + m);
		NumberVector L_hes_vec(n * n);
		for(j = 0; j < n; j++)
			x_vec[j]   = x[j];
		w_vec[0] = obj_factor;
		for(Index i = 0; i < m; i++)
			w_vec[1+i] = lambda[i];
		if( retape_ )
			L_hes_vec = fg_fun_.Hessian(x_vec, w_vec);
		else	L_hes_vec = fg_fun_.SparseHessian(
				x_vec, w_vec, pattern_h_lag_
			);
		for(k = 0; k < nnz_h_lag_; k++)
		{	Index i = iRow_h_lag_[k] - 1;
			Index j = jCol_h_lag_[k] - 1;	
			values[k] = L_hes_vec[ i * n + j ];
		} 
	}

	return true;
}

void ipopt_cppad_nlp::finalize_solution(
	Ipopt::SolverReturn               status    ,
	Index                             n         , 
	const Number*                     x         , 
	const Number*                     z_L       , 
	const Number*                     z_U       ,
	Index                             m         , 
	const Number*                     g         , 
	const Number*                     lambda    ,
	Number                            obj_value ,
	const Ipopt::IpoptData*           ip_data   ,
	Ipopt::IpoptCalculatedQuantities* ip_cq
)
{	Index i, j;

	assert( n == n_ );
	assert( m == m_ );

	switch(status)
	{	// convert status from Ipopt enum to ipopt_cppad_solution enum
		case Ipopt::SUCCESS:
		solution_->status = 
			ipopt_cppad_solution::success;
		break;

		case Ipopt::MAXITER_EXCEEDED:
		solution_->status = 
			ipopt_cppad_solution::maxiter_exceeded;
		break;

		case Ipopt::STOP_AT_TINY_STEP:
		solution_->status = 
			ipopt_cppad_solution::stop_at_tiny_step;
		break;

		case Ipopt::STOP_AT_ACCEPTABLE_POINT:
		solution_->status = 
			ipopt_cppad_solution::stop_at_acceptable_point;
		break;

		case Ipopt::LOCAL_INFEASIBILITY:
		solution_->status = 
			ipopt_cppad_solution::local_infeasibility;
		break;

		case Ipopt::USER_REQUESTED_STOP:
		solution_->status = 
			ipopt_cppad_solution::user_requested_stop;
		break;

		case Ipopt::DIVERGING_ITERATES:
		solution_->status = 
			ipopt_cppad_solution::diverging_iterates;
		break;

		case Ipopt::RESTORATION_FAILURE:
		solution_->status = 
			ipopt_cppad_solution::restoration_failure;
		break;

		case Ipopt::ERROR_IN_STEP_COMPUTATION:
		solution_->status = 
			ipopt_cppad_solution::error_in_step_computation;
		break;

		case Ipopt::INVALID_NUMBER_DETECTED:
		solution_->status = 
			ipopt_cppad_solution::invalid_number_detected;
		break;

		case Ipopt::INTERNAL_ERROR:
		solution_->status = 
			ipopt_cppad_solution::internal_error;
		break;

		default:
		solution_->status = 
			ipopt_cppad_solution::unknown;
	}

	solution_->x.resize(n);
	solution_->z_l.resize(n);
	solution_->z_u.resize(n);
	for(j = 0; j < n; j++)
	{	solution_->x[j]    = x[j];
		solution_->z_l[j]  = z_L[j];
		solution_->z_u[j]  = z_U[j];
	}
	solution_->g.resize(m);
	solution_->lambda.resize(m);
	for(i = 0; i < m; i++)
	{	solution_->g[i]      = g[i];
		solution_->lambda[i] = lambda[i];
	}
	solution_->obj_value = obj_value;
	return;
}
