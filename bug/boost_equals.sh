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
This is a simpler case that has the same error message as ./boost_lu.sh.
The gist of the message:
	/usr/include/boost/numeric/ublas/detail/matrix_assign.hpp:33:35:
	error: no match for ‘operator<’
	... snip ...
		return norm_inf (e1 - e2) < epsilon *
EOF
cat << EOF > bug.$$

# include <cppad/cppad.hpp>
# include <boost/numeric/ublas/lu.hpp>
int main() {
    typedef CppAD::AD<double> T;
	using boost::numeric::ublas::norm_inf;

    boost::numeric::ublas::matrix<T> a(5,5);
	T epsilon  = static_cast<T>( std::numeric_limits<double>::epsilon() );
	T min_norm = static_cast<T>( std::numeric_limits<double>::min() );
	T norm     = norm_inf(a);
	bool equals = norm < epsilon;
	std::cout << "equals = " << equals << std::endl;
	//
	// using boost::numeric::ublas::detail::equals;
	// std::cout << "equals = " << equals(a, a, epsilon, min_norm) << std::endl;
    return 0;
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
echo "g++ -I../.. --std=c++11 -g $name.cpp -o $name >& $name.log"
if ! g++ -I../.. --std=c++11 -g $name.cpp -o $name >& $name.log
then
	cat << EOF > $name.sed
s|\\[|&\\n|g
s|\\]|&\\n|g
s|[‘{}]|&\\n|g
s|[a-zA-Z0-9_]* *=|\\n&|g
#
s|boost::numeric::ublas::||g
s|/usr/include/boost/numeric/ublas/||g
#
s|CppAD::AD<double> *|AD|g
s|<matrix<AD>, basic_unit_lower<> >|<matrix_AD_ulower>|g
s|<matrix<AD>, basic_upper<> >|<matrix_AD_upper>|g
s|scalar_minus<AD, AD>|scalar_minus_AD|g
EOF
	echo "sed -f $name.sed $name.log > ../$name.log"
	sed -f $name.sed $name.log > ../$name.log
	echo "$name.sh: Compliation Error: see $name.log"
	exit 1
	exit 1
fi
#
echo "./$name"
if ! ./$name
then
	echo
	echo "$name.sh: Execution Error"
	exit 1
fi
echo
echo "$name.sh: OK"
exit 0
