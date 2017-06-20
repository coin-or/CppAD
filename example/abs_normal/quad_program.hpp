# ifndef CPPAD_EXAMPLE_ABS_NORMAL_QUAD_PROGRAM_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_QUAD_PROGRAM_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin quad_program$$
$spell
	const
	col
	xout
	yout
	sout
	cols
	prog
	maxitr
$$
$latex
	\newcommand{\B}[1]{{\bf #1}}
	\newcommand{\R}[1]{{\rm #1}}
$$

$section Solve a Quadratic Program Using Interior Point Method$$

$head Under Construction$$

$head Syntax$$
$icode%ok% = quad_program(
	%A%, %b%, %H%, %g%, %epsilon%, %maxitr%, %xout%, %yout%, %sout%
)%$$
see $cref/prototype/quad_program/Prototype/$$

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
\R{minimize}      & \frac{1}{2} x^\R{T} H x + g^\R{T} x \\
\R{subject \; to} & A x + b \leq 0
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
see $cref/KKT conditions/quad_program/KKT Conditions/$$ below.
It must be greater than zero.

$head maxitr$$
This is the maximum number of newton iterations to try before giving up
on convergence.

$head xout$$
This argument has prototype
$codei%
	vector& %xout%
%$$
and its size is $icode n$$.
The input value of its elements does no matter.
Upon return it is the primal variables corresponding to the problem solution.

$head yout$$
This argument has prototype
$codei%
	vector& %yout%
%$$
and its size is $icode m$$.
The input value of its elements does no matter.
Upon return it the components of $icode yout$$ are all positive
and it is the dual variables corresponding to the program solution.

$head sout$$
This argument has prototype
$codei%
	vector& %sout%
%$$
and its size is $icode m$$.
The input value of its elements does no matter.
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
Subtracting $latex A^T$$ times the third row from the second row we obtain:
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
r_x (x, y, s) - A^T D(s)^{-1} \left[ r_s (x, y, s) - D(y) r_y(x, y, s) \right]
\] $$
Given $latex \Delta x$$ we have that
$latex \[
\Delta s = r_y (x, y, s) - A \Delta x
\] $$
$latex \[
\Delta y =  D(s)^{-1} r_s (x, y, s) - D(y/s) r_y(x, y, s) + D(y/s) A \Delta x
\] $$

$head Prototype$$
$srcfile%example/abs_normal/quad_program.hpp%
	0%// BEGIN PROTOTYPE%// END PROTOTYPE%
1%$$

$end
-----------------------------------------------------------------------------
*/
namespace {
	template <class Vector>
	Vector quad_program_F(
		const Vector& A       ,
		const Vector& b       ,
		const Vector& H       ,
		const Vector& g       ,
		const Vector& x       ,
		const Vector& y       ,
		const Vector& s       )
	{	// compute r_x(x, y, s) = g + H x + y^T A
		Vector r_x(n);
		for(size_t j = 0; j < n; i++)
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
			r_s[i] = s[i] * y[i] - mu;
		//
		// combine into one vector
		Vector F_mu(n + m + m);
		for(size_t j = 0; j < n; j++)
			F_mu[j] = r_x[j];
		for(size_t i = 0; i < m; i++)
		{	F_mu[n + i]     = r_y[i];
			F_mu[n + m + i] = r_s[i];
		}
		return F_mu;
	}
	template <class Vector>
	double quad_program_norm_sq(const Vector& v)
	{	double norm_sq = 0.0;
		for(size_t j = 0; j < v.size(); j++)
			norm_sq += v[j] * v[j];
		return norm_sq;
	}
}
// BEGIN PROTOTYPE
template <class Vector>
bool quad_program(
	const Vector& A       ,
	const Vector& b       ,
	const Vector& H       ,
	const Vector& g       ,
	double        epsilon ,
	size_t        maxitr  ,
	Vector&       xout    ,
	Vector&       yout    ,
	Vector&       sout    )
// END PROTOTYPE
{	bool ok = false;
	size_t m = b.size();
	size_t n = g.size();
	CPPAD_ASSERT_KNOWN(
		A.size() == m * n,
		"quad_program: size of A is not m * n"
	);
	CPPAD_ASSERT_KNOWN(
		H.size() == n * n,
		"quad_program: size of H is not n * n"
	);
	// initialze mu
	mu = 1e6 * epsilon;
	//
	// initialize x, y, s
	for(size_t j = 0; j < n; j++)
		xout[j] = 0.0;
	for(size_t j = 0; j < m; j++)
	{	yout[j] = std::sqrt(mu);
		sout[j] = std::sqrt(mu);
	}
	// ----------------------------------------------------------------------
	//          [ H  A^T   0    ]
	// dF_mu =  [ A  0     I    ]
	//          [ 0  D(s)  D(y) ]
	// fill in parts of dF_mu that do not change
	size_t n_var = n + m + m;
	Vector dF_mu(n_var * n_var);
	for(size_t i = 0; i < n_var; i++)
	{	for(size_t j = 0; j < n_var; j++)
			dF_mu[ i * n_var + j] = 0.0;          // initialize as 0
	}
	for(size_t i = 0; i < n; i++)
	{	for(size_t j = 0; j < n; j++)
			dF_mu[ i * n_var + j] = H[i * n + j]; // fill in H
	}
	for(size_t i = 0; i < m; i++)
	{	for(size_t j = 0; j < n; j++)
		{	dF_mu[ (i + n) * n_var + j ] = A[ i * n + j]; //fill in  A
			dF_mu[ j * n_var + (n + i) ] = A[ i * n + j]; //fill in  A^T
		}
	}
	for(size_t i = 0; i < m; i++)
		dF_mu[ (i + n) * n_var + (n + m + i) ] = 1.0;     // fill in I
	// ----------------------------------------------------------------------
	for(itr = 0; itr <= maxitr; itr++)
	{	// compute F_mu(x, y, s)
		Vector F_mu = quad_program_F(A, b, H, g, xout, yout, sout);
		//
		// compute F_0(x, y, s)
		Vector F_0  = F_mu;
		for(size_t k = 0; k < m; i++)
			F_0[n + m + i] += mu;
		//
		// check for convergence
		double F_norm_sq   = quad_program_norm_sq( F_0 );
		if( F_norm_sq <= epsilon * epsilon )
			return true;
		if( itr == maxitr )
			return false;
		//
		// fill in parts of dF_mu that change
		for(size_t i = 0; i < m; i++)
		{	dF_mu[ (n + m + i) * n_var + (n + i) ]     = sout[i]; // D(s)
			dF_mu[ (n + m + i) * n_var + (n + m + i) ] = yout[i]; // D(y)
		}
		// right hand side in linear equation
		Vector rhs_xys(n_var), delta_xys(n_var);
		for(size_t i = 0; i < n_var; i++)
			rhs_xys[i] = - F_mu[i];
		// solve for Newton Step
		double logdet;
		LuSolve(n_var, 1, dF_mu, rhs_xys, Delta_xys, logdet)
		//
		// The initial derivative in direction  Delta_xys is equal to
		// the negative of the norm square of F_mu
		F_norm_sq = quad_program_norm_sq( F_mu );
		//
		// line search parameter lambda
		Vector x(n), y(m), s(m);
		double  lambda = 2.0;
		bool lambda_ok = false;
		while( ! lambda_ok && lambda > 1e-3 )
		{	lambda = lambda / 2.0;
			for(size_t j = 0; j < n; j++)
				x[j] = xout[j] + lambda * delta_xyz[j];
			lambda_ok = true;
			for(size_t i = 0; i < m; i++)
			{	y[i] = yout[i] + lambda * delta_xys[n + i];
				s[i] = sout[i] + lambda * delta_xys[n + m + i];
				lambda_ok &= s[i] > 0.0 && y[i] > 0.0;
			}
			Vector F_mu_tmp = quad_program_F(A, b, H, g, x, y, s);
			double F_norm_sq_tmp = quad_program_norm_sq( F_mu_tmp );
			lambda_ok  &= (F_mu - F_mu_tmp) / lambda <= 0.5;
		}
		if( ! lambda_ok )
			return false;
		// update current solution
		xout = x;
		yout = y;
		sout = s;
		// update mu
		if( F_norm_sq <= double( n_var ) * mu * mu )
			mu = mu / 10.0;
	}
	return false;
}

# endif
