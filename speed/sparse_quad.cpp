/*
$head sparse_quad$$
$index sparse, quadratic$$
$index quadratic, sparse$$
$index speed, sparse quadratic$$
$index correct, sparse quadratic$$
If $italic option$$ is equal to $code sparse_quad$$,
the correctness and speed test for computing a sparse Hessian matrix are run.
Each package defines a version of this test with the following prototype:
$codep */
	extern bool compute_sparse_quad(
		size_t                     size       , 
		size_t                     repeat     ,
		size_t                      ell       ,
		CppAD::vector<size_t>      &i         ,
		CppAD::vector<size_t>      &j         , 
		CppAD::vector<double>      &hessian
	);
/* $$

$subhead f$$
The function $latex f : \R^n \rightarrow \R $$ is defined by
$latex \[
	f(x) = \sum_{k=1}^\ell x_{i[k]} x_{j[k]}
\] $$
Note that different vectors $latex i$$ and $latex j$$ 
correspond to different functions $latex f(x)$$.

$subhead size$$
The argument $italic size$$
is the dimension of the argument space for the function $latex f$$; 
i.e. $italic size$$ is equal to $latex n$$.

$subhead repeat$$
The argument $italic repeat$$ is the number of different matrices
that the Hessian is computed for.

$head ell$$
The argument $italic ell$$ has prototype
$syntax%
        size_t %ell%
%$$
and is the number of terms in the summation that defines $latex f(x)$$; i.e.,
$latex \ell$$.

$subhead i$$
The argument $italic i$$ is a vector of size $italic ell$$.
The input value of its elements does not matter.
On output, it has been set the the first index vector
for the last function $latex f(x)$$ (each element of the repeat
loop corresponds to a different choice of $latex i$$ and $latex j$$).
All the elements of $italic i$$ must be between zero and $syntax%%size%-1%$$.

$subhead j$$
The argument $italic j$$ is a vector of size $italic ell$$.
The input value of its elements does not matter.
On output, it has been set the the second index vector
for the last function $latex f(x)$$. 
All the elements of $italic j$$ must be between zero and $syntax%%size%-1%$$.

$subhead hessian$$
The argument $italic hessian$$ is a vector with 
$syntax%%n%*%n%$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the Hessian of the function $latex f(x)$$
corresponding to output values of $italic i$$ and $italic j$$.
To be more specific,
$latex k = 0 , \ldots , n-1$$,
$latex m = 0 , \ldots , n-1$$,
$latex \[
	\DD{f}{x[k]}{x[m]} = hessian [ k * n + m ]
\] $$
(If $italic package$$ is $code double$$, only the first element
of $italic hessian$$ is used and it is actually $latex f(x)$$  value instead
of $latex f^{(2)} (x)$$.)

*/

// --------------------------------------------------------------
	bool available_sparse_quad(void)
	{	size_t size   = 10;
		size_t repeat = 1;
		size_t ell    = 3 * n;
		CppAD::vector<size_t> i, j; 
		CppAD::vector<double> hessian(size * size);

		return compute_sparse_quad(
			size, repeat, ell, i, j, hessian
		);
	}
// --------------------------------------------------------------
	bool correct_sparse_quad(void)
	{	size_t size   = 10;
		size_t repeat = 1;
		size_t ell    = 3 * n;
		CppAD::vector<size_t> i, j; 
		CppAD::vector<double> x(size); // values do not matter
		CppAD::vector<double> hessian(size * size);

		compute_sparse_quad(size, repeat, ell, i, j, hessian);
		size_t m = 2; // order of derivative
		bool ok = CppAD::sparse_quad(size, i, j, x, m, hessian);
		return ok;
	}
// --------------------------------------------------------------
	void speed_sparse_quad(size_t size, size_t repeat)
	{	CppAD::vector<size_t> i, j; 
		CppAD::vector<double> hessian(size * size);

		size_t ell = 3 * n;
		compute_sparse_quad(size, repeat, ell, i, j, hessian);
		return;
	}
// --------------------------------------------------------------
	char *option[]= {
		"correct",
		"speed",
		"det_lu",
		"det_minor",
		"poly",
		"sparse_quad"
	};
// --------------------------------------------------------------
        const size_t option_sparse_quad  = 5;
// --------------------------------------------------------------
		if( available_sparse_quad() ) 
		ok &= Run_correct(correct_sparse_quad,    "sparse_quad" );
// --------------------------------------------------------------
		if( available_sparse_quad() ) Run_speed(
		speed_sparse_quad,   size_speed_sparse_quad,  "sparse_quad" );

// --------------------------------------------------------------
		case sparse_quad:
		if( ! available_sparse_quad() )
		{	cout << AD_PACKAGE << ": option " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= Run_correct(correct_sparse_quad,            "poly");
		Run_speed(speed_sparse_quad, size_sparse_quad,    "poly");
		break;

