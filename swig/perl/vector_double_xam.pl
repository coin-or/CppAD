
# This file can be automatically generaeted using the following command
# m4 ../perl.m4 ../xam/vector_double_xam.m4 > vector_double_xam.pl
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
# std::vector<double>
# -----------------------------------------------------------------------------
package vector_double_xam;
sub vector_double_xam() {
	# check for standard perl programming conventions
	use strict;
	use warnings;
	#
	# load the cppad swig library
	use pl_cppad;
	#
	# initilaize return variable
	my $ok = 1;
	my $n = 4;
	my $vec = new pl_cppad::vector_double(n);
	#
	# check size
	$ok = $ok && $vec->size() == n;
	#
	# setting elements
	for(my $i = 0; $i < $n ; $i++) {
		$vec->set($i, 2.0 * $i);
	}
	# getting elements
	for(my $i = 0; $i < $n ; $i++) {
		my $element = $vec->get($i);
		$ok = $ok && $element == 2.0 * $i;
	}
	return( $ok );
}
