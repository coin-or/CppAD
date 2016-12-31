
# This file can be automatically generaeted using the following command
# m4 ../perl.m4 ../xam/vector_ad_xam.m4 > vector_ad_xam.pl
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
# std::vector<a_double>
# -----------------------------------------------------------------------------
package vector_ad_xam;
sub vector_ad_xam() {
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
	my $a_vec = new pl_cppad::vector_ad(n);
	#
	# check size
	$ok = $ok && $a_vec->size() == n;
	#
	# setting elements
	for(my $i = 0; $i < $n ; $i++) {
		my $ad = new pl_cppad::a_double(2.0 * i);
		$a_vec->set($i, $ad);
	}
	# getting elements
	for(my $i = 0; $i < $n ; $i++) {
		my $a_element = $a_vec->get($i);
		$ok = $ok && $a_element->value() == 2.0 * $i;
	}
	return( $ok );
}
