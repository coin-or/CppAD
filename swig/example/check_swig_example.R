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
options(echo = FALSE)
# Turn R echo off (for R CMD BATCH execution)
#
# load the module
swig_module = 'swig_example'
dyn.load( paste( swig_module, .Platform$dynlib.ext, sep='') )
source( paste( swig_module, '.R', sep='') )
cacheMetaData(1)
#
# initialize error count
error_count = 0
# --------------------------------------------
# factorial_by_val
if( factorial_by_val(4) == 24 ) {
	write( 'factorial_by_val: OK', stdout() )
} else {
	write( 'factorial_by_val: Error', stdout() )
	error_count = error_count + 1
}
# ---------------------------------------------
# message_of_void
if( message_of_void() == 'OK' ) {
    write( 'message_of_void: OK', stdout() )
} else {
    write( 'message_of_void: Error', stdout() )
	error_count = error_count + 1
}
# ---------------------------------------------
# return error_count
quit(save = 'no', status = error_count, runLast = FALSE)
EOF
