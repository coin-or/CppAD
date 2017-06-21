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
	%A%, %b%, %H%, %g%, %epsilon%, %maxitr%, %xin%, %xout%, %yout%, %sout%
)%$$
see $cref/prototype/qp_interior/Prototype/$$

$head Problem$$
We are given
$latex A \in \B{R}^{m \times n}$$,
$latex b \in \B{R}^m$$,
$latex H \in \B{R}^{n \times n}$$,
$latex g \in \B{R}^n$$,
where $latex H$$ is positive semi-definite
and $latex H + A^T A$$ is positive definite.
This routine solves the problem
$latex \[
\begin{array}{rl}
\R{minimize} &
\frac{1}{2} x^\R{T} H x + g^\R{T} x \; \R{w.r.t} \; x \in \B{R}^n
\\
\R{subject \; to} &
A x + b \leq 0
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
	A_{i, j} = v[ i \times m + j ]
\] $$

$head A$$
This is the matrix $latex A$$ in the problem in row-major order.

$head b$$
This is the vector $latex b$$ in the problem.

$head H$$
This is the matrix $latex H$$ in the problem in row-major order.

$head g$$
This is the vector $latex g$$ in the problem.

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
$latex A x - b < 0$$  for $icode%x% = %xin%$$.

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
g + H x + y^\R{T} A             \\
A x + b + s                           \\
D(s) D(y) \R{1}_m - \mu 1_m
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
H       & A^T  & 0_{n,m} \\
A       & 0    & I_{m,m} \\
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
r_x (x, y, s) & = & g + H x + y^\R{T} A \\
r_y (x, y, s) & = & A x + b + s          \\
r_s (x, y, s) & = & D(s) D(y) \R{1}_m - \mu 1_m
\end{array}
\] $$
It follows that
$latex \[
\left( \begin{array}{ccc}
H       & A^T  & 0_{n,m} \\
A       & 0    & I_{m,m} \\
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
H        & A^T  & 0_{n,m} \\
A        & 0    & I_{m,m} \\
- D(y) A & D(s) & 0_{m,m}
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
H          & A^T     & 0_{n,m} \\
A          & 0       & I_{m,m} \\
- D(y/s) A & I_{m,m} & 0_{m,m}
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
Subtracting $latex A^T$$ times the third row from the first row we obtain:
$latex \[
\left( \begin{array}{ccc}
H + A^T D(y/s) A & 0_{n,m} & 0_{n,m} \\
A                & 0       & I_{m,m} \\
- D(y/s) A       & I_{m,m} & 0_{m,m}
\end{array} \right)
\left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
=
-
\left( \begin{array}{c}
	r_x (x, y, s)
		- A^T D(s)^{-1} \left[ r_s (x, y, s) - D(y) r_y(x, y, s) \right] \\
	r_y (x, y, s) \\
	D(s)^{-1} r_s (x, y, s) - D(y/s) r_y(x, y, s)
\end{array} \right)
\] $$

$head Solution$$
It follows that $latex H + A^T D(y/s) A$$ is invertible and
we can determine $latex \Delta x$$ by solving the equation
$latex \[
[ H + A^T D(y/s) A ] \Delta x
=
A^T D(s)^{-1} \left[ r_s (x, y, s) - D(y) r_y(x, y, s) \right] - r_x (x, y, s)
\] $$
Given $latex \Delta x$$ we have that
$latex \[
\Delta s = - r_y (x, y, s) - A \Delta x
\] $$
$latex \[
\Delta y =
D(s)^{-1}[ D(y) r_y(x, y, s) - r_s (x, y, s) + D(y) A \Delta x ]
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
	template <class Vector>
	Vector qp_interior_F_0(
		const Vector& A       ,
		const Vector& b       ,
		const Vector& H       ,
		const Vector& g       ,
		const Vector& x       ,
		const Vector& y       ,
		const Vector& s       )
	{	size_t n = g.size();
		size_t m = b.size();
		// compute r_x(x, y, s) = g + H x + y^T A
		Vector r_x(n);
		for(size_t j = 0; j < n; j++)
		{	r_x[j] = g[j];
			for(size_t i = 0; i < n; i++)
				r_x[j] += H[j * n + i] * x[i];
			for(size_t i = 0; i < m; i++)
				r_x[j] += y[i] * A[i * n + j];
		}
		// compute r_y(x, y, s) = A x + b + s
		Vector r_y(m);
		for(size_t i = 0; i < m; i++)
		{	r_y[i] = b[i] + s[i];
			for(size_t j = 0; j < n; j++)
				r_y[i] += A[i * n + j] * x[j];
		}
		// compute r_s(x, y, s) = D(s) * D(y) * 1_m - mu * 1_m
		Vector r_s(m);
		for(size_t i = 0; i < m; i++)
			r_s[i] = s[i] * y[i];
		//
		// combine into one vector
		Vector F_0(n + m + m);
		for(size_t j = 0; j < n; j++)
			F_0[j] = r_x[j];
		for(size_t i = 0; i < m; i++)
		{	F_0[n + i]     = r_y[i];
			F_0[n + m + i] = r_s[i];
		}
		return F_0;
	}
	template <class Vector>
	double qp_interior_norm_sq(const Vector& v)
	{	double norm_sq = 0.0;
		for(size_t j = 0; j < v.size(); j++)
			norm_sq += v[j] * v[j];
		return norm_sq;
	}
}
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN PROTOTYPE
template <class Vector>
bool qp_interior(
	const Vector& A       ,
	const Vector& b       ,
	const Vector& H       ,
	const Vector& g       ,
	double        epsilon ,
	size_t        maxitr  ,
	const Vector& xin     ,
	Vector&       xout    ,
	Vector&       yout    ,
	Vector&       sout    )
// END PROTOTYPE
{	size_t m = b.size();
	size_t n = g.size();
	CPPAD_ASSERT_KNOWN(
		A.size() == m * n,
		"qp_interior: size of A is not m * n"
	);
	CPPAD_ASSERT_KNOWN(
		H.size() == n * n,
		"qp_interior: size of H is not n * n"
	);
	//
	// compute the maximum absolute element of the problem vectors and matrices
	double max_element = 0.0;
	for(size_t i = 0; i < A.size(); i++)
		max_element = std::max(max_element , std::fabs(A[i]) );
	for(size_t i = 0; i < b.size(); i++)
		max_element = std::max(max_element , std::fabs(b[i]) );
	for(size_t i = 0; i < H.size(); i++)
		max_element = std::max(max_element , std::fabs(H[i]) );
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
	{	double sum = b[i];
		for(size_t j = 0; j < n; j++)
			sum += A[ i * n + j ] * xout[j];
		if( sum > 0.0 )
			return false;
		//
		sout[i] = std::sqrt(mu);
		yout[i] = std::sqrt(mu);
	}
	// ----------------------------------------------------------------------
	// initialie F_0(xout, yout, sout)
	Vector F_0 = qp_interior_F_0(A, b, H, g, xout, yout, sout);
	for(size_t itr = 0; itr <= maxitr; itr++)
	{	// compute F_mu(xout, yout, sout)
		Vector F_mu  = F_0;
		for(size_t i = 0; i < m; i++)
			F_mu[n + m + i] -= mu;
		//
		// check for convergence
		double F_norm_sq   = qp_interior_norm_sq( F_0 );
		if( F_norm_sq <= epsilon * epsilon )
			return true;
		if( itr == maxitr )
			return false;
		// -------------------------------------------------------------------
		// tmp1 = D(s)^{-1} * [ r_s - D(y) r_y ]
		Vector tmp1(m);
		for(size_t i = 0; i < m; i++)
		{	tmp1[i]  = F_mu[n + m + i];        // r_s
			tmp1[i] -= yout[i] * F_mu[n + i];  // r_s - D(y) * r_y
			tmp1[i] /= sout[i];                // D(s)^-1 [ r_s - D(y) * r_y ]
		}
		//
		// rhs_x = A^T * D(s)^{-1} * [ r_s - D(y) r_y ] - r_x
		Vector rhs_x(n);
		for(size_t j = 0; j < n; j++)
		{	rhs_x[j] = - F_mu[j]; // - r_x
			for(size_t i = 0; i < m; i++)
				rhs_x[j] += A[ i * m + j ] * tmp1[i];
		}
		//
		// Left_x = H + A^T * D(y / s) * A
		Vector Left_x = H;
		for(size_t i = 0; i < n; i++)
		{	for(size_t j = 0; j < n; j++)
			{	for(size_t k = 0; k < m; k++)
				{	double y_s = yout[k] / sout[k];
					Left_x[ i * n + j] += A[k * n + j] * y_s * A[k * n + i];
				}
			}
		}
		// delta_x
		Vector delta_x(n);
		double logdet;
		LuSolve(n, 1, Left_x, rhs_x, delta_x, logdet);
		//
		// delta_y
		Vector delta_y(m);
		for(size_t i = 0; i < m; i++)
		{	delta_y[i] = 0.0;
			for(size_t j = 0; j < n; j++)
				delta_y[i] += A[i * n + j] * delta_x[j]; // A * delta_x
			delta_y[i] += F_mu[n + i];      // r_y + A * delta_x
			delta_y[i] *= yout[i];          // D(y) * [r_y + A * delta_x]
			delta_y[i] -= F_mu[n + m + i];  // D(y) * [r_y + A * delta_x] - r_s
			delta_y[i] /= sout[i]; // D(s)^-1*( D(y)*[r_y + A*delta_x] - r_s )
		}
		// delta_s
		Vector delta_s(m);
		for(size_t i = 0; i < m; i++)
		{	// - r_y - A * delta_x
			delta_s[i] = - F_mu[n + i];
			for(size_t j = 0; j < n; j++)
				delta_s[i] -= A[i * n + j] * delta_x[j];
		}
		// delta_xys
		Vector delta_xys(n + m + m);
		for(size_t j = 0; j < n; j++)
			delta_xys[j] = delta_x[j];
		for(size_t i = 0; i < m; i++)
			delta_xys[n + i] = delta_y[i];
		for(size_t i = 0; i < m; i++)
			delta_xys[n + m + i] = delta_s[i];
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
			{	Vector F_mu_tmp = qp_interior_F_0(A, b, H, g, x, y, s);
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
		F_0 = qp_interior_F_0(A, b, H, g, xout, yout, sout);
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
