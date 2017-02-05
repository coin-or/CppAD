# ifndef CPPAD_CORE_SPARSE_JAC_WORK_HPP
# define CPPAD_CORE_SPARSE_JAC_WORK_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file sparse_jac_work.hpp
Sparse Jacobian work class
*/

/*!
class used to hold information used by Sparse Jacobian routines,
so it does not need to be recomputed every time.
*/
namespace CppAD {
	class sparse_jacobian_work {
		public:
			/// Coloring method: "cppad", or "colpack"
			/// (this field is set by user)
			std::string color_method;
			/// indices that sort the user row and col arrays by color
			CppAD::vector<size_t> order;
			/// results of the coloring algorithm
			CppAD::vector<size_t> color;
			//
			/// constructor
			sparse_jacobian_work(void) : color_method("cppad")
			{ }
			/// reset work to empty.
			/// This informs CppAD that color and order need to be recomputed
			void clear(void)
			{	color_method = "cppad";
				order.clear();
				color.clear();
			}
	};

}

# endif
