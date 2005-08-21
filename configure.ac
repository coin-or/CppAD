dnl BEGIN SHORT COPYRIGHT
dnl ---------------------------------------------------------------------------
dnl CppAD: C++ Algorithmic Differentiation: 
dnl Copyright (C) 2003-04 Bradley M. Bell
dnl 
dnl This program is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU General Public License
dnl as published by the Free Software Foundation; either version 2
dnl of the License, or (at your option) any later version.
dnl 
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl 
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
dnl ---------------------------------------------------------------------------
dnl END SHORT COPYRIGHT
dnl
dnl Process this file with autoconf to produce a configure script.

dnl   package   version           bug-report
AC_INIT(CppAD, 05-08-20, bradbell@seanet.com)

dnl ----------------------------------------------------------------------
dnl Command line argument results

dnl The C++ error and warning flags for every compliation
AC_SUBST(CPP_ERROR_WARN)
AC_MSG_CHECKING( [extra C++ flags] )
if test "$CPP_ERROR_WARN" ; then
	AC_MSG_RESULT( $CPP_ERROR_WARN ) 
else
	AC_MSG_RESULT( no )
fi 

dnl The prefix corresponding to the installation of Adolc
dnl The default action is to not include Adolc examples and tests
AC_MSG_CHECKING( [with Adolc] )
AC_SUBST(ADOLC_DIR)
AM_CONDITIONAL(CppAD_ADOLC, test $ADOLC_DIR )
if test $ADOLC_DIR ; then
	AC_MSG_RESULT( [yes] )
else
	AC_MSG_RESULT( [no] )
fi

dnl The directory corresponding to the installation of FADBAD
dnl The default action is to not include Fadbad examples and tests
AC_MSG_CHECKING( [with Fadbad] )
AC_SUBST(FADBAD_DIR)
AM_CONDITIONAL(CppAD_FADBAD, test $FADBAD_DIR )
if test $FADBAD_DIR ; then
	AC_MSG_RESULT( [yes] )
else
	AC_MSG_RESULT( [no] )
fi

dnl Are we using standard vectors
dnl This will define CppADvector as std::vector
AC_MSG_CHECKING( [with std vector] )
AC_ARG_WITH(stdvector, 
	AC_HELP_STRING( [--with-stdvector], [default is no] ),
	[stdvector="yes"],
	[stdvector="no"]
)
if test "$stdvector" = "no" ; then
	AC_DEFINE(CppAD_STDVECTOR, 0, [not using stdandard vector])
	AC_MSG_RESULT( [no] )
else
	AC_DEFINE(CppAD_STDVECTOR, 1, [using stdandard vector])
	AC_MSG_RESULT( [yes] )
fi

dnl The directory corresponding to the installation of BOOST
dnl This will define CppADvector as boost::numeric::ublas::vector
AC_MSG_CHECKING( [with boost vector] )
AC_SUBST(BOOST_DIR)
AM_CONDITIONAL(CppAD_BOOSTVECTOR, test $BOOST_DIR )
if test "X${BOOST_DIR}" = "X"; then
	AC_DEFINE(CppAD_BOOSTVECTOR, 0, [not using boost vectors])
	AC_MSG_RESULT( [no] )
else
	AC_DEFINE(CppAD_BOOSTVECTOR, 1, [using boost vectors])
	AC_MSG_RESULT( [yes] )
fi

dnl Check if we are using the default definition for CppADvector
AC_MSG_CHECKING( [with CppAD vector] )
if test "$stdvector${BOOST_DIR}" = "no"; then
	AC_DEFINE(CppAD_CPPADVECTOR, 1, [using CppAD vectors])
	AC_MSG_RESULT( [yes] )
else
	AC_DEFINE(CppAD_CPPADVECTOR, 0, [using CppAD vectors])
	AC_MSG_RESULT( [no] )
fi

dnl Are we including profiling
AC_MSG_CHECKING( [with profiling] )
AC_ARG_WITH(profiling, 
	AC_HELP_STRING( [--with-profiling], [default is no] ),
	[profiling="yes"],
	[profiling="no"]
)
AM_CONDITIONAL(CppAD_PROFILING, test $profiling = "yes" )
if test $profiling = "yes"; then
	AC_MSG_RESULT( [yes] )
else
	AC_MSG_RESULT( [no] )
fi
dnl ----------------------------------------------------------------------

dnl Set the default install prefix to the installers home directory
AC_PREFIX_DEFAULT(${HOME})

dnl Run any macros required for proper operation of generated Makefiles
dnl nostdinc: Do not define standard include directories in generated Makefiles
AM_INIT_AUTOMAKE(nostdinc)

dnl Determine if ranlib is present and where it is if present (set RANLIB)
AC_PROG_RANLIB

dnl AC_PROG_CXX([compiler-search-list]) outputs CXX as C++ compiler to use
AC_PROG_CXX

dnl No default setting for compiler flags
AC_SUBST(CXXFLAGS, "")

dnl Have configure check source directory to see if CppAD/CppAD.h is there
AC_CONFIG_SRCDIR( CppAD/CppAD.h )

dnl Use config.h instead of make file command line arguments
AC_CONFIG_HEADER( CppAD/config.h )

dnl AC_PROG_CP outputs CPP as command to run C preprocessor
AC_PROG_CPP

dnl Do nothing if complier supports inline, otherwise try to find a subsitute
dnl definition or, if cannot, define inline as empty
AC_C_INLINE

dnl AC_CONFIG_FILES(file-list) for each file in the list, configure will 
dnl read file.in, do its substitutions, and create file
AC_CONFIG_FILES([ 
	Makefile
	Adolc/Makefile 
	Example/Makefile 
	Fadbad/Makefile 
	GetStarted/Makefile 
	PrintFor/Makefile
	lib/Makefile
	Speed/Makefile 
	TestMore/Makefile 
])


dnl create all the requested output files
AC_OUTPUT
