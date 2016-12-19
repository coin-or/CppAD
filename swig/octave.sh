#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#! /bin/bash -e
# ---------------------------------------------------------------------------
# create new build directory
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
#
# create my_example_octave.cpp
echo_eval swig -octave -I.. -c++ -o my_example_octave.cpp ../my_example.i
echo 'fix warnings about unitialized variables in my_example_octave.cpp'
sed \
	-e '/^ *int *val1 *;/s/;/= 0 ;/' \
	-e '/^ *int *val2 *;/s/;/= 0 ;/' \
	-e '/^ *int *val3 *;/s/;/= 0 ;/' \
	-i my_example_octave.cpp
#
# build module that is loadable by octave
echo_eval mkoctfile -I.. ../my_example.cpp my_example_octave.cpp \
	-o my_example.oct
# ---------------------------------------------------------------------------
# text module
cat << EOF > my_example.m
% load the module
my_example
% initialze exit status as OK
error_count = 0;
% ---------------------------------------------
if (my_example.my_fact(4) == 24)
	printf("my_example.my_fact: OK\n")
else
	printf("my_example.my_fact: Error\n")
	error_count = error_count + 1;
end
% ---------------------------------------------
if (my_example.my_mod(4,3) == 1)
	printf("my_example.my_mod: OK\n")
else
	printf("my_example.my_mod: Error\n")
	error_count = error_count + 1;
end
% ---------------------------------------------
if (my_example.my_message() == "OK" )
	printf("my_example.my_message: OK\n")
else
	printf("my_example.my_message: Error\n")
	error_count = error_count + 1;
end
% ---------------------------------------------
ptr = my_example.int_ctor();
my_example.my_add(3, 4, ptr)
if( ptr.value() == 7 )
	printf("my_example.my_add: OK\n")
else
	printf("my_example.my_add: Error\n")
	error_count = error_count + 1;
end
%
n   = 10;
vec = my_example.new_int_vector(n);
for i = 0 : (n-1)
	my_example.int_vector_setitem(vec, i, 2 * i);
endfor
if( my_example.my_max(n, vec) == 18 )
	printf("my_example.my_max: OK\n")
else
	printf("my_example.my_max: Error\n")
	error_count = error_count + 1;
end
my_example.delete_int_vector(vec);
% ---------------------------------------------
% return error_count
exit(error_count)
EOF
# ---------------------------------------------------------------------------
if octave -q my_example.m
then
	echo 'All tests passed'
else
	echo 'At least one test failed'
fi
