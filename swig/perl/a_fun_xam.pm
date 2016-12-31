
# This file can be automatically generaeted using the following command
# m4 ../perl.m4 ../xam/a_fun_xam.m4 > a_fun_xam.pl
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
package a_fun_xam;
sub a_fun_xam() {
	# check for standard perl programming conventions
	use strict;
	use warnings;
	#
	# load the cppad swig library
	use pl_cppad;
	#
	# initilaize return variable
	my $ok = 1;
	my $n = 2;
	my $m = 1;
	#
	# create ax
	my $x = new pl_cppad::vector_double(n);
	for(my $i = 0; $i < $n ; $i++) {
		$x->set($i, $i + 1.0);
	my $ax = pl_cppad::independent(x);
	#
	# create af
	my $ax0 = $ax->get(0);
	my $ax1 = $ax->get(1);
	my $ay = new pl_cppad::vector_ad(1);
	$ay->set(0, $ax0 + $ax0 - $ax1);
	my $af = new pl_cppad::a_fun($ax, $ay);
	#
	# zero order forward
	$x->set(0, 3.0);
	$x->set(1, 1.0);
	my $y = $af->forward(0, $x);
	$ok = $ok && $y->get(0) == 5.0;
	#
	# first order forward
	$x->set(0, 0.0);
	$x->set(1, 1.0);
	$y = $af->forward(1, $x);
	$ok = $ok && $y->get(0) == -1.0;
	#
	return( $ok );
}
