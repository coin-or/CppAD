/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include "cppad_ipopt_nlp.hpp"
# include "sparse_map2vec.hpp"
# include "jac_g_map.hpp"
# include "hes_fg_map.hpp"
# include "vec_fun_pattern.hpp"

// define as 0 for false or 1 for true
# define  CPPAD_NLP_TRACE 0

# if CPPAD_NLP_TRACE
# include <cstdio>
# endif

/* Constructor. */
cppad_ipopt_nlp::cppad_ipopt_nlp(
	size_t                     n           , 
	size_t                     m           , 
	const NumberVector&        x_i         ,
	const NumberVector&        x_l         ,
	const NumberVector&        x_u         ,
	const NumberVector&        g_l         ,
	const NumberVector&        g_u         ,
	cppad_ipopt_fg_info*       fg_info     ,
	cppad_ipopt_solution*      solution    )
	: n_ ( n ),
	  m_ ( m ),
	  x_i_ ( x_i ),
	  x_l_ ( x_l ),
	  x_u_ ( x_u ),
	  g_l_ ( g_l ),
	  g_u_ ( g_u ),
	  fg_info_ ( fg_info ) ,
	  solution_ (solution)
{	size_t j, k;

	// set information needed in cppad_ipopt_fg_info
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
	for(k = 0; k < K_; k++)
	{	L_[k]       = fg_info->number_terms(k);
		p_[k]       = fg_info->range_size(k);
		q_[k]       = fg_info->domain_size(k);
		retape_[k]  = fg_info->retape(k);
		max_p       = std::max(max_p, p_[k]);
		max_q       = std::max(max_q, q_[k]);
		pattern_jac_r_[k].resize( p_[k] * q_[k] );
		pattern_r_lag_[k].resize( q_[k] * q_[k] );
	}
	I_.resize(max_p);
	J_.resize(max_q);
# ifndef NDEBUG
	size_t i, ell;
	// check for valid range and domain indices
	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{
		for( i = 0; i < p_[k]; i++)
			I_[i] = m+1; // an invalid range index
		for( j = 0; j < q_[k]; j++)
			J_[j] = n; // an invalid domain index
		fg_info->index(k, ell, I_, J_);	
		for( i = 0; i < p_[k]; i++) if( I_[i] > m )
		{	std::cerr << "k=" << k << ", ell=" << ell 
			<< ", I[" << i << "]=" << I_[i] << std::endl;
		 	CPPAD_ASSERT_KNOWN( I_[i] <= m,
			"cppad_ipopt_nlp: invalid value in index vector I"
			);
		}
		for( j = 0; j < q_[k]; j++) if( J_[j] >= n )
		{	std::cerr << "k=" << k << ", ell=" << ell 
			<< ", J[" << j << "]=" << J_[j] << std::endl;
			CPPAD_ASSERT_KNOWN( J_[j] < n,
			"cppad_ipopt_nlp: invalid value in index vector J"
			);
		}
	}
# endif
	// record r[k] for functions that do not need retaping
	for(k = 0; k < K_; k++) if( ! retape_[k] )
	{	// Record r_k (u): operation sequence does not depend on value 
		// of u but record at initial value to make debugging easier.
		fg_info->index(k, 0, I_, J_);
		ADVector u_ad(q_[k]);
		for(j = 0; j < q_[k]; j++)
			u_ad[j] = x_i[ J_[j] ];
		record_r_fun(
			fg_info_, k, p_, q_, u_ad, // inputs
			r_fun_                     // outputs
		);
	}

	// compute a sparsity patterns for each r_k (u)
	vec_fun_pattern(
		K_, p_, q_, retape_, r_fun_,      // inputs 
		pattern_jac_r_, pattern_r_lag_    // outputs
	);

	// mapping from (i,j) to Ipopt sparsity index for Jacobian of g
	jac_g_map(
		fg_info_, m_, n_, K_, L_, p_, q_, pattern_jac_r_,   // inputs
		I_, J_,                                             // work
		index_jac_g_                                        // outputs
	);

	// mapping from (i,j) to Ipopt sparsity index for Hessian of Lagragian
	hes_fg_map(
		fg_info_, m_, n_, K_, L_, p_, q_, pattern_r_lag_,   // inputs
		I_, J_,                                             // work
		index_hes_fg_                                       // outputs
	);
	
	// Compute Ipopt sparsity structure for Jacobian of g 
	sparse_map2vec(
		index_jac_g_,                         // inputs
		nnz_jac_g_, iRow_jac_g_, jCol_jac_g_  // outputs
	);

	// Compute Ipopt sparsity structure for Hessian of Lagragian
	sparse_map2vec(
		index_hes_fg_,                        // inputs
		nnz_h_lag_, iRow_h_lag_, jCol_h_lag_  // outputs
	);

}

// static member function that records operation sequence
void cppad_ipopt_nlp::record_r_fun( 
	cppad_ipopt_fg_info*   fg_info  , 
	size_t                 k        ,
	SizeVector&            p        ,
	SizeVector&            q        ,
	ADVector&              u_ad     ,
	ADFunVector&           r_fun    )
/*
fg_info: input
the cppad_ipopt_fg_info object that is used to information
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
{	CPPAD_ASSERT_UNKNOWN( u_ad.size() == size_t(q[k]) );
	// start the recording
	CppAD::Independent(u_ad);
	// vector of dependent variables during function recording
	ADVector r_ad = fg_info->eval_r(k, u_ad);
	CPPAD_ASSERT_KNOWN( r_ad.size() == p[k] ,
		"cppad_ipopt_nlp: eval_r return value size not equal to p[k]."
	);
	// stop the recording and store operation sequence in r_fun
	r_fun[k].Dependent(u_ad, r_ad);
}

cppad_ipopt_nlp::~cppad_ipopt_nlp()
{}

bool cppad_ipopt_nlp::get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
                         Index& nnz_h_lag, IndexStyleEnum& index_style)
{
	n = n_;
	m = m_;
	nnz_jac_g = nnz_jac_g_;
	nnz_h_lag = nnz_h_lag_;

  	// use the fortran index style for row/col entries
	index_style = C_STYLE;

	return true;
}

bool cppad_ipopt_nlp::get_bounds_info(Index n, Number* x_l, Number* x_u,
                            Index m, Number* g_l, Number* g_u)
{	size_t i, j;
	// here, the n and m we gave IPOPT in get_nlp_info are passed back 
	CPPAD_ASSERT_UNKNOWN(size_t(n) == n_);
	CPPAD_ASSERT_UNKNOWN(size_t(m) == m_);

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

bool cppad_ipopt_nlp::get_starting_point(Index n, bool init_x, Number* x,
                               bool init_z, Number* z_L, Number* z_U,
                               Index m, bool init_lambda,
                               Number* lambda)
{	size_t j;

	CPPAD_ASSERT_UNKNOWN(size_t(n) == n_ );
	CPPAD_ASSERT_UNKNOWN(size_t(m) == m_ );
	CPPAD_ASSERT_UNKNOWN(init_x == true);
	CPPAD_ASSERT_UNKNOWN(init_z == false);
	CPPAD_ASSERT_UNKNOWN(init_lambda == false);

	for(j = 0; j < n_; j++)
		x[j] = x_i_[j];

	return true;
}

bool cppad_ipopt_nlp::eval_f(
	Index n, const Number* x, bool new_x, Number& obj_value
)
{	// This routine is not efficient. It would be better if users version
	// of the function was templated so that we could use double to 
	// calculate function values instead of retaping (when necessary).
	CPPAD_ASSERT_UNKNOWN(size_t(n) == n_ );

	size_t iobj, j, k, ell;

	// initialize summation
	obj_value = 0.;

	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{	fg_info_->index(k, ell, I_, J_);
		for(iobj = 0; iobj < p_[k]; iobj++) if( I_[iobj] == 0 )
		{	if( (new_x || K_ > 1)  && retape_[k] )
			{	// Record r_k for value of u corresponding to x
				ADVector u_ad(q_[k]);
				for(j = 0; j < q_[k]; j++)
				{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
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
			{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
				u[j]   = x[ J_[j] ];
			}
			r          = r_fun_[k].Forward(0, u);
			obj_value += r[iobj];
		}
	}
# if CPPAD_NLP_TRACE
	using std::printf;
	for(j = 0; j < n_; j++)
		printf("cppad_ipopt_nlp::eval_f::x[%d] = %20.14g\n", j, x[j]);
	printf("cppad_ipopt_nlp::eval_f::obj_value = %20.14g\n", obj_value);
# endif
	return true;
}

bool cppad_ipopt_nlp::eval_grad_f(
	Index n, const Number* x, bool new_x, Number* grad_f
)
{	CPPAD_ASSERT_UNKNOWN(size_t(n) == n_ );

	size_t iobj, i, j, k, ell;

	// initialize summation
	for(j = 0; j < n_; j++)
		grad_f[j] = 0.;

	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{	fg_info_->index(k, ell, I_, J_);
		for(iobj = 0; iobj < p_[k]; iobj++) if( I_[iobj] == 0 )
		{	if( (new_x || K_ > 1)  && retape_[k] )
			{	// Record r_k for value of u corresponding to x
				// If we has stored all recordings in f_eval
				// we might not need to do this over again.
				ADVector u_ad(q_[k]);
				for(j = 0; j < q_[k]; j++)
				{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
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
			{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
				u[j]   = x[ J_[j] ];
			}
			r_fun_[k].Forward(0, u);
			for(i = 0; i < p_[k]; i++)
				w[i] = 0.;
			w[iobj]    = 1.;
			r_grad     = r_fun_[k].Reverse(1, w);
			for(j = 0; j < q_[k]; j++)
			{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
				grad_f[ J_[j] ]  += r_grad[j];
			}
		}
	}
# if CPPAD_NLP_TRACE
	using std::printf;
	for(j = 0; j < n_; j++) printf(
	"cppad_ipopt_nlp::eval_grad_f::x[%d] = %20.14g\n", j, x[j]
	);
	for(j = 0; j < n_; j++) printf(
	"cppad_ipopt_nlp::eval_grad_f::grad_f[%d] = %20.14g\n", j, grad_f[j]
	);
# endif
	return true;
}

bool cppad_ipopt_nlp::eval_g(
	Index n, const Number* x, bool new_x, Index m, Number* g
)
{	CPPAD_ASSERT_UNKNOWN(size_t(n) == n_ );

	size_t i, j, k, ell;

	// initialize summation
	for(i = 0; i < m_; i++)
		g[i] = 0.;

	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{	fg_info_->index(k, ell, I_, J_);
		if( (new_x || K_ > 1)  && retape_[k] )
		{	// Record r_k for value of u corresponding to x
			ADVector     u_ad(q_[k]);
			for(j = 0; j < q_[k]; j++)
			{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
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
		{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
			u[j]   = x[ J_[j] ];
		}
		r   = r_fun_[k].Forward(0, u);
		for(i = 0; i < p_[k]; i++)
		{	CPPAD_ASSERT_UNKNOWN( I_[i] <= m_ );
			if( I_[i] >= 1 )
				g[ I_[i] - 1 ] += r[i];
		}
	}
# if CPPAD_NLP_TRACE
	using std::printf;
	for(j = 0; j < n_; j++)
		printf("cppad_ipopt_nlp::eval_g::x[%d] = %20.14g\n", j, x[j]);
	for(i = 0; i < m_; i++)
		printf("cppad_ipopt_nlp::eval_g::g[%d] = %20.14g\n", i, g[i]);
# endif
	return true;
}

bool cppad_ipopt_nlp::eval_jac_g(Index n, const Number* x, bool new_x,
                       Index m, Index nele_jac, Index* iRow, Index *jCol,
                       Number* values)
{	CPPAD_ASSERT_UNKNOWN(size_t(m) == m_ );
	CPPAD_ASSERT_UNKNOWN(size_t(n) == n_ );

	size_t i, j, k, ell, l;
	std::map<size_t,size_t>::iterator index_ij;


	if (values == NULL) 
	{	for(k = 0; k < nnz_jac_g_; k++)
		{	iRow[k] = iRow_jac_g_[k];
			jCol[k] = jCol_jac_g_[k];
		}
		return true;
	}

	// initialize summation
	l = nnz_jac_g_;
	while(l--)
		values[l] = 0.;

	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{	fg_info_->index(k, ell, I_, J_);
		if( (new_x || K_ > 1)  && retape_[k] )
		{	// Record r_k for value of u corresponding to x
			ADVector     u_ad(q_[k]);
			for(j = 0; j < q_[k]; j++)
			{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
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
		{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
			u[j]   = x[ J_[j] ];
		}
		if( retape_[k] )
			jac_r = r_fun_[k].Jacobian(u);
		else	jac_r = r_fun_[k].SparseJacobian(u, pattern_jac_r_[k]);
		for(i = 0; i < p_[k]; i++) if( I_[i] != 0 )
		{	CPPAD_ASSERT_UNKNOWN( I_[i] <= m_ );
			for(j = 0; j < q_[k]; j++)
			{	index_ij = index_jac_g_[I_[i]-1].find(J_[j]);
				if( index_ij != index_jac_g_[I_[i]-1].end() )
				{	l          = index_ij->second;
					values[l] += jac_r[i * q_[k] + j];
				}
				else	CPPAD_ASSERT_UNKNOWN(
					jac_r[i * q_[k] + j] == 0.
				);
			}
		}
	}
  	return true;
}

bool cppad_ipopt_nlp::eval_h(Index n, const Number* x, bool new_x,
                   Number obj_factor, Index m, const Number* lambda,
                   bool new_lambda, Index nele_hess, Index* iRow,
                   Index* jCol, Number* values)
{	CPPAD_ASSERT_UNKNOWN(size_t(m) == m_ );
	CPPAD_ASSERT_UNKNOWN(size_t(n) == n_ );

	size_t i, j, k, ell, l;
	std::map<size_t,size_t>::iterator index_ij;

	if (values == NULL) 
	{	for(k = 0; k < nnz_h_lag_; k++)
		{	iRow[k] = iRow_h_lag_[k];
			jCol[k] = jCol_h_lag_[k];
		}
		return true;
	}

	// initialize summation
	l = nnz_h_lag_;
	while(l--)
		values[l] = 0.;

	for(k = 0; k < K_; k++) for(ell = 0; ell < L_[k]; ell++)
	{	fg_info_->index(k, ell, I_, J_);
		if( (new_x || K_ > 1)  && retape_[k] )
		{	// Record r_k for value of u corresponding to x
			ADVector     u_ad(q_[k]);
			for(j = 0; j < q_[k]; j++)
			{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
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
		{	CPPAD_ASSERT_UNKNOWN( J_[j] < n_ );
			u[j]   = x[ J_[j] ];
		}
		for(i = 0; i < p_[k]; i++)
		{	CPPAD_ASSERT_UNKNOWN( I_[i] <= m_ );
			if( I_[i] == 0 )
				w[i] = obj_factor;
			else	w[i] = lambda[ I_[i] - 1 ];
		}
		if( retape_[k] )
			r_hes = r_fun_[k].Hessian(u, w);
		else	r_hes = 
			r_fun_[k].SparseHessian(u, w, pattern_r_lag_[k]);
		for(i = 0; i < q_[k]; i++)
		{	for(j = 0; j < q_[k]; j++) if( J_[j] <= J_[i] ) 
			{	index_ij = index_hes_fg_[J_[i]].find(J_[j]);
				if( index_ij != index_hes_fg_[J_[i]].end() )
				{	l          = index_ij->second;
					values[l] += r_hes[i * q_[k] + j];
				}
				else	CPPAD_ASSERT_UNKNOWN(
					r_hes[i * q_[k] + j] == 0.
				);
			}
		}
	}
	return true;
}

void cppad_ipopt_nlp::finalize_solution(
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

	CPPAD_ASSERT_UNKNOWN(size_t(n) == n_ );
	CPPAD_ASSERT_UNKNOWN(size_t(m) == m_ );

	switch(status)
	{	// convert status from Ipopt enum to cppad_ipopt_solution enum
		case Ipopt::SUCCESS:
		solution_->status = 
			cppad_ipopt_solution::success;
		break;

		case Ipopt::MAXITER_EXCEEDED:
		solution_->status = 
			cppad_ipopt_solution::maxiter_exceeded;
		break;

		case Ipopt::STOP_AT_TINY_STEP:
		solution_->status = 
			cppad_ipopt_solution::stop_at_tiny_step;
		break;

		case Ipopt::STOP_AT_ACCEPTABLE_POINT:
		solution_->status = 
			cppad_ipopt_solution::stop_at_acceptable_point;
		break;

		case Ipopt::LOCAL_INFEASIBILITY:
		solution_->status = 
			cppad_ipopt_solution::local_infeasibility;
		break;

		case Ipopt::USER_REQUESTED_STOP:
		solution_->status = 
			cppad_ipopt_solution::user_requested_stop;
		break;

		case Ipopt::DIVERGING_ITERATES:
		solution_->status = 
			cppad_ipopt_solution::diverging_iterates;
		break;

		case Ipopt::RESTORATION_FAILURE:
		solution_->status = 
			cppad_ipopt_solution::restoration_failure;
		break;

		case Ipopt::ERROR_IN_STEP_COMPUTATION:
		solution_->status = 
			cppad_ipopt_solution::error_in_step_computation;
		break;

		case Ipopt::INVALID_NUMBER_DETECTED:
		solution_->status = 
			cppad_ipopt_solution::invalid_number_detected;
		break;

		case Ipopt::INTERNAL_ERROR:
		solution_->status = 
			cppad_ipopt_solution::internal_error;
		break;

		default:
		solution_->status = 
			cppad_ipopt_solution::unknown;
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
