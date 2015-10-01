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
This is not a bug, but rather a test of specialization of numeric_limits
for AD types.
EOF
cat << EOF > bug.$$
# include <cppad/cppad.hpp>

# if CPPAD_USE_CPLUSPLUS_2011
# define CPPAD_NUMERIC_LIMIT_VAL(type, name ) \\
	static constexpr type name = numeric_limits<Base>::name;
# else
# define CPPAD_NUMERIC_LIMIT_VAL(type, name ) \\
	static const type name = numeric_limits<Base>::name;
# endif

# if CPPAD_USE_CPLUSPLUS_2011
# define CPPAD_NUMERIC_LIMIT_FUN(name) \\
	static constexpr CppAD::AD<Base> name(void) \\
	{	return numeric_limits<Base>::name(); }
# else
# define CPPAD_NUMERIC_LIMIT_FUN(name) \\
	static CppAD::AD<Base> name(void) \\
	{	return numeric_limits<Base>::name(); }
# endif

namespace std {
	template <class Base> class numeric_limits< CppAD::AD<Base> >
	{
	public:
		// bool
		CPPAD_NUMERIC_LIMIT_VAL(bool, is_specialized)
		CPPAD_NUMERIC_LIMIT_VAL(bool, is_signed)
		CPPAD_NUMERIC_LIMIT_VAL(bool, is_integer)
		CPPAD_NUMERIC_LIMIT_VAL(bool, is_exact)
		CPPAD_NUMERIC_LIMIT_VAL(bool, has_infinity)
		CPPAD_NUMERIC_LIMIT_VAL(bool, has_quiet_NaN)
		CPPAD_NUMERIC_LIMIT_VAL(bool, has_signaling_NaN)
		CPPAD_NUMERIC_LIMIT_VAL(bool, has_denorm_loss)
		CPPAD_NUMERIC_LIMIT_VAL(bool, is_iec559)
		CPPAD_NUMERIC_LIMIT_VAL(bool, is_bounded)
		CPPAD_NUMERIC_LIMIT_VAL(bool, is_modulo)
		CPPAD_NUMERIC_LIMIT_VAL(bool, traps)
		CPPAD_NUMERIC_LIMIT_VAL(bool, tinyness_before)
		// int
		CPPAD_NUMERIC_LIMIT_VAL(int, digits)
		CPPAD_NUMERIC_LIMIT_VAL(int, digits10)
		CPPAD_NUMERIC_LIMIT_VAL(int, radix)
		CPPAD_NUMERIC_LIMIT_VAL(int, min_exponent)
		CPPAD_NUMERIC_LIMIT_VAL(int, min_exponent10)
		CPPAD_NUMERIC_LIMIT_VAL(int, max_exponent)
		CPPAD_NUMERIC_LIMIT_VAL(int, max_exponent10)
		// function
		CPPAD_NUMERIC_LIMIT_FUN(min)
		CPPAD_NUMERIC_LIMIT_FUN(max)
		CPPAD_NUMERIC_LIMIT_FUN(epsilon)
		CPPAD_NUMERIC_LIMIT_FUN(round_error)
		CPPAD_NUMERIC_LIMIT_FUN(infinity)
		CPPAD_NUMERIC_LIMIT_FUN(quiet_NaN)
		CPPAD_NUMERIC_LIMIT_FUN(signaling_NaN)
		CPPAD_NUMERIC_LIMIT_FUN(denorm_min)
		// other
		CPPAD_NUMERIC_LIMIT_VAL(float_denorm_style, has_denorm)
		CPPAD_NUMERIC_LIMIT_VAL(float_round_style, round_style)
# if CPPAD_USE_CPLUSPLUS_2011
		CPPAD_NUMERIC_LIMIT_VAL(int, max_digits10)
		CPPAD_NUMERIC_LIMIT_FUN(lowest)
# endif
	};
}

# define PRINT_VAL(name) \
cout << #name << " = " << std::numeric_limits< AD<double> >::name << endl;

# define PRINT_FUN(name) \
cout << #name << " = " << std::numeric_limits< AD<double> >::name() << endl;

int main(void)
{	bool ok = true;
	using std::cout;
	using std::endl;
	using CppAD::AD;
	//
	PRINT_VAL(is_specialized)
	PRINT_VAL(is_signed)
	PRINT_VAL(is_integer)
	PRINT_VAL(is_exact)
	PRINT_VAL(has_infinity)
	PRINT_VAL(has_quiet_NaN)
	PRINT_VAL(has_signaling_NaN)
	PRINT_VAL(has_denorm_loss)
	PRINT_VAL(is_iec559)
	PRINT_VAL(is_bounded)
	PRINT_VAL(is_modulo)
	PRINT_VAL(traps)
	PRINT_VAL(tinyness_before)
	// int
	PRINT_VAL(digits)
	PRINT_VAL(digits10)
	PRINT_VAL(radix)
	PRINT_VAL(min_exponent)
	PRINT_VAL(min_exponent10)
	PRINT_VAL(max_exponent)
	PRINT_VAL(max_exponent10)
	// function
	PRINT_FUN(min)
	PRINT_FUN(max)
	PRINT_FUN(epsilon)
	PRINT_FUN(round_error)
	PRINT_FUN(infinity)
	PRINT_FUN(quiet_NaN)
	PRINT_FUN(signaling_NaN)
	PRINT_FUN(denorm_min)
	// other
	PRINT_VAL(has_denorm)
	PRINT_VAL(round_style)
# if CPPAD_USE_CPLUSPLUS_2011
	PRINT_VAL(max_digits10)
	PRINT_FUN(lowest)
# endif
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
