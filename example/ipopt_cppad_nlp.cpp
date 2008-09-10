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
	  solution_ (solution)
{	size_t i, j, k;

	// set information needed in ipopt_cppad_fg_info
	fg_info->set_n(n);
	fg_info->set_m(m);

	// get information from derived class version of fg_info
	K_ = fg_info->number_functions();
	L_.resize(K_);
	p_.resize(K_);
	q_.resize(K_);
	r_fun_.resize(K_);
	retape_.resize(K_);
	pattern_jac_r_.resize(K_);
	pattern_r_lag_.resize(K_);
	size_t max_p      = 0;
	size_t max_q      = 0;
	bool   retape_any = false;
	for(k = 0; k < K_; k++)
	{	L_[k]       = fg_info->number_terms(k);
		p_[k]       = fg_info->range_size(k);
		q_[k]       = fg_info->domain_size(k);
		retape_[k]  = fg_info->retape(k);
		max_p       = std::max(max_p, p_[k]);
		max_q       = std::max(max_q, q_[k]);
		retape_any |= retape_[k];
		pattern_jac_r_[k].resize( p_[k] * q_[k] );
		pattern_r_lag_[k].resize( q_[k] * q_[k] );
		if( ! retape_[k] )
		{	// Record both r_k (u) 
			// (operation sequence does not depend on value of u).
			ADVector u_ad(q_[k]);
			for(j = 0; j < q_[k]; j++)
				u_ad[j] = 0.;
			record_r_fun(
				fg_info_, k, p_, q_, u_ad, // inputs
				r_fun_                     // outputs
			);
		}
	}
	I_.resize(max_p);
	J_.resize(max_q);

	pattern_jac_fg_.resize( (m + 1) * n );
	pattern_h_lag_.resize( n * n );

	if ( retape_any )
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
	{	// compute CppAD sparsity partern for Jacobian of fg
		compute_pattern_jac_fg(
		fg_info_, I_, J_, K_, L_, m_, n_, p_, q_, r_fun_,   // inputs
		pattern_jac_r_, pattern_jac_fg_                     // outputs
		);

		// compute CppAD sparsity partern for Hessian of Lagragian
		compute_pattern_h_lag(
		fg_info_, I_, J_, K_, L_, m_, n_, p_, q_, r_fun_,   // inputs
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
	size_t                 k        ,
	SizeVector&            p        ,
	SizeVector&            q        ,
	ADVector&              u_ad     ,
	ADFunVector&           r_fun    )
/*
fg_info: input
the ipopt_cppad_fg_info object that is used to information
about the representation of fg(x).

k: input
index of the function r_k (u)

p: input
p[k] is number of components in the range space for r_k (u).

q: input
q[k] number of components in domain space for r_k (u).

u_ad: input
vector of independent variable values at which to record r_k (u).
This is an input except for 
the fact that its CppAD private data changes.

r_fun: output 
The CppAD operation sequence corresponding to the value of u_ad,
and the algorithm used by fg_info, is stored in r_fun[k]. (Any operation
seqeunce that was previously in r_fun[k] is deleted.)
*/
{	assert( u_ad.size() == size_t(q[k]) );
	// vector of dependent variables during function recording
	ADVector r_ad(p[k]);
	// start the recording
	CppAD::Independent(u_ad);
	// record operations for r(u)
	r_ad = fg_info->r_eval(k, u_ad);
	// stop the resording and store operation sequence in r_fun
	r_fun[k].Dependent(u_ad, r_ad);
}

// static member function that computes CppAD sparsity pattern for 
// Jacobian of fg
void ipopt_cppad_nlp::compute_pattern_jac_fg(
	ipopt_cppad_fg_info*  fg_info        , 
	SizeVector&           I              ,
	SizeVector&           J              ,
	size_t                K              ,
	SizeVector&           L              ,
	size_t                m              ,
	size_t                n              ,
	SizeVector&           p              ,
	SizeVector&           q              ,
	ADFunVector&          r_fun          ,
	BoolVectorVector&     pattern_jac_r  ,
	BoolVector&           pattern_jac_fg )
/*
fg_info: input
the ipopt_cppad_fg_info object that is used to compute 
information about the representation of fg(x).

I: scratch space
Vector of length greater than or equal p[k] for all k.

J: scratch space
Vector of length greatern than or equal q[k] for all k.

K: input
Number of functions in the representation for fg(x) in terms of r_k (u).

L: input
L[k] is the number of terms, in the representaiton for fg(x), that use r_k (u).

m: input
The number of components in the constraint function g.

n: input
Number of indpendent variables.

p: input
for k = 0 , ... , K-1,
p[k] is dimension of the range space for r_k (u).

q: input
for k = 0 , ... , K-1,
p[k] is dimension of the domain space for r_k (u).

r_fun: input
for k = 0 , ... , K-1,
r_fun[k] is the CppAD function object that is used to compute 
the sparsity patterns for r_k (u).
The state of these objects actually changes because some forward mode
routines are used.
The operation sequence correspopnding to these object do not change.

pattern_jac_r: output
For k = 0 , ... , K-1,
on input pattern_jac_r[k], this must be a vector of length p[k] * q[k].
On output it is the CppAD sparsity pattern for the Jacobian of r_k (u).

pattern_jac_fg: output
On input, this must be a vector of length (m+1) * n.
On output it is the CppAD sparsity pattern for the Jacobian of fg(x).
*/
{
	assert( pattern_jac_fg.size() == (m+1) * n );

	size_t i, j, k, ell, ir, ifg;

	for(k = 0; k < K; k++)
	{	// compute sparsity pattern for r_k (u)
		assert( pattern_jac_r[k].size() == p[k] * q[k] );
		assert( r_fun[k].Range() == p[k] );
		assert( r_fun[k].Domain() == q[k] );
		if( q[k] < p[k] )
		{	// use forward mode
			BoolVector pattern_domain(q[k] * q[k]);
			for(i = 0; i < q[k]; i++)
			{	for(j = 0; j < q[k]; j++) 
					pattern_domain[i * q[k] + j] = false;
				pattern_domain[i * q[k] + i] = true;
			}
			pattern_jac_r[k] = 
				r_fun[k].ForSparseJac(q[k], pattern_domain);
		}
		else
		{	// use reverse mode
			BoolVector pattern_range(p[k] * p[k]);
			for(i = 0; i < p[k]; i++)
			{	for(j = 0; j < p[k]; j++) 
					pattern_range[i * p[k] + j] = false;
				pattern_range[i * p[k] + i] = true;
			}
			pattern_jac_r[k] = 
				r_fun[k].RevSparseJac(p[k], pattern_range);
		}
	}

	// now compute pattern for fg
	j = (m+1) * n;
	while(j--)
		pattern_jac_fg[j] = false;
	for(k = 0; k < K; k++) for(ell = 0; ell < L[k]; ell++)
	{	fg_info->range_index(k, ell, I);	
		fg_info->domain_index(k, ell, J);
		for(i = 0; i < p[k]; i++)
		{	for(j = 0; j < q[k]; j++)
			{	ir  = i * q[k] + j;
				ifg = I[i] * n + J[j];
				pattern_jac_fg[ifg] = ( pattern_jac_fg[ifg] 
				                    | pattern_jac_r[k][ir]    );
			}
		}
	}
}

// static member function that computes CppAD sparsity pattern for 
// Hessian of Lagragian
void ipopt_cppad_nlp::compute_pattern_h_lag(
	ipopt_cppad_fg_info  *fg_info        , 
	SizeVector&           I              ,
	SizeVector&           J              ,
	size_t                K              ,
	SizeVector&           L              ,
	size_t                m              ,
	size_t                n              ,
	SizeVector&           p              ,
	SizeVector&           q              ,
	ADFunVector&          r_fun          ,
	BoolVectorVector&     pattern_r_lag  ,
	BoolVector&           pattern_h_lag  )
/*
fg_info: input
the ipopt_cppad_fg_info object that is used to compute 
information about the representation of fg(x).

I: scratch space
Vector of length greater than or equal p[k] for all k.

J: scratch space
Vector of length greatern than or equal q[k] for all k.

K: input
Number of functions in the representation for fg(x) in terms of r_k (u).

L: input
L[k] is the number of terms, in the representaiton for fg(x), that use r_k (u).

m: input
The number of components in the constraint function g.

n: input
Number of indpendent variables.

p: input
for k = 0 , ... , K-1,
p[k] is dimension of the range space for r_k (u).

q: input
for k = 0 , ... , K-1,
p[k] is dimension of the domain space for r_k (u).

r_fun: input
for k = 0 , ... , K-1,
r_fun[k] is the CppAD function object that is used to compute 
the sparsity patterns for r_k (u).
The state of these objects actually changes because some forward mode
routines are used.
The operation sequence correspopnding to these object do not change.

pattern_r_lag: output
For k = 0 , ... , K-1,
on input pattern_r_lag[k], this must be a vector of length q[k] * q[k].
On output it is the CppAD sparsity pattern for the Hessian of 
a Lagragian that sums components of r_k (u).

pattern_h_lag: output
On input, this must be a vector of length n * n.
On output it is the CppAD sparsity pattern for the Hessian of 
a Lagragian that sums components of fg(x).
*/
{
	assert( pattern_h_lag.size() == n * n );
	size_t i, j, k, ell;

	for(k = 0; k < K; k++)
	{	assert( pattern_r_lag[k].size() == q[k] * q[k] );
		assert( r_fun[k].Range() == p[k] );
		assert( r_fun[k].Domain() == q[k] );

		BoolVector pattern_domain(q[k] * q[k]);
		BoolVector pattern_ones(p[k]);

		for(i = 0; i < q[k]; i++)
		{	for(j = 0; j < q[k]; j++) 
				pattern_domain[i * q[k] + j] = false;
			pattern_domain[i * q[k] + i] = true;
		}
		r_fun[k].ForSparseJac(q[k], pattern_domain);
		for(i = 0; i < p[k]; i++)
			pattern_ones[i] = true;
		pattern_r_lag[k] = r_fun[k].RevSparseHes(q[k], pattern_ones);
	}

	// now compute pattern for fg
	j = (n * n);
	while(j--)
		pattern_h_lag[j] = false;
	for(k = 0; k < K; k++) for(ell = 0; ell < L[k]; ell++)
	{	fg_info->domain_index(k, ell, J);
		for(i = 0; i < q[k]; i++)
		{	for(j = 0; j < q[k]; j++)
			{	size_t ir, ifg;
				ir  = i * q[k] + j;
				ifg = J[i] * n + J[j];
				pattern_h_lag[ifg] = ( pattern_h_lag[ifg] 
				                    | pattern_r_lag[k][ir]    );
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

	size_t iobj, j, k, ell;

	// initialize summation
	obj_value = 0.;

	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{	fg_info_->range_index(k, ell, I_);
		fg_info_->domain_index(k, ell, J_);
		for(iobj = 0; iobj < p_[k]; iobj++) if( I_[iobj] == 0 )
		{	if( (new_x || K_ > 1)  && retape_[k] )
			{	// Record r_k for value of u corresponding to x
				ADVector u_ad(q_[0]);
				for(j = 0; j < q_[k]; j++)
				{	assert( J_[j] < n_ );
					u_ad[j] = x[ J_[j] ];
				}
				record_r_fun(
					fg_info_, k, p_, q_, u_ad,  // inputs
					r_fun_                      // outputs
				);
			}
			NumberVector u(q_[k]);
			NumberVector r(p_[k]);
			for(j = 0; j < q_[k]; j++)
			{	assert( J_[j] < n_ );
				u[j]   = x[ J_[j] ];
			}
			r          = r_fun_[k].Forward(0, u);
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

	size_t iobj, i, j, k, ell;

	// initialize summation
	for(j = 0; j < n_; j++)
		grad_f[j] = 0.;

	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{	fg_info_->range_index(k, ell, I_);
		fg_info_->domain_index(k, ell, J_);
		for(iobj = 0; iobj < p_[k]; iobj++) if( I_[iobj] == 0 )
		{	if( (new_x || K_ > 1)  && retape_[k] )
			{	// Record r_k for value of u corresponding to x
				// If we has stored all recordings in f_eval
				// we might not need to do this over again.
				ADVector u_ad(q_[k]);
				for(j = 0; j < q_[k]; j++)
				{	assert( J_[j] < n_ );
					u_ad[j] = x[ J_[j] ];
				}
				record_r_fun(
					fg_info_, k, p_, q_, u_ad,  // inputs
					r_fun_                      // outputs
				);
			}
			NumberVector u(q_[k]);
			NumberVector w(p_[k]);
			NumberVector r_grad(q_[k]);
			for(j = 0; j < q_[k]; j++)
			{	assert( J_[j] < n_ );
				u[j]   = x[ J_[j] ];
			}
			r_fun_[k].Forward(0, u);
			for(i = 0; i < p_[k]; i++)
				w[i] = 0.;
			w[iobj]    = 1.;
			r_grad     = r_fun_[k].Reverse(1, w);
			for(j = 0; j < q_[k]; j++)
			{	assert( J_[j] < n_ );
				grad_f[ J_[j] ]  += r_grad[j];
			}
		}
	}
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

	size_t i, j, k, ell;

	// initialize summation
	for(i = 0; i < m_; i++)
		g[i] = 0.;

	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{	fg_info_->range_index(k, ell, I_);
		fg_info_->domain_index(k, ell, J_);
		if( (new_x || K_ > 1)  && retape_[k] )
		{	// Record r_k for value of u corresponding to x
			ADVector     u_ad(q_[k]);
			for(j = 0; j < q_[k]; j++)
			{	assert( J_[j] < n_ );
				u_ad[j] = x[ J_[j] ];
			}
			record_r_fun(
				fg_info_, k, p_, q_, u_ad,  // inputs
				r_fun_                      // outputs
			);
		}
		NumberVector u(q_[k]);
		NumberVector r(p_[k]);
		for(j = 0; j < q_[k]; j++)
		{	assert( J_[j] < n_ );
			u[j]   = x[ J_[j] ];
		}
		r   = r_fun_[k].Forward(0, u);
		for(i = 0; i < p_[k]; i++)
		{	assert( I_[i] <= m_ );
			if( I_[i] >= 1 )
				g[ I_[i] - 1 ] += r[i];
		}
	}
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

	size_t i, j, k, ell;

	if (values == NULL) 
	{	for(k = 0; k < nnz_jac_g_; k++)
		{	iRow[k] = iRow_jac_g_[k];
			jCol[k] = jCol_jac_g_[k];
		}
		return true;
	}

	// initialize summation
	// This stores the entire Jacobian including zeros (wastes space ?)
	NumberVector jac_fg((m_ + 1) * n_);
	i = (m_ + 1) * n_; 
	while(i--)
		jac_fg[i] = 0.;

	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{	fg_info_->range_index(k, ell, I_);
		fg_info_->domain_index(k, ell, J_);
		if( (new_x || K_ > 1)  && retape_[k] )
		{	// Record r_k for value of u corresponding to x
			ADVector     u_ad(q_[k]);
			for(j = 0; j < q_[k]; j++)
			{	assert( J_[j] < n_ );
				u_ad[j] = x[ J_[j] ];
			}
			record_r_fun(
				fg_info_, k, p_, q_, u_ad,  // inputs
				r_fun_                      // outputs
			);
		}
		NumberVector u(q_[k]);
		NumberVector jac_r(p_[k] * q_[k]);
		for(j = 0; j < q_[k]; j++)
		{	assert( J_[j] < n_ );
			u[j]   = x[ J_[j] ];
		}
		if( retape_[k] )
			jac_r = r_fun_[k].Jacobian(u);
		else	jac_r = r_fun_[k].SparseJacobian(u, pattern_jac_r_[k]);
		for(i = 0; i < p_[k]; i++)
		{	assert( I_[i] <= m_ );
			for(j = 0; j < q_[k]; j++)
				jac_fg[ I_[i] * n_ + J_[j] ] += 
					jac_r[i * q_[k] + j];
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

	size_t i, j, k, ell;

	if (values == NULL) 
	{	for(k = 0; k < nnz_h_lag_; k++)
		{	iRow[k] = iRow_h_lag_[k];
			jCol[k] = jCol_h_lag_[k];
		}
		return true;
	}

	// initialize Jacobian of fg
	NumberVector fg_hes(n_ * n_);
	j = n_ * n_; 
	while(j--)
		fg_hes[j] = 0.;

	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{	fg_info_->range_index(k, ell, I_);
		fg_info_->domain_index(k, ell, J_);
		if( (new_x || K_ > 1)  && retape_[k] )
		{	// Record r_k for value of u corresponding to x
			ADVector     u_ad(q_[k]);
			for(j = 0; j < q_[k]; j++)
			{	assert( J_[j] < n_ );
				u_ad[j] = x[ J_[j] ];
			}
			record_r_fun(
				fg_info_, k, p_, q_, u_ad,  // inputs
				r_fun_                      // outputs
			);
		}
		NumberVector w(p_[k]);
		NumberVector r_hes(q_[k] * q_[k]);
		NumberVector u(q_[k]);
		for(j = 0; j < q_[k]; j++)
		{	assert( J_[j] < n_ );
			u[j]   = x[ J_[j] ];
		}
		for(i = 0; i < p_[k]; i++)
		{	assert( I_[i] <= m_ );
			if( I_[i] == 0 )
				w[i] = obj_factor;
			else	w[i] = lambda[ I_[i] - 1 ];
		}
		if( retape_[k] )
			r_hes = r_fun_[k].Hessian(u, w);
		else	r_hes = 
			r_fun_[k].SparseHessian(u, w, pattern_r_lag_[k]);
		for(i = 0; i < q_[k]; i++)
		{	for(j = 0; j < q_[k]; j++)
				fg_hes[ J_[i] * n_ + J_[j] ] += 
					r_hes[i * q_[k] + j];
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
