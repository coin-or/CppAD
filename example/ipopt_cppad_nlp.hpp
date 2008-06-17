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

#include <coin/IpTNLP.hpp>
#include <cppad/cppad.hpp>

using Ipopt::Number;
using Ipopt::Index;

typedef CppAD::AD<Number>       ADNumber;
typedef CppAD::vector<Number>   NumberVector;
typedef CppAD::vector<ADNumber> ADVector;
typedef CppAD::vectorBool       BoolVector;
typedef CppAD::vector<Index>    IndexVector;
typedef ADVector (*FgPointer) (const ADVector& x);

/* 
C++ class for interfacing a problem to IPOPT and using CppAD for derivative 
and sparsity pattern calculations.
*/
class ipopt_cppad_solution 
{
public:
	Ipopt::SolverReturn        status;
	NumberVector               x;
	NumberVector               z_l;
	NumberVector               z_u;
	NumberVector               g;
	NumberVector               lambda;
	Number                     obj_value;
};

class ipopt_cppad_nlp : public Ipopt::TNLP
{
	using Ipopt::TNLP::IndexStyleEnum;
public:
	// constructor 
	ipopt_cppad_nlp(
		Index n                          , 
		Index m                          ,
		const NumberVector    &x_i       ,
		const NumberVector    &x_l       ,
		const NumberVector    &x_u       ,
		const NumberVector    &g_l       ,
		const NumberVector    &g_u       ,
		FgPointer              fg        ,
		bool                   retape    ,
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
		Index n, 
		bool init_x, 
		Number* x,
		bool init_z, 
		Number* z_L, 
		Number* z_U,
		Index m, 
		bool init_lambda,
		Number* lambda);

	// return the objective value 
	virtual bool eval_f(
		Index n, 
		const Number* x, 
		bool new_x, 
		Number& obj_value);

	// Method to return the gradient of the objective 
	virtual bool eval_grad_f(
		Index n, 
		const Number* x, 
		bool new_x, 
		Number* grad_f
  	);

	// return the constraint residuals
	virtual bool eval_g(
		Index n, 
		const Number* x, 
		bool new_x, 
		Index m, 
		Number* g);

	// Method to return:
	// 1) The structure of the jacobian (if "values" is NULL)
	// 2) The values of the jacobian (if "values" is not NULL)
	virtual bool eval_jac_g(
		Index n, 
		const Number* x, 
		bool new_x,
		Index m, 
		Index nele_jac, 
		Index* iRow, 
		Index *jCol,
		Number* values);

	// Method to return:
	//  1) structure of hessian of the lagrangian (if "values" is NULL)
	//  2) values of hessian of the lagrangian (if "values" is not NULL)
	virtual bool eval_h(
		Index n, 
		const Number* x, 
		bool new_x,
		Number obj_factor, 
		Index m, 
		const Number* lambda,
		bool new_lambda, 
		Index nele_hess, 
		Index* iRow,
		Index* jCol, 
		Number* values);

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
	const Index                     n_;
	// number of components in g
	const Index                     m_;
	// initial x
	const NumberVector              x_i_;
	// limits for x and g
	const NumberVector              x_l_;
	const NumberVector              x_u_;
	const NumberVector              g_l_;
	const NumberVector              g_u_;
	// Users function that evaluates f and g
	FgPointer const                fg_;
	// does operation sequence chage with argument value
	const bool                    retape_;
	// object for storing final solution results
	ipopt_cppad_solution* const   solution_;
	/*
 	Computed values
	*/
	// CppAD sparsity patterns
	BoolVector           pattern_jac_fg_;
	BoolVector           pattern_h_lag_;
	// Ipopt sparsity structure for Jacobian of g
	Index                nnz_jac_g_;
	IndexVector          iRow_jac_g_;
	IndexVector          jCol_jac_g_;
	// Ipopt sparsity structure for Hessian of Lagragian
	Index                nnz_h_lag_;
	IndexVector          iRow_h_lag_;
	IndexVector          jCol_h_lag_;
	// CppAD function object for both f and g as one function
	CppAD::ADFun<Number> fg_fun_;
	/*
 	Methods
	*/
	// Methods to block default compiler methods.
	ipopt_cppad_nlp(const ipopt_cppad_nlp&);
	ipopt_cppad_nlp& operator=(const ipopt_cppad_nlp&);

	// Methods used by public methods
	static void record_fg_fun(
		Index                 m      ,
		Index                 n      ,
		ADVector&             x_vec  , 
		FgPointer fg                 , 
		CppAD::ADFun<Number>& fg_fun
	);
	static void compute_pattern_jac_fg(
		Index                 m              ,
		Index                 n              ,
		CppAD::ADFun<Number>& fg_fun         ,
		BoolVector&           pattern_jac_fg 
	);
	static void compute_pattern_h_lag(
		Index                 m              ,
		Index                 n              ,
		CppAD::ADFun<Number>& fg_fun         ,
		BoolVector&           pattern_h_lag 
	);
	static void compute_structure_jac_g(
		Index                 m              ,
		Index                 n              ,
		const BoolVector&     pattern_jac_fg ,
		Index&                nnz_jac_g      ,
		IndexVector&          iRow_jac_g     ,
		IndexVector&          jCol_jac_g
	);
	static void compute_structure_h_lag(
		Index                 m              ,
		Index                 n              ,
		const BoolVector&     pattern_h_lag  ,
		Index&                nnz_h_lag      ,
		IndexVector&          iRow_h_lag     ,
		IndexVector&          jCol_h_lag
	);

};



# endif
