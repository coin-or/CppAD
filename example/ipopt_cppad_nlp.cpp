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
	size_t                     n           , 
	size_t                     m           , 
	const NumberVector&        x_i         ,
	const NumberVector&        x_l         ,
	const NumberVector&        x_u         ,
	const NumberVector&        g_l         ,
	const NumberVector&        g_u         ,
	ipopt_cppad_fg_info*       fg_info     ,
	ipopt_cppad_solution*      solution    )
	: n_ ( n ),
	  m_ ( m ),
	  x_i_ ( x_i ),
	  x_l_ ( x_l ),
	  x_u_ ( x_u ),
	  g_l_ ( g_l ),
	  g_u_ ( g_u ),
	  fg_info_ ( fg_info ) ,
	  retape_ ( fg_info->retape() ) ,
	  solution_ (solution)
{	size_t i, j, k;

	// set information needed in ipopt_cppad_fg_info
	fg_info->set_n(n);
	fg_info->set_m(m);

	// get information from derived class version of fg_info
	K_ = fg_info->number_sum();
	q_ = fg_info->domain_size();
	p_ = fg_info->range_size();
	J_k_.resize(q_);
	I_k_.resize(p_);

	pattern_jac_r_.resize( p_ * q_ );
	pattern_r_lag_.resize( q_ * q_ );

	pattern_jac_fg_.resize( (m + 1) * n );
	pattern_h_lag_.resize( n * n );

	if ( retape_ )
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
		ADVector u_ad(q_);
		for(j = 0; j < q_; j++)
			u_ad[j] = 0.;
		record_r_fun(
			fg_info_, p_, q_, u_ad, // inputs
			r_fun_                  // outputs
		);

		// compute CppAD sparsity partern for Jacobian of fg
		compute_pattern_jac_fg(
		fg_info_, I_k_, J_k_, K_, m_, n_, p_, q_, r_fun_,   // inputs
		pattern_jac_r_, pattern_jac_fg_                     // outputs
		);

		// compute CppAD sparsity partern for Hessian of Lagragian
		compute_pattern_h_lag(
		fg_info_, I_k_, J_k_, K_, m_, n_, p_, q_, r_fun_,   // inputs
		pattern_r_lag_, pattern_h_lag_                      // outputs
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
void ipopt_cppad_nlp::record_r_fun( 
	ipopt_cppad_fg_info*   fg_info  , 
	size_t                 p        ,
	size_t                 q        ,
	ADVector&              u_ad     ,
	CppAD::ADFun<Number>&  r_fun    )
/*
fg_info: input
the ipopt_cppad_fg_info object that is used to compute the value of r(u).

p: input
number of components in the range space for r(u).

q: input
number of components in domain space for r(u).

u_ad: input
vector of independent variable values at which to record r(u).
This is an input except for 
the fact that its CppAD private data changes.

r_fun: output 
The CppAD operation sequence corresponding to the value of u_ad,
and the algorithm used by fg_info, is stored in r_fun. (Any operation
seqeunce that was previously in r_fun  is deleted.)
*/
{	assert( u_ad.size() == size_t(q) );
	// vector of dependent variables during function recording
	ADVector r_ad(p);
	// start the recording
	CppAD::Independent(u_ad);
	// record operations for r(u)
	r_ad = fg_info->r_eval(u_ad);
	// stop the resording and store operation sequence in r_fun
	r_fun.Dependent(u_ad, r_ad);
}

// static member function that computes CppAD sparsity pattern for 
// Jacobian of fg
void ipopt_cppad_nlp::compute_pattern_jac_fg(
	ipopt_cppad_fg_info*  fg_info        , 
	SizeVector&           I_k            ,
	SizeVector&           J_k            ,
	size_t                K              ,
	size_t                m              ,
	size_t                n              ,
	size_t                p              ,
	size_t                q              ,
	CppAD::ADFun<Number>& r_fun          ,
	BoolVector&           pattern_jac_r  ,
	BoolVector&           pattern_jac_fg )
/*
fg_info: input
the ipopt_cppad_fg_info object that is used to compute the value of r(u).

I_k: scratch space
Vector of length p.

J_k: scratch space
Vector of length q.

K: input
Number of terms in the representation for fg(x) in terms of r(u).

m: input
The number of components in the constraint function g.

n: input
Number of indpendent variables.

p: input
Dimension of the range space for r(u).

q: input
Dimension of the domain space for r(u).

r_fun: input
the CppAD function object that is used to compute the sparsity patterns.
The state of this object actually changes because some forward mode
routines are used.
The operation sequence correspopnding to this object does not change.

pattern_jac_r: output
On input, this must be a vector of length p * q.
On output it is the CppAD sparsity pattern for the Jacobian of r.

pattern_jac_fg: output
On input, this must be a vector of length (m+1) * n.
On output it is the CppAD sparsity pattern for the Jacobian of fg.
*/
{	assert( r_fun.Range() == p );
	assert( r_fun.Domain() == q );
	assert( pattern_jac_r.size() == p * q );
	assert( pattern_jac_fg.size() == (m+1) * n );

	size_t i, j, k, ir, ifg;

	// compute sparsity pattern for r
	if( q < p )
	{	// use forward mode
		BoolVector pattern_domain(q * q);
		for(i = 0; i < q; i++)
		{	for(j = 0; j < q; j++) 
				pattern_domain[ i * q + j ] = false;
			pattern_domain[ i * q + i ] = true;
		}
		pattern_jac_r = r_fun.ForSparseJac(q, pattern_domain);
	}
	else
	{	// use reverse mode
		BoolVector pattern_range(p * p);
		for(i = 0; i < p; i++)
		{	for(k = 0; k < p; k++) 
				pattern_range[ i * p + k ] = false;
			pattern_range[ i * p + i ] = true;
		}
		pattern_jac_r = r_fun.RevSparseJac(p, pattern_range);
	}

	// now compute pattern for fg
	k = (m+1) * n;
	while(k--)
		pattern_jac_fg[k] = false;
	for(k = 0; k < K; k++)
	{	fg_info->range_index(k, I_k);	
		fg_info->domain_index(k, J_k);
		for(i = 0; i < p; i++)
		{	for(j = 0; j < q; j++)
			{	ir  = i * q + j;
				ifg = I_k[i] * n + J_k[j];
				pattern_jac_fg[ifg] = ( pattern_jac_fg[ifg] 
				                    | pattern_jac_r[ir]    );
			}
		}
	}
}

// static member function that computes CppAD sparsity pattern for 
// Hessian of Lagragian
void ipopt_cppad_nlp::compute_pattern_h_lag(
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
	BoolVector&           pattern_h_lag  )
/*
fg_info: input
the ipopt_cppad_fg_info object that is used to compute the value of r(u).

I_k: scratch space
Vector of length p.

J_k: scratch space
Vector of length q.

K: input
Number of terms in the representation for fg(x) in terms of r(u).

m: input
The number of components in the constraint function g.

n: input
Number of indpendent variables.

p: input
Dimension of the range space for r(u).

q: input
Dimension of the domain space for r(u).

r_fun: input
the CppAD function object that is used to compute the sparsity patterns.
The state of this object actually changes because some forward mode
routines are used.
The operation sequence correspopnding to this object does not change.

pattern_r_lag: output
On input, this must be a vector of length q * q.
On output it is the CppAD sparsity pattern for the Hessian of 
the Lagragian for r.

pattern_h_lag: output
On input, this must be a vector of length n * n.
On output it is the CppAD sparsity pattern for the Hessian of 
the Lagragian for fg.
*/
{	assert( r_fun.Range() == p );
	assert( r_fun.Domain() == q );
	assert( pattern_r_lag.size() == q * q );
	assert( pattern_h_lag.size() == n * n );
	size_t i, j, k;
	BoolVector pattern_domain(q * q);
	BoolVector pattern_ones(p);

	for(i = 0; i < q; i++)
	{	for(j = 0; j < q; j++) 
			pattern_domain[ i * q + j ] = false;
		pattern_domain[ i * q + i ] = true;
	}
	r_fun.ForSparseJac(q, pattern_domain);
	for(i = 0; i < p; i++)
		pattern_ones[i] = true;
	pattern_r_lag = r_fun.RevSparseHes(q, pattern_ones);

	// now compute pattern for fg
	k = (n * n);
	while(k--)
		pattern_h_lag[k] = false;
	for(k = 0; k < K; k++)
	{	fg_info->domain_index(k, J_k);
		for(i = 0; i < q; i++)
		{	for(j = 0; j < q; j++)
			{	size_t ir, ifg;
				ir  = i * q + j;
				ifg = J_k[i] * n + J_k[j];
				pattern_h_lag[ifg] = ( pattern_h_lag[ifg] 
				                    | pattern_r_lag[ir]    );
			}
		}
	}
}

// static member function that computes the Ipopt sparsity structure for 
// Jacobian of g
void ipopt_cppad_nlp::compute_structure_jac_g(
	size_t            m              ,
	size_t            n              ,
	const BoolVector& pattern_jac_fg ,
	size_t&           nnz_jac_g      ,
	SizeVector&       iRow_jac_g     ,
	SizeVector&       jCol_jac_g     )
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
{	size_t i, j, k;

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
	size_t             m             ,
	size_t             n             ,
	const BoolVector& pattern_h_lag  ,
	size_t&           nnz_h_lag      ,
	SizeVector&       iRow_h_lag     ,
	SizeVector&       jCol_h_lag     )
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
{	size_t i, j, k;

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
{	size_t i, j;
	// here, the n and m we gave IPOPT in get_nlp_info are passed back 
	assert(size_t(n) == n_);
	assert(size_t(m) == m_);

	// pass back bounds
	for(j = 0; j < n_; j++)
	{	x_l[j] = x_l_[j];
		x_u[j] = x_u_[j];
	}
	for(i = 0; i < m_; i++)
	{	g_l[i] = g_l_[i];
		g_u[i] = g_u_[i];
	}
	
	return true;
}

bool ipopt_cppad_nlp::get_starting_point(Index n, bool init_x, Number* x,
                               bool init_z, Number* z_L, Number* z_U,
                               Index m, bool init_lambda,
                               Number* lambda)
{	size_t j;

	assert(size_t(n) == n_ );
	assert(size_t(m) == m_ );
	assert(init_x == true);
	assert(init_z == false);
	assert(init_lambda == false);

	for(j = 0; j < n_; j++)
		x[j] = x_i_[j];

	return true;
}

bool ipopt_cppad_nlp::eval_f(
	Index n, const Number* x, bool new_x, Number& obj_value
)
{	// This routine is not efficient. It would be better if users version
	// of the function was templated so that we could use double to 
	// calculate function values instead of retaping (when necessary).
	assert(size_t(n) == n_ );

	size_t iobj, j, k;
	ADVector     u_ad(q_);
	NumberVector u(q_);
	NumberVector r(p_);

	obj_value = 0.;
	for(k = 0; k < K_; k++)
	{	fg_info_->range_index(k, I_k_);
		for(iobj = 0; iobj < p_; iobj++) if( I_k_[iobj] == 0 )
		{	if( (new_x || K_ > 1)  && retape_ )
			{	// Record r for value of u corresponding to x
				fg_info_->domain_index(k, J_k_);
				for(j = 0; j < q_; j++)
				{	assert( J_k_[j] < n_ );
					u_ad[j] = x[ J_k_[j] ];
				}
				record_r_fun(
					fg_info_, p_, q_, u_ad,  // inputs
					r_fun_                   // outputs
				);
			}
			for(j = 0; j < q_; j++)
			{	assert( J_k_ [j] < n_ );
				u[j]   = x[ J_k_[j] ];
			}
			r          = r_fun_.Forward(0, u);
			obj_value += r[iobj];
		}
	}
# if CPPAD_NLP_TRACE
	using std::printf;
	for(j = 0; j < n_; j++)
		printf("ipopt_cppad_nlp::eval_f::x[%d] = %20.14g\n", j, x[j]);
	printf("ipopt_cppad_nlp::eval_f::obj_value = %20.14g\n", obj_value);
# endif
	return true;
}

bool ipopt_cppad_nlp::eval_grad_f(
	Index n, const Number* x, bool new_x, Number* grad_f
)
{	assert(size_t(n) == n_ );

	size_t iobj, i, j, k;
	ADVector     u_ad(q_);
	NumberVector u(q_);
	NumberVector w(p_);
	NumberVector obj_grad(n_);
	NumberVector r_grad(q_);

	// initialize weights as all zero
	for(i = 0; i < p_; i++)
		w[i] = 0.;
	// initialize gradient as zero
	for(j = 0; j < n_; j++)
		obj_grad[j] = 0.;

	for(k = 0; k < K_; k++)
	{	fg_info_->range_index(k, I_k_);
		for(iobj = 0; iobj < p_; iobj++) if( I_k_[iobj] == 0 )
		{	if( (new_x || K_ > 1)  && retape_ )
			{	// Record r for value of u corresponding to x
				fg_info_->domain_index(k, J_k_);
				for(j = 0; j < q_; j++)
				{	assert( J_k_[j] < n_ );
					u_ad[j] = x[ J_k_[j] ];
				}
				record_r_fun(
					fg_info_, p_, q_, u_ad,  // inputs
					r_fun_                   // outputs
				);
			}
			for(j = 0; j < q_; j++)
			{	assert( J_k_ [j] < n_ );
				u[j]   = x[ J_k_[j] ];
			}
			r_fun_.Forward(0, u);
			w[iobj]    = 1.;
			r_grad     = r_fun_.Reverse(1, w);
			w[iobj]    = 0.;
			for(j = 0; j < q_; j++)
				obj_grad[ J_k_[j] ]  += r_grad[j];
		}
	}
	for(j = 0; j < n_; j++)
		grad_f[j] = obj_grad[j];
# if CPPAD_NLP_TRACE
	using std::printf;
	for(j = 0; j < n_; j++) printf(
	"ipopt_cppad_nlp::eval_grad_f::x[%d] = %20.14g\n", j, x[j]
	);
	for(j = 0; j < n_; j++) printf(
	"ipopt_cppad_nlp::eval_grad_f::grad_f[%d] = %20.14g\n", j, grad_f[j]
	);
# endif
	return true;
}

bool ipopt_cppad_nlp::eval_g(
	Index n, const Number* x, bool new_x, Index m, Number* g
)
{	assert(size_t(n) == n_ );

	size_t i, j, k;
	ADVector     u_ad(q_);
	NumberVector u(q_);
	NumberVector r(p_);
	NumberVector fg(m_ + 1);

	for(i = 0; i <= m_; i++)
		fg[i] = 0.;

	for(k = 0; k < K_; k++)
	{	fg_info_->range_index(k, I_k_);
		if( (new_x || K_ > 1)  && retape_ )
		{	// Record r for value of u corresponding to x
			fg_info_->domain_index(k, J_k_);
			for(j = 0; j < q_; j++)
			{	assert( J_k_[j] < n_ );
				u_ad[j] = x[ J_k_[j] ];
			}
			record_r_fun(
				fg_info_, p_, q_, u_ad,  // inputs
				r_fun_                   // outputs
			);
		}
		for(j = 0; j < q_; j++)
		{	assert( J_k_ [j] < n_ );
			u[j]   = x[ J_k_[j] ];
		}
		r          = r_fun_.Forward(0, u);
		for(i = 0; i < p_; i++)
			fg[ I_k_[i] ] += r[i];
	}
	for(i = 0; i < m_; i++)
		g[i] = fg[i + 1];
# if CPPAD_NLP_TRACE
	using std::printf;
	for(j = 0; j < n_; j++)
		printf("ipopt_cppad_nlp::eval_g::x[%d] = %20.14g\n", j, x[j]);
	for(i = 0; i < m_; i++)
		printf("ipopt_cppad_nlp::eval_g::g[%d] = %20.14g\n", i, g[i]);
# endif
	return true;
}

bool ipopt_cppad_nlp::eval_jac_g(Index n, const Number* x, bool new_x,
                       Index m, Index nele_jac, Index* iRow, Index *jCol,
                       Number* values)
{	assert(size_t(m) == m_ );
	assert(size_t(n) == n_ );

	size_t i, j, k;
	ADVector     u_ad(q_);
	NumberVector u(q_);
	NumberVector jac_fg((m_ + 1) * n_);
	NumberVector jac_r(p_ * q_);

	if (values == NULL) 
	{	for(k = 0; k < nnz_jac_g_; k++)
		{	iRow[k] = iRow_jac_g_[k];
			jCol[k] = jCol_jac_g_[k];
		}
		return true;
	}

	// initialize Jacobian of fg
	i = (m_ + 1) * n_; 
	while(i--)
		jac_fg[i] = 0.;

	for(k = 0; k < K_; k++)
	{	fg_info_->range_index(k, I_k_);
		fg_info_->domain_index(k, J_k_);
		if( (new_x || K_ > 1)  && retape_ )
		{	// Record r for value of u corresponding to x
			fg_info_->domain_index(k, J_k_);
			for(j = 0; j < q_; j++)
			{	assert( J_k_[j] < n_ );
				u_ad[j] = x[ J_k_[j] ];
			}
			record_r_fun(
				fg_info_, p_, q_, u_ad,  // inputs
				r_fun_                   // outputs
			);
		}
		for(j = 0; j < q_; j++)
		{	assert( J_k_ [j] < n_ );
			u[j]   = x[ J_k_[j] ];
		}
		if( retape_ )
			jac_r = r_fun_.Jacobian(u);
		else	jac_r = r_fun_.SparseJacobian(u, pattern_jac_r_);
		for(i = 0; i < p_; i++)
		{	assert( I_k_[i] <= m_ );
			for(j = 0; j < q_; j++)
				jac_fg[ I_k_[i] * n_ + J_k_[j] ] += 
					jac_r[i * q_ + j];
		}
	}
	for(k = 0; k < nnz_jac_g_; k++)
	{	size_t i  = iRow_jac_g_[k];
		size_t j  = jCol_jac_g_[k] - 1;	
		values[k] = jac_fg[ i * n_ + j ];
	}
  	return true;
}

bool ipopt_cppad_nlp::eval_h(Index n, const Number* x, bool new_x,
                   Number obj_factor, Index m, const Number* lambda,
                   bool new_lambda, Index nele_hess, Index* iRow,
                   Index* jCol, Number* values)
{	assert(size_t(m) == m_ );
	assert(size_t(n) == n_ );

	size_t i, j, k;
	ADVector     u_ad(q_);
	NumberVector u(q_);
	NumberVector w(p_);
	NumberVector fg_hes(n_ * n_);
	NumberVector r_hes(q_ * q_);

	if (values == NULL) 
	{	for(k = 0; k < nnz_h_lag_; k++)
		{	iRow[k] = iRow_h_lag_[k];
			jCol[k] = jCol_h_lag_[k];
		}
		return true;
	}

	// initialize Jacobian of fg
	k = n_ * n_; 
	while(k--)
		fg_hes[k] = 0.;

	for(k = 0; k < K_; k++)
	{	fg_info_->range_index(k, I_k_);
		fg_info_->domain_index(k, J_k_);
		if( (new_x || K_ > 1)  && retape_ )
		{	// Record r for value of u corresponding to x
			fg_info_->domain_index(k, J_k_);
			for(j = 0; j < q_; j++)
			{	assert( J_k_[j] < n_ );
				u_ad[j] = x[ J_k_[j] ];
			}
			record_r_fun(
				fg_info_, p_, q_, u_ad,  // inputs
				r_fun_                   // outputs
			);
		}
		for(j = 0; j < q_; j++)
		{	assert( J_k_ [j] < n_ );
			u[j]   = x[ J_k_[j] ];
		}
		for(i = 0; i < p_; i++)
		{	assert( I_k_[i] <= m_ );
			if( I_k_[i] == 0 )
				w[i] = obj_factor;
			else	w[i] = lambda[ I_k_[i] - 1 ];
		}
		if( retape_ )
			r_hes = r_fun_.Hessian(u, w);
		else	r_hes = r_fun_.SparseHessian(u, w, pattern_r_lag_);
		for(i = 0; i < q_; i++)
		{	for(j = 0; j < q_; j++)
				fg_hes[ J_k_[i] * n_ + J_k_[j] ] += 
					r_hes[i * q_ + j];
		}
	}
	for(k = 0; k < nnz_h_lag_; k++)
	{	size_t i  = iRow_h_lag_[k] - 1;
		size_t j  = jCol_h_lag_[k] - 1;	
		values[k] = fg_hes[ i * n_ + j ];
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
{	size_t i, j;

	assert(size_t(n) == n_ );
	assert(size_t(m) == m_ );

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

	solution_->x.resize(n_);
	solution_->z_l.resize(n_);
	solution_->z_u.resize(n_);
	for(j = 0; j < n_; j++)
	{	solution_->x[j]    = x[j];
		solution_->z_l[j]  = z_L[j];
		solution_->z_u[j]  = z_U[j];
	}
	solution_->g.resize(m_);
	solution_->lambda.resize(m_);
	for(i = 0; i < m_; i++)
	{	solution_->g[i]      = g[i];
		solution_->lambda[i] = lambda[i];
	}
	solution_->obj_value = obj_value;
	return;
}
