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
package check_swig;
#
use strict;
use warnings;
sub check_swig ()
{	# load the swig library
	use pl_cppad;
	#
	# initialze exit status as OK
	my $error_count = 0;
	# --------------------------------------------
	# std::vector<double>
	# --------------------------------------------
	my $ok  = 1;
	my $vec = new pl_cppad::vector_double(4);
	# size
	$ok       = $ok && $vec->size() == 4;
	#
	# resize (does not work in perl)
	# $vec->resize(2);
	# $ok  = $ok && $vec->size() == 2;
	#
	# setting elements
	for(my $i = 0; $i < $vec->size(); $i++)
	{	$vec->set($i, 2.0 * $i); }
	# getting elements
	for(my $i = 0; $i < $vec->size(); $i++)
	{	$ok = $ok && $vec->get($i) == 2.0 * $i; }
	if( $ok )
	{	print "pl_cppad::vector_double: OK\n"; }
	else
	{	print "pl_cppad::vector_double: Error\n";
		$error_count = $error_count + 1;
	}
	# --------------------------------------------
	# a_double
	# --------------------------------------------
	$ok            = 1;
	my $two        = new pl_cppad::a_double(2.0);
	my $three      = new pl_cppad::a_double(3.0);
	my $five       = $two + $three;
	my $six        = $two * $three;
	my $neg_one    = $two - $three;
	my $two_thirds = $two / $three;
	$ok = $ok && $five->value() == 5.0;
	$ok = $ok && $six->value() == 6.0;
	$ok = $ok && $neg_one->value() == -1.0;
	$ok = $ok && 0.5 < $two_thirds->value() && $two_thirds->value() < 1.0;
	$ok = $ok && $five < $six;
	if( $ok )
	{	print "pl_cppad::a_double: OK\n"; }
	else
	{	print "pl_cppad::a_double: Error\n";
		$error_count = $error_count + 1;
	}
	# --------------------------------------------
	# std::vector<a_double>
	# --------------------------------------------
	$ok       = 1;
	my $a_vec = new pl_cppad::vector_ad(4);
	# size
	$ok  = $ok && $a_vec->size() == 4;
	#
	# resize (does not work in perl)
	# a_vec.resize(2)
	# ok  = ok and a_vec.size() == 2
	#
	# setting elements
	for(my $i = 0; $i < $a_vec->size(); $i++)
	{	my $ad = new pl_cppad::a_double(2.0 * $i);
		$a_vec->set($i, $ad);
	}
	# getting elements
	for(my $i = 0; $i < $a_vec->size(); $i++)
	{	my $ad = $a_vec->get($i);
		$ok = $ok && $ad->value() == 2.0 * $i;
	}
	if( $ok )
	{	print "pl_cppad::vector_ad: OK\n"; }
	else
	{	print "pl_cppad::vector_ad:: Error\n";
		$error_count = $error_count + 1;
	}
	# --------------------------------------------
	# afun(ax, ay)
	$ok    = 1;
	my $n  = 2;
	my $m  = 1;
	my $x  = new pl_cppad::vector_double($n);
	for(my $i = 0; $i < $n; $i++)
	{	$x->set($i, $i + 1);
	}
	my $ax = pl_cppad::independent($x);
	my $ay = new pl_cppad::vector_ad($m);
	{	my $ad_0 = $ax->get(0);
		my $ad_1 = $ax->get(1);
		$ay->set(0, $ad_0 + $ad_0 - $ad_1);
	}
	my $af     = new pl_cppad::a_fun($ax, $ay);
	my $xp     = new pl_cppad::vector_double($n);
	$xp->set(0, 3.0);
	$xp->set(1, 1.0);
	my $p      = 0;
	my $yp     = $af->forward($p, $xp);
	$ok     = $ok && $yp->get(0) == 5.0;
	$xp->set(0, 0.0);
	$xp->set(1, 1.0);
	$p      = 1;
	$yp     = $af->forward($p, $xp);
	$ok     = $ok && $yp->get(0) == -1.0;
	if( $ok )
	{	print "pl_cppad::a_fun OK\n"; }
	else
	{	print "pl_cppad::a_fun Error\n";
		$error_count = $error_count + 1;
	}
	# --------------------------------------------
	return $error_count == 0;
}
# a strange flag required by perl compiler
return 1;
