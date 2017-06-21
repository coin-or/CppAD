# ifndef CPPAD_EXAMPLE_ABS_NORMAL_QP_INTERIOR_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_QP_INTERIOR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin qp_interior$$
$spell
	const
	col
	xout
	yout
	sout
	cols
	prog
	maxitr
	xin
	qp
$$

$section Solve a Quadratic Program Using Interior Point Method$$

$head Under Construction$$

$head Syntax$$
$icode%ok% = qp_interior(
	%c%, %C%, %g%, %G%, %epsilon%, %maxitr%, %xin%, %xout%, %yout%, %sout%
)%$$
see $cref/prototype/qp_interior/Prototype/$$

$head Problem$$
We are given
$latex C \in \B{R}^{m \times n}$$,
$latex c \in \B{R}^m$$,
$latex G \in \B{R}^{n \times n}$$,
$latex g \in \B{R}^n$$,
where $latex G$$ is positive semi-definite
and $latex G + C^T C$$ is positive definite.
This routine solves the problem
$latex \[
\begin{array}{rl}
\R{minimize} &
\frac{1}{2} x^T G x + g^T x \; \R{w.r.t} \; x \in \B{R}^n
\\
\R{subject \; to} &
C x + c \leq 0
\end{array}
\] $$

$head Vector$$
The type $icode Vector$$ is a
simple vector with elements of type $code double$$.

$head Row-major$$
A vector $icode v$$ is a matrix $latex M \in \B{R}^{m \times n}$$
in row major order, if the size of $icode v$$ is $latex m  \times n$$ and for
$latex i = 0 , \ldots , m-1$$,
$latex j = 0 , \ldots , n-1$$,
$latex \[
	M_{i, j} = v[ i \times m + j ]
\] $$

$head c$$
This is the vector $latex c$$ in the problem.

$head C$$
This is the matrix $latex C$$ in the problem in row-major order.

$head g$$
This is the vector $latex g$$ in the problem.

$head G$$
This is the matrix $latex G$$ in the problem in row-major order.

$head epsilon$$
This argument is the convergence criteria;
see $cref/KKT conditions/qp_interior/KKT Conditions/$$ below.
It must be greater than zero.

$head maxitr$$
This is the maximum number of newton iterations to try before giving up
on convergence.

$head xin$$
This argument has size $icode n$$ and is the initial point for the algorithm.
It must strictly satisfy the constraints; i.e.,
$latex C x - c < 0$$  for $icode%x% = %xin%$$.

$head xout$$
This argument has size is $icode n$$ and
the input value of its elements does no matter.
Upon return it is the primal variables corresponding to the problem solution.

$head yout$$
This argument has size is $icode m$$ and
the input value of its elements does no matter.
Upon return it the components of $icode yout$$ are all positive
and it is the dual variables corresponding to the program solution.

$head sout$$
This argument has size is $icode m$$ and
the input value of its elements does no matter.
Upon return it the components of $icode sout$$ are all positive
and it is the slack variables corresponding to the program solution.

$head ok$$
If the return value $icode ok$$ is true, convergence is obtained; i.e.,
$latex \[
	| F_0 (xout , yout, sout) | \leq epsilon
\] $$
where $latex | v |$$ is the Euclidean norm of the vector $latex v$$
and $latex F_\mu (x, y, s)$$ is defined below.


$head KKT Conditions$$
Give a vector $latex v \in \B{R}^m$$ we define
$latex D(v) \in \B{R}^{m \times m}$$ as the corresponding diagonal matrix.
We also define $latex 1_m \in \B{R}^m$$ as the vector of ones.
We define
$latex F_\mu : \B{R}^{n + m + m } \rightarrow \B{R}^{n + m + m}$$
by
$latex \[
F_\mu ( x , y , s )
=
\left(
\begin{array}{c}
g + G x + y^T C             \\
C x + c + s                           \\
D(s) D(y) 1_m - \mu 1_m
\end{array}
\right)
\] $$
The KKT conditions for a solution of this problem is
$latex 0 \leq y$$,
$latex 0 \leq s$$, and
$latex F_0 (x , y, s) = 0$$.

$head Newton Step$$
The derivative of $latex F_\mu$$ is given by
$latex \[
F_\mu^{(1)} (x, y, s)  =
\left( \begin{array}{ccc}
G       & C^T  & 0_{n,m} \\
C       & 0    & I_{m,m} \\
0_{m,m} & D(s) & D(y)
\end{array} \right)
\] $$
The Newton step solves the following equation for
$latex \Delta x$$, $latex \Delta y$$, and $latex \Delta z$$
$latex \[
F_\mu^{(1)} (x, y, s)
\left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
=
- F_\mu (x, y, s)
\] $$
To simplify notation, we define
$latex \[
\begin{array}{rcl}
r_x (x, y, s) & = & g + G x + y^T C \\
r_y (x, y, s) & = & C x + c + s          \\
r_s (x, y, s) & = & D(s) D(y) 1_m - \mu 1_m
\end{array}
\] $$
It follows that
$latex \[
\left( \begin{array}{ccc}
G       & C^T  & 0_{n,m} \\
C       & 0    & I_{m,m} \\
0_{m,m} & D(s) & D(y)
\end{array} \right)
\left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
=
-
\left( \begin{array}{c}
	r_x (x, y, s) \\
	r_y (x, y, s) \\
	r_s (x, y, s)
\end{array} \right)
\] $$

$subhead Elementary Row Reduction$$
Subtracting $latex D(y)$$ times the second row from the third row
we obtain:
$latex \[
\left( \begin{array}{ccc}
G        & C^T  & 0_{n,m} \\
C        & 0    & I_{m,m} \\
- D(y) C & D(s) & 0_{m,m}
\end{array} \right)
\left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
=
-
\left( \begin{array}{c}
	r_x (x, y, s) \\
	r_y (x, y, s) \\
	r_s (x, y, s) - D(y) r_y(x, y, s)
\end{array} \right)
\] $$
Multiplying the third row by $latex D(s)^{-1}$$ we obtain:
$latex \[
\left( \begin{array}{ccc}
G          & C^T     & 0_{n,m} \\
C          & 0       & I_{m,m} \\
- D(y/s) C & I_{m,m} & 0_{m,m}
\end{array} \right)
\left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
=
-
\left( \begin{array}{c}
	r_x (x, y, s) \\
	r_y (x, y, s) \\
	D(s)^{-1} r_s (x, y, s) - D(y/s) r_y(x, y, s)
\end{array} \right)
\] $$
where $latex y/s$$ is the vector in $latex \B{R}^m$$ defined by
$latex (y/s)_i = y_i / s_i$$.
Subtracting $latex C^T$$ times the third row from the first row we obtain:
$latex \[
\left( \begin{array}{ccc}
G + C^T D(y/s) C & 0_{n,m} & 0_{n,m} \\
C                & 0       & I_{m,m} \\
- D(y/s) C       & I_{m,m} & 0_{m,m}
\end{array} \right)
\left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
=
-
\left( \begin{array}{c}
	r_x (x, y, s)
		- C^T D(s)^{-1} \left[ r_s (x, y, s) - D(y) r_y(x, y, s) \right] \\
	r_y (x, y, s) \\
	D(s)^{-1} r_s (x, y, s) - D(y/s) r_y(x, y, s)
\end{array} \right)
\] $$

$head Solution$$
It follows that $latex G + C^T D(y/s) C$$ is invertible and
we can determine $latex \Delta x$$ by solving the equation
$latex \[
[ G + C^T D(y/s) C ] \Delta x
=
C^T D(s)^{-1} \left[ r_s (x, y, s) - D(y) r_y(x, y, s) \right] - r_x (x, y, s)
\] $$
Given $latex \Delta x$$ we have that
$latex \[
\Delta s = - r_y (x, y, s) - C \Delta x
\] $$
$latex \[
\Delta y =
D(s)^{-1}[ D(y) r_y(x, y, s) - r_s (x, y, s) + D(y) C \Delta x ]
\] $$

$children%example/abs_normal/qp_interior.cpp
%$$
$head Example$$
The file $cref qp_interior.cpp$$ contains an example and test of
$code qp_interior$$.
It returns true if the test passes and false otherwise.

$head Prototype$$
$srcfile%example/abs_normal/qp_interior.hpp%
	0%// BEGIN PROTOTYPE%// END PROTOTYPE%
1%$$

$end
-----------------------------------------------------------------------------
*/
# include <cmath>
# include <cppad/utility/lu_solve.hpp>

namespace {
	// ------------------------------------------------------------------------
	template <class Vector>
	double qp_interior_norm_sq(const Vector& v)
	{	double norm_sq = 0.0;
		for(size_t j = 0; j < v.size(); j++)
			norm_sq += v[j] * v[j];
		return norm_sq;
	}
	// ------------------------------------------------------------------------
	template <class Vector>
	void qp_interior_split(
		const Vector& v, Vector& v_x, Vector& v_y, Vector& v_s
	)
	{	size_t n = v_x.size();
		size_t m = v_y.size();
		CPPAD_ASSERT_UNKNOWN( v_s.size() == m );
		CPPAD_ASSERT_UNKNOWN( v.size() == n + m + m );
		for(size_t i = 0; i < n; i++)
			v_x[i] = v[i];
		for(size_t i = 0; i < m; i++)
		{	v_y[i] = v[n + i];
			v_s[i] = v[n + m + i];
		}
		return;
	}
	// ------------------------------------------------------------------------
	template <class Vector>
	void qp_interior_join(
		Vector& v, const Vector& v_x, const Vector& v_y, const Vector& v_s
	)
	{	size_t n = v_x.size();
		size_t m = v_y.size();
		CPPAD_ASSERT_UNKNOWN( v_s.size() == m );
		CPPAD_ASSERT_UNKNOWN( v.size() == n + m + m );
		for(size_t i = 0; i < n; i++)
			v[i] = v_x[i];
		for(size_t i = 0; i < m; i++)
			v[n + i] = v_y[i];
		for(size_t i = 0; i < m; i++)
			v[n + m + i] = v_s[i];
		return;
	}
	// ------------------------------------------------------------------------
	template <class Vector>
	Vector qp_interior_F_0(
		const Vector& c       ,
		const Vector& C       ,
		const Vector& g       ,
		const Vector& G       ,
		const Vector& x       ,
		const Vector& y       ,
		const Vector& s       )
	{	size_t n = g.size();
		size_t m = c.size();
		// compute r_x(x, y, s) = g + G x + y^T C
		Vector r_x(n);
		for(size_t j = 0; j < n; j++)
		{	r_x[j] = g[j];
			for(size_t i = 0; i < n; i++)
				r_x[j] += G[j * n + i] * x[i];
			for(size_t i = 0; i < m; i++)
				r_x[j] += y[i] * C[i * n + j];
		}
		// compute r_y(x, y, s) = C x + c + s
		Vector r_y(m);
		for(size_t i = 0; i < m; i++)
		{	r_y[i] = c[i] + s[i];
			for(size_t j = 0; j < n; j++)
				r_y[i] += C[i * n + j] * x[j];
		}
		// compute r_s(x, y, s) = D(s) * D(y) * 1_m - mu * 1_m
		// where mu = 0
		Vector r_s(m);
		for(size_t i = 0; i < m; i++)
			r_s[i] = s[i] * y[i];
		//
		// combine into one vector
		Vector F_0(n + m + m);
		qp_interior_join(F_0, r_x, r_y, r_s);
		//
		return F_0;
	}
}
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN PROTOTYPE
template <class Vector>
bool qp_interior(
	const Vector& c       ,
	const Vector& C       ,
	const Vector& g       ,
	const Vector& G       ,
	double        epsilon ,
	size_t        maxitr  ,
	const Vector& xin     ,
	Vector&       xout    ,
	Vector&       yout    ,
	Vector&       sout    )
// END PROTOTYPE
{	size_t m = c.size();
	size_t n = g.size();
	CPPAD_ASSERT_KNOWN(
		C.size() == m * n,
		"qp_interior: size of C is not m * n"
	);
	CPPAD_ASSERT_KNOWN(
		G.size() == n * n,
		"qp_interior: size of G is not n * n"
	);
	//
	// compute the maximum absolute element of the problem vectors and matrices
	double max_element = 0.0;
	for(size_t i = 0; i < C.size(); i++)
		max_element = std::max(max_element , std::fabs(C[i]) );
	for(size_t i = 0; i < c.size(); i++)
		max_element = std::max(max_element , std::fabs(c[i]) );
	for(size_t i = 0; i < G.size(); i++)
		max_element = std::max(max_element , std::fabs(G[i]) );
	for(size_t i = 0; i < g.size(); i++)
		max_element = std::max(max_element , std::fabs(g[i]) );
	//
	double mu = 1e-3 * max_element;
	//
	if( max_element == 0.0 )
		return false;
	//
	// initialize x, y, s
	xout = xin;
	for(size_t i = 0; i < m; i++)
	{	double sum = c[i];
		for(size_t j = 0; j < n; j++)
			sum += C[ i * n + j ] * xout[j];
		if( sum > 0.0 )
			return false;
		//
		sout[i] = std::sqrt(mu);
		yout[i] = std::sqrt(mu);
	}
	// ----------------------------------------------------------------------
	// initialie F_0(xout, yout, sout)
	Vector F_0 = qp_interior_F_0(c, C, g, G, xout, yout, sout);
	for(size_t itr = 0; itr <= maxitr; itr++)
	{
		//
		// check for convergence
		double F_norm_sq   = qp_interior_norm_sq( F_0 );
		if( F_norm_sq <= epsilon * epsilon )
			return true;
		if( itr == maxitr )
			return false;
		//
		// compute F_mu(xout, yout, sout)
		Vector F_mu  = F_0;
		for(size_t i = 0; i < m; i++)
			F_mu[n + m + i] -= mu;
		//
		// r_x, r_y, r_s (xout, yout, sout)
		Vector r_x(n), r_y(m), r_s(m);
		qp_interior_split(F_mu, r_x, r_y, r_s);
		//
		// tmp_m = D(s)^{-1} * [ r_s - D(y) r_y ]
		Vector tmp_m(m);
		for(size_t i = 0; i < m; i++)
			tmp_m[i]  = ( r_s[i] - yout[i] * r_y[i] ) / sout[i];
		//
		// right_x = C^T * D(s)^{-1} * [ r_s - D(y) r_y ] - r_x
		Vector right_x(n);
		for(size_t j = 0; j < n; j++)
		{	right_x[j] = 0.0;
			for(size_t i = 0; i < m; i++)
				right_x[j] += C[ i * m + j ] * tmp_m[i];
			right_x[j] -= r_x[j];
		}
		//
		// Left_x = G + C^T * D(y / s) * C
		Vector Left_x = G;
		for(size_t i = 0; i < n; i++)
		{	for(size_t j = 0; j < n; j++)
			{	for(size_t k = 0; k < m; k++)
				{	double y_s = yout[k] / sout[k];
					Left_x[ i * n + j] += C[k * n + j] * y_s * C[k * n + i];
				}
			}
		}
		// delta_x
		Vector delta_x(n);
		double logdet;
		LuSolve(n, 1, Left_x, right_x, delta_x, logdet);
		//
		// C_delta_x = C * delta_x
		Vector C_delta_x(m);
		for(size_t i = 0; i < m; i++)
		{	C_delta_x[i] = 0.0;
			for(size_t j = 0; j < n; j++)
				C_delta_x[i] += C[ i * n + j ] * delta_x[j];
		}
		//
		// delta_y = D(s)^-1 * [D(y) * r_y - r_s + D(y) * C * delta_x]
		Vector delta_y(m);
		for(size_t i = 0; i < m; i++)
		{	delta_y[i] = yout[i] * r_y[i] - r_s[i] + yout[i] * C_delta_x[i];
			delta_y[i] /= sout[i];
		}
		// delta_s = - r_y - C * delta_x
		Vector delta_s(m);
		for(size_t i = 0; i < m; i++)
			delta_s[i] = - r_y[i] - C_delta_x[i];
		//
		// delta_xys
		Vector delta_xys(n + m + m);
		qp_interior_join(delta_xys, delta_x, delta_y, delta_s);
		// -------------------------------------------------------------------
		//
		// The initial derivative in direction  Delta_xys is equal to
		// the negative of the norm square of F_mu
		F_norm_sq = qp_interior_norm_sq( F_mu );
		//
		// line search parameter lam
		Vector x(n), y(m), s(m);
		double  lam = 2.0;
		bool lam_ok = false;
		while( ! lam_ok && lam > 1e-5 )
		{	lam = lam / 2.0;
			for(size_t j = 0; j < n; j++)
				x[j] = xout[j] + lam * delta_xys[j];
			lam_ok = true;
			for(size_t i = 0; i < m; i++)
			{	y[i] = yout[i] + lam * delta_xys[n + i];
				s[i] = sout[i] + lam * delta_xys[n + m + i];
				lam_ok &= s[i] > 0.0 && y[i] > 0.0;
			}
			if( lam_ok )
			{	Vector F_mu_tmp = qp_interior_F_0(c, C, g, G, x, y, s);
				for(size_t i = 0; i < m; i++)
					F_mu_tmp[n + m + i] -= mu;
				double F_norm_sq_tmp = qp_interior_norm_sq( F_mu_tmp );
				lam_ok &= F_norm_sq_tmp - F_norm_sq < - lam * F_norm_sq / 4.0;
			}
		}
		if( ! lam_ok )
			return false;
		//
		// update current solution
		xout = x;
		yout = y;
		sout = s;
		//
		// updage F_0
		F_0 = qp_interior_F_0(c, C, g, G, xout, yout, sout);
		//
		// update mu
		F_norm_sq = qp_interior_norm_sq( F_0 );
		if( F_norm_sq <= 1e1 * double(n + m + m) * mu * mu )
			mu = mu / 1e3;
	}
	return false;
}
} // END_CPPAD_NAMESPACE

# endif
