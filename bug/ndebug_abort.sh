#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
cat << EOF
A bug that only shows up when NDEBUG is defined and a tape is aborted.
EOF
cat << EOF > bug.$$
# include <cppad/cppad.hpp>

namespace { // put this function in the empty namespace
	template <typename Type>
	Type Minimum(const Type &x, const Type &y)
	{	// Use a comparision to compute the min(x, y)
		// (note that CondExp would never require retaping). 
		if( x < y )  
			return x;
		return y;
	}
	struct error_info {
		bool known;
		int  line;
		std::string file;
		std::string exp;
		std::string msg;
	};
	void error_handler(
		bool        known       ,
		int         line        ,
		const char *file        ,
		const char *exp         ,
		const char *msg         )
	{	// error handler must not return, so throw an exception
		error_info info;
		info.known = known;
		info.line  = line;
		info.file  = file;
		info.exp   = exp;
		info.msg   = msg;
		throw info;
	}

}

int main(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::vector;

	// domain space vector
	size_t n = 2;
	vector< AD<double> > ax(n);
	ax[0] = 3.;
	ax[1] = 4.;

	// declare independent variables and start tape recording
	CppAD::Independent(ax);

	// range space vector
	size_t m = 1;
	vector< AD<double> > ay(m);
	ay[0] = Minimum(ax[0], ax[1]);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f;
	f.Dependent(ax, ay);


	// Local block during which default CppAD error handler is replaced.
	size_t op_index = 3;
	{	CppAD::ErrorHandler local_error_handler(error_handler);

		try {
			// determine the operation index where the change occurred
			CppAD::Independent(ax, op_index);
		}
		catch( error_info info )
		{	// Must abort the recording so we can start a new one
			// (and to avoid a memory leak).
			AD<double>::abort_recording();
		}
	}

	// now retape where compairson has a different result
	vector<double> x(n), y(m);
	ax[0] = x[0] = 3.;
	ax[1] = x[1] = 2.;
	Independent(ax);
	ay[0] = Minimum(ax[0], ax[1]);
	f.Dependent(ax, ay);
	y    = f.Forward(0, x);
	ok  &= (y[0] == x[1]);

	if( ! ok )
		return 1;
	return 0;
}
// END C++
EOF
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
mv ../bug.$$ $name.cpp
echo "g++ -I../.. --std=c++11 -DNDEBUG $name.cpp -o $name"
g++ -I../.. --std=c++11 -DNDEBUG $name.cpp -o $name
#
echo "./$name"
if ./$name
then
	echo "OK"
else
	echo "Error"
fi
