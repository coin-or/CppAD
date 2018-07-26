# ifndef CPPAD_LOCAL_OPTIMIZE_PAR_PLAY2REC_HPP
# define CPPAD_LOCAL_OPTIMIZE_PAR_PLAY2REC_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/recorder.hpp>
# include <cppad/local/player.hpp>

/*!
\file par_play2rec.hpp
Convert from index in player to index in optimized recorder.
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {

/*!
Convert from index in player to index in optimized recorder.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type
Base.

\param rec
This is the recording where we are placing the parameter.

\param play
This is the operation sequence that we are optimizing.

\param index
This is the index of the parameter in play.

\return
is the index of the paramerer in rec.
*/
template <class Base>
addr_t par_play2rec(
	recorder<Base>*       rec        ,
	const player<Base>*   play       ,
	addr_t                index      )
{	// dynamic parameters have same index in play and rec
	if( size_t( index ) < play->num_ind_dynamic() )
	{	CPPAD_ASSERT_UNKNOWN( rec->get_num_ind_dynamic() == play->num_ind_dynamic() );
		return addr_t( index );
	}
	//
	// put this paramerer in the recording and return its rec index
	return rec->put_con_par( play->GetPar(index) );
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
