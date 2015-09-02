#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
cat << EOF
Bug in dependency calcualtion when using VecAD objects.
EOF
cat << EOF > bug.$$
# include <cppad/cppad.hpp>
int main(void)
{	bool ok = true;
	using CppAD::AD;

	CppAD::VecAD<double> avec(2);
	avec[0] = 3.0;
	avec[1] = 4.0;
	CppAD::vector< AD<double> > ax(2), ay(2);
	ax[0] = 0.0;
	ax[1] = 1.0;
	CppAD::Independent(ax);
	avec[ ax[0] ] = ax[1];
	avec[ ax[1] ] = 5.0;
	ay[0]         = avec[ AD<double>(0) ];
	ay[1]         = avec[ AD<double>(1) ];
	CppAD::ADFun<double> f(ax, ay);
	//
	// Compute dependency matrix for f
	CppAD::vector<bool> depend(2 * 2), eye(2 * 2);
	for(size_t i = 0; i < 2; i++)
	{	for(size_t j = 0; j < 2; j++)
			eye[i * 2 + j] = (i == j );
	}
	bool transpose  = false;
	bool dependency = true;
	depend = f.ForSparseJac(2, eye, transpose, dependency);
	//
	CppAD::vector<double> x(2), y(2);
	x[0] = 1.0;
	x[1] = 0.0;
	y = f.Forward(0, x);
	//
	if( y[0] == Value(ay[0]) )
		ok &= ! depend[0 * 2 + 0];
	else
		ok &= depend[0 * 2 + 0];
	//
	if( ok )
		return 0;
	return 1;
}
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
echo "g++ -I../.. --std=c++11 -g $name.cpp -o $name"
g++ -I../.. --std=c++11 -g $name.cpp -o $name
#
echo "./$name"
if ! ./$name
then
	echo
	echo "$name.sh: Error"
	exit 1
fi
echo
echo "$name.sh: OK"
exit 0
