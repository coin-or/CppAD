/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin user_atomic.cpp$$

$section Define User Atomic Operations: Example and Test$$

$index user_atomic, example$$
$index user_atomic, test$$
$index matrix, atomic example$$

$code
$verbatim%example/user_atomic.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>

// Define matrix multply an an atomic AD<double> operation
namespace { // Empty namespace 
	using CppAD::vector;

	// ------------------- Static Variables ------------------------------
	// Information we will attach to each matrix multiply call
	struct call_info {
		size_t nr_result;
		size_t n_middle;
		size_t nc_result;
		vector<bool>  vx;
	};
	vector<call_info> info_;

	// number of orders for this operation (k + 1)
	size_t n_order_ = 0;
	// number of rows in the result matrix
	size_t nr_result_ = 0;
	// number of columns in left matrix and number of rows in right matrix
	size_t n_middle_ = 0;
	// number of columns in the result matrix
	size_t nc_result_ = 0;
	// which components of x are variables
	vector<bool>* vx_ = 0;

	// -------------- Helper Functions -----------------------------------
	// get the information corresponding to this call
	void get_info(size_t id, size_t k, size_t n, size_t m)
	{	n_order_   = k + 1;	
		nr_result_ = info_[id].nr_result; 
		n_middle_  = info_[id].n_middle;
		nc_result_ = info_[id].nc_result;
		vx_        = &(info_[id].vx);

		assert(n == nr_result_ * n_middle_ + n_middle_ * nc_result_);
		assert(m ==  nr_result_ * nc_result_);
	}
	// index in tx of Taylor coefficient of order ell for left[i,j]
	size_t left(size_t i, size_t j, size_t ell)
	{	assert( i < nr_result_ );
		assert( j < n_middle_ );
		return (i * n_middle_ + j) * n_order_ + ell;
	}

	// index in tx of Taylor coefficient of order ell for right[i,j]
	size_t right(size_t i, size_t j, size_t ell)
	{	assert( i < n_middle_ );
		assert( j < nc_result_ );
		size_t offset = nr_result_ * n_middle_;
		return (offset + i * nc_result_ + j) * n_order_ + ell;
	}

	// index in ty of Taylor coefficinet of order ell for result[i,j]
	size_t result(size_t i, size_t j, size_t ell)
	{	assert( i < nr_result_ );
		assert( j < nc_result_ );
		return (i * nc_result_ + j) * n_order_ + ell;
	}

	// forward mode, multiply left times right and sum into result
	void multiply_and_sum(
		size_t                order_left , 
		size_t                order_right, 
		const vector<double>&         tx ,
		vector<double>&               ty ) 
	{	size_t i, j;
		size_t order_result = order_left + order_right; 
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
			{	double sum = 0.;
				size_t middle, im_left, mj_right, ij_result;
				for(middle = 0; middle < n_middle_; middle++)
				{	im_left  = left(i, middle, order_left);
					mj_right = right(middle, j, order_right);
					sum     += tx[im_left] * tx[mj_right];
				}
				ij_result = result(i, j, order_result);
				ty[ ij_result ] += sum;
			}
		}
		return;
	}

	// reverse mode, compute partials for this order and sum into px, py
	void reverse_multiply(
		size_t                order_left , 
		size_t                order_right, 
		const vector<double>&         tx ,
		const vector<double>&         ty ,
		vector<double>&               px ,
		const vector<double>          py ) 
	{	size_t i, j;
		size_t order_result = order_left + order_right; 
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
			{	size_t middle, im_left, mj_right, ij_result;
				for(middle = 0; middle < n_middle_; middle++)
				{	ij_result = result(i, j, order_result);
					im_left   = left(i, middle, order_left);
					mj_right  = right(middle, j, order_right);
					// sum       += tx[im_left]  * tx[mj_right];
					px[im_left]  += tx[mj_right] * py[ij_result];
					px[mj_right] += tx[im_left]  * py[ij_result];
				}
			}
		}
		return;
	}

	void my_union(
		std::set<size_t>&         result  ,
		const std::set<size_t>&   left    ,
		const std::set<size_t>&   right   )
	{	std::set<size_t> temp;
		std::set_union(
			left.begin()              ,
			left.end()                ,
			right.begin()             ,
			right.end()               ,
			std::inserter(temp, temp.begin())
		);
		result.swap(temp);
	}

	// ----------------------------------------------------------------------
	// forward mode routine called by CppAD
	bool mat_mul_forward(
		size_t                   id ,
		size_t                    k ,
		size_t                    n ,
		size_t                    m ,
		const vector<bool>&      vx ,
		vector<bool>&            vy ,
		const vector<double>&    tx ,
		vector<double>&          ty
	)
	{	size_t i, j, ell;
		get_info(id, k, n, m);

		// check if this is during the call to mat_mul(id, ax, ay)
		if( vx.size() > 0 )
		{	assert( k == 0 && vx.size() > 0 );

			// store the vx information in info_
			assert( vx_->size() == 0 );
			info_[id].vx.resize(n);
			for(j = 0; j < n; j++)
				info_[id].vx[j] = vx[j];
			assert( vx_->size() == n );
			
			// now compute vy
			for(i = 0; i < nr_result_; i++)
			{	for(j = 0; j < nc_result_; j++)
				{	// compute vy[ result(i, j, 0) ]
					bool   var = false;
					bool   nz_left, nz_right;
					size_t middle, im_left, mj_right, ij_result;
					for(middle = 0; middle < n_middle_; middle++)
					{	im_left  = left(i, middle, k);
						mj_right = right(middle, j, k);
						nz_left  = vx[im_left]  | (tx[im_left] != 0.);
						nz_right = vx[mj_right] | (tx[mj_right]!= 0.);
						// if not multiplying by the constant zero
						if( nz_left & nz_right )
							var |= (vx[im_left] | vx[mj_right]);
					}
					ij_result     = result(i, j, k);
					vy[ij_result] = var;
				}
			}
		}

		// initialize result as zero
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
				ty[ result(i, j, k) ] = 0.;
		}
		// sum the product of proper orders
		for(ell = 0; ell <=k; ell++)
			multiply_and_sum(ell, k-ell, tx, ty);

		// All orders are implemented and there are no possible error
		// conditions, so always return true.
		return true;
	}

	// ----------------------------------------------------------------------
	// reverse mode routine called by CppAD
	bool mat_mul_reverse(
		size_t                   id ,
		size_t                    k ,
		size_t                    n ,
		size_t                    m ,
		const vector<double>&    tx ,
		const vector<double>&    ty ,
		vector<double>&          px ,
		const vector<double>&    py
	)
	{	get_info(id, k, n, m);

		size_t ell = n * n_order_;
		while(ell--)
			px[ell] = 0.;

		size_t order = n_order_;
		while(order--)
		{	// reverse sum the products for specified order
			for(ell = 0; ell <=order; ell++)
				reverse_multiply(ell, order-ell, tx, ty, px, py);
		}

		// All orders are implemented and there are no possible error
		// conditions, so always return true.
		return true;
	}

	// ----------------------------------------------------------------------
	// forward Jacobian sparsity routine called by CppAD
	bool mat_mul_for_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		vector< std::set<size_t> >&           s )
	{	size_t i, j, k, im_left, middle, mj_right, ij_result;
		k = 0;
		get_info(id, k, n, m);
	
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
			{	ij_result = result(i, j, k);
				s[ij_result].clear();
				for(middle = 0; middle < n_middle_; middle++)
				{	im_left   = left(i, middle, k);
					mj_right  = right(middle, j, k);

					// s[ij_result] = union( s[ij_result], r[im_left] )
					my_union(s[ij_result], s[ij_result], r[im_left]);

					// s[ij_result] = union( s[ij_result], r[mj_right] )
					my_union(s[ij_result], s[ij_result], r[mj_right]);
				}
			}
		}
		return true;
	}

	// ----------------------------------------------------------------------
	// reverse Jacobian sparsity routine called by CppAD
	bool mat_mul_rev_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		vector< std::set<size_t> >&           r ,
		const vector< std::set<size_t> >&     s )
	{	size_t i, j, k, im_left, middle, mj_right, ij_result;
		k = 0;
		get_info(id, k, n, m);
	
		for(j = 0; j < n; j++)
			r[j].clear();

		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
			{	ij_result = result(i, j, k);
				for(middle = 0; middle < n_middle_; middle++)
				{	im_left   = left(i, middle, k);
					mj_right  = right(middle, j, k);

					// r[im_left] = union( r[im_left], s[ij_result] )
					my_union(r[im_left], r[im_left], s[ij_result]);

					// r[mj_right] = union( r[mj_right], s[ij_result] )
					my_union(r[mj_right], r[mj_right], s[ij_result]);
				}
			}
		}
		return true;
	}

	// ----------------------------------------------------------------------
	// reverse Hessian sparsity routine called by CppAD
	bool mat_mul_rev_hes_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		const vector<bool>&                   s ,
		vector<bool>&                         t ,
		const vector< std::set<size_t> >&     u ,
		vector< std::set<size_t> >&           v )
	{	size_t i, j, k, im_left, middle, mj_right, ij_result;
		k = 0;
		get_info(id, k, n, m);
	
		for(j = 0; j < n; j++)
		{	t[j] = false;	
			v[j].clear();
		}

		assert( vx_->size() == n );
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
			{	ij_result = result(i, j, k);
				for(middle = 0; middle < n_middle_; middle++)
				{	im_left   = left(i, middle, k);
					mj_right  = right(middle, j, k);

					// back propogate Jacobian sparsity
					t[im_left]   |= s[ij_result];
					t[mj_right]  |= s[ij_result];

					// back propotate Hessian sparsity 
					// v[im_left]  = union( v[im_left],  u[ij_result] )
					// v[mj_right] = union( v[mj_right], u[ij_result] )
					my_union(v[im_left],  v[im_left],  u[ij_result] );
					my_union(v[mj_right], v[mj_right], u[ij_result] );

					// Check for case where the (i,j) result element
					// is in reverse Jacobian and both left and right
					// operands in multiplication are variables 
					if(s[ij_result] & (*vx_)[im_left] & (*vx_)[mj_right])
					{	// v[im_left] = union( v[im_left], r[mj_right] )
						my_union(v[im_left], v[im_left], r[mj_right] );
						// v[mj_right] = union( v[mj_right], r[im_left] )
						my_union(v[mj_right], v[mj_right], r[im_left] );
					}
				}
			}
		}
		return true;
	}

	// ----------------------------------------------------------------------
	// Declare the AD<double> routine mat_mul(id, ax, ay)
	CPPAD_USER_ATOMIC(
		mat_mul                 , 
		CPPAD_TEST_VECTOR       ,
		double                  , 
		mat_mul_forward         , 
		mat_mul_reverse         ,
		mat_mul_for_jac_sparse  ,
		mat_mul_rev_jac_sparse  ,
		mat_mul_rev_hes_sparse  
	)

} // End empty namespace

// ---------------------------------------------------------------------------
// Test the above implementation
bool user_atomic(void)
{	bool ok = true;
	using CppAD::AD;

	// matrix sizes for this test
	size_t nr_result = 2;
	size_t n_middle  = 2;
	size_t nc_result = 2;
	
	// declare the AD<double> vectors ax and ay and X 
	size_t n = nr_result * n_middle + n_middle * nc_result;
	size_t m = nr_result * nc_result;
	CPPAD_TEST_VECTOR< AD<double> > X(4), ax(n), ay(m);
	size_t i, j;
	for(j = 0; j < X.size(); j++)
		X[j] = (j + 1);

	// X is the vector of independent variables
	CppAD::Independent(X);
	// left matrix
	ax[0]  = X[0];  // left[0,0]   = x[0] = 1
	ax[1]  = X[1];  // left[0,1]   = x[1] = 2
	ax[2]  = 5.;    // left[1,0]   = 5
	ax[3]  = 6.;    // left[1,1]   = 6
	// right matrix
	ax[4]  = X[2];  // right[0,0]  = x[2] = 3
	ax[5]  = 7.;    // right[0,1]  = 7
	ax[6]  = X[3];  // right[1,0]  = x[3] = 4 
	ax[7]  = 8.;    // right[1,1]  = 8
	/*
	[ x0 , x1 ] * [ x2 , 7 ] = [ x0*x2 + x1*x3 , x0*7 + x1*8 ]
	[ 5  , 6 ]    [ x3 , 8 ]   [ 5*x2  + 6*x3  , 5*7 + 6*8 ]
	*/

	// The call back routines need to know the dimensions of the matrices.
	// Store information about the matrix multiply for this call to mat_mul.
	call_info info;
	info.nr_result = nr_result;
	info.n_middle  = n_middle;
	info.nc_result = nc_result;
	// info.vx gets set by forward during call to mat_mul below
	assert( info.vx.size() == 0 ); 
	size_t id      = info_.size();
	info_.push_back(info);

	// user defined AD<double> version of matrix multiply
	mat_mul(id, ax, ay);
	//----------------------------------------------------------------------
	// check AD<double>  results
	ok &= ay[0] == (1*3 + 2*4); ok &= Variable( ay[0] );
	ok &= ay[1] == (1*7 + 2*8); ok &= Variable( ay[1] );
	ok &= ay[2] == (5*3 + 6*4); ok &= Variable( ay[2] );
	ok &= ay[3] == (5*7 + 6*8); ok &= Parameter( ay[3] );
	//----------------------------------------------------------------------
	// use mat_mul to define a function g : X -> ay
	CppAD::ADFun<double> G(X, ay);
	// g(x) = [ x0*x2 + x1*x3 , x0*7 + x1*8 , 5*x2  + 6*x3  , 5*7 + 6*8 ]^T
	//----------------------------------------------------------------------
	// Test zero order forward mode evaluation of g(x)
	CPPAD_TEST_VECTOR<double> x( X.size() ), y(m);
	for(j = 0; j <  X.size() ; j++)
		x[j] = j + 2;
	y = G.Forward(0, x);
	ok &= y[0] == x[0] * x[2] + x[1] * x[3];
	ok &= y[1] == x[0] * 7.   + x[1] * 8.;
	ok &= y[2] == 5. * x[2]   + 6. * x[3];
	ok &= y[3] == 5. * 7.     + 6. * 8.;

	//----------------------------------------------------------------------
	// Test first order forward mode evaluation of g'(x) * [1, 2, 3, 4]^T 
	// g'(x) = [ x2, x3, x0, x1 ]
	//         [ 7 ,  8,  0, 0  ]
	//         [ 0 ,  0,  5, 6  ]
	//         [ 0 ,  0,  0, 0  ] 
	CPPAD_TEST_VECTOR<double> dx( X.size() ), dy(m);
	for(j = 0; j <  X.size() ; j++)
		dx[j] = j + 1;
	dy = G.Forward(1, dx);
	ok &= dy[0] == 1. * x[2] + 2. * x[3] + 3. * x[0] + 4. * x[1];
	ok &= dy[1] == 1. * 7.   + 2. * 8.   + 3. * 0.   + 4. * 0.;
	ok &= dy[2] == 1. * 0.   + 2. * 0.   + 3. * 5.   + 4. * 6.;
	ok &= dy[3] == 1. * 0.   + 2. * 0.   + 3. * 0.   + 4. * 0.;

	//----------------------------------------------------------------------
	// Test second order forward mode 
	// g_0^2 (x) = [ 0, 0, 1, 0 ], g_0^2 (x) * [1] = [3]
	//             [ 0, 0, 0, 1 ]              [2]   [4]
	//             [ 1, 0, 0, 0 ]              [3]   [1]
	//             [ 0, 1, 0, 0 ]              [4]   [2]
	CPPAD_TEST_VECTOR<double> ddx( X.size() ), ddy(m);
	for(j = 0; j <  X.size() ; j++)
		ddx[j] = 0.;
	ddy = G.Forward(2, ddx);
	// [1, 2, 3, 4] * g_0^2 (x) * [1, 2, 3, 4]^T = 1*3 + 2*4 + 3*1 + 4*2
	ok &= 2. * ddy[0] == 1. * 3. + 2. * 4. + 3. * 1. + 4. * 2.; 
	// for i > 0, [1, 2, 3, 4] * g_i^2 (x) * [1, 2, 3, 4]^T = 0
	ok &= ddy[1] == 0.;
	ok &= ddy[2] == 0.;
	ok &= ddy[3] == 0.;

	//----------------------------------------------------------------------
	// Test second order reverse mode 
	CPPAD_TEST_VECTOR<double> w(m), dw(2 *  X.size() );
	for(i = 0.; i < m; i++)
		w[i] = 0.;
	w[0] = 1.;
	dw = G.Reverse(2, w);
	// g_0'(x) = [ x2, x3, x0, x1 ]
	ok &= dw[0*2 + 0] == x[2];
	ok &= dw[1*2 + 0] == x[3];
	ok &= dw[2*2 + 0] == x[0];
	ok &= dw[3*2 + 0] == x[1];
	// g_0'(x)   * [1, 2, 3, 4]  = 1 * x2 + 2 * x3 + 3 * x0 + 4 * x1
	// g_0^2 (x) * [1, 2, 3, 4]  = [3, 4, 1, 2]
	ok &= dw[0*2 + 1] == 3.;
	ok &= dw[1*2 + 1] == 4.;
	ok &= dw[2*2 + 1] == 1.;
	ok &= dw[3*2 + 1] == 2.;

	//----------------------------------------------------------------------
	// Test forward and reverse Jacobian sparsity pattern
	/*
	[ x0 , x1 ] * [ x2 , 7 ] = [ x0*x2 + x1*x3 , x0*7 + x1*8 ]
	[ 5  , 6 ]    [ x3 , 8 ]   [ 5*x2  + 6*x3  , 5*7 + 6*8 ]
	so the sparsity pattern should be
	s[0] = {0, 1, 2, 3}
	s[1] = {0, 1}
	s[2] = {2, 3}
	s[3] = {}
	*/
	CPPAD_TEST_VECTOR< std::set<size_t> > r( X.size() ), s(m);
	for(j = 0; j <  X.size() ; j++)
	{	assert( r[j].empty() );
		r[j].insert(j);
	}
	s = G.ForSparseJac( X.size() , r);
	for(j = 0; j <  X.size() ; j++)
	{	// s[0] = {0, 1, 2, 3}
		ok &= s[0].find(j) != s[0].end();
		// s[1] = {0, 1}
		if( j == 0 || j == 1 )
			ok &= s[1].find(j) != s[1].end();
		else	ok &= s[1].find(j) == s[1].end();
		// s[2] = {2, 3}
		if( j == 2 || j == 3 )
			ok &= s[2].find(j) != s[2].end();
		else	ok &= s[2].find(j) == s[2].end();
	}
	// s[3] == {}
	ok &= s[3].empty();
	
	//----------------------------------------------------------------------
	// Test reverse Jacobian sparsity pattern
	/*
	[ x0 , x1 ] * [ x2 , 7 ] = [ x0*x2 + x1*x3 , x0*7 + x1*8 ]
	[ 5  , 6 ]    [ x3 , 8 ]   [ 5*x2  + 6*x3  , 5*7 + 6*8 ]
	so the sparsity pattern should be
	r[0] = {0, 1, 2, 3}
	r[1] = {0, 1}
	r[2] = {2, 3}
	r[3] = {}
	*/
	for(i = 0; i <  m; i++)
	{	s[i].clear();
		s[i].insert(i);
	}
	r = G.RevSparseJac(m, s);
	for(j = 0; j <  X.size() ; j++)
	{	// r[0] = {0, 1, 2, 3}
		ok &= r[0].find(j) != r[0].end();
		// r[1] = {0, 1}
		if( j == 0 || j == 1 )
			ok &= r[1].find(j) != r[1].end();
		else	ok &= r[1].find(j) == r[1].end();
		// r[2] = {2, 3}
		if( j == 2 || j == 3 )
			ok &= r[2].find(j) != r[2].end();
		else	ok &= r[2].find(j) == r[2].end();
	}
	// r[3] == {}
	ok &= r[3].empty();

	//----------------------------------------------------------------------
	/* Test reverse Hessian sparsity pattern
	g_0^2 (x) = [ 0, 0, 1, 0 ] and for i > 0, g_i^2 = 0
	            [ 0, 0, 0, 1 ]
	            [ 1, 0, 0, 0 ]
	            [ 0, 1, 0, 0 ]
	so for the sparsity pattern for the first component of g is
	h[0] = {2}
	h[1] = {3}
	h[2] = {0}
	h[3] = {1}
	*/
	CPPAD_TEST_VECTOR< std::set<size_t> > h( X.size() ), t(1);
	t[0].clear();
	t[0].insert(0);
	h = G.RevSparseHes(X.size() , t);
	size_t check[] = {2, 3, 0, 1};
	for(j = 0; j <  X.size() ; j++)
	{	// h[j] = { check[j] }
		for(i = 0; i < n; i++) 
		{	if( i == check[j] )
				ok &= h[j].find(i) != h[j].end();
			else	ok &= h[j].find(i) == h[j].end();
		}
	}
	t[0].clear();
	for( j = 1; j < X.size(); j++)
			t[0].insert(j);
	h = G.RevSparseHes(X.size() , t);
	for(j = 0; j <  X.size() ; j++)
	{	// h[j] = { }
		for(i = 0; i < X.size(); i++) 
			ok &= h[j].find(i) == h[j].end();
	}

	// --------------------------------------------------------------------
	// Free temporary work space. (If there are future calls to 
	// mat_mul they would create new temporary work space.)
	CppAD::user_atomic<double>::clear();
	info_.resize(0);

	return ok;
}
// END PROGRAM
