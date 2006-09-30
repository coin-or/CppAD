dnl ---------------------------------------------------------------------------
dnl CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
dnl
dnl CppAD is distributed under multiple licenses. This distribution is under
dnl the terms of the
dnl                     Common Public License Version 1.0.
dnl 
dnl A copy of this license is included in the COPYING file of this distribution.
dnl Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
dnl ---------------------------------------------------------------------------
dnl
dnl Process this file with autoconf to produce a configure script.
dnl   package   version           bug-report
AC_INIT(CppAD, 06-09-29, bradbell@seanet.com)

dnl Set the default install prefix to the installers home directory
AC_PREFIX_DEFAULT(${HOME})

AC_MSG_NOTICE([******** Begin Checking Command Line arguments ***************])

dnl the prefix directory corresponding to CppAD -------------------------
dnl The following seems to work but NONE is not in autoconf specifications
dnl AC_MSG_CHECKING([prefix])
dnl if test "x$prefix" = "NONE" ; then
dnl 	AC_MSG_RESULT([$HOME])
dnl else
dnl 	AC_MSG_RESULT([$prefix])
dnl fi
AC_MSG_CHECKING([--prefix])
AC_MSG_RESULT([your choice can't be reported here (autoconf limitation)])


dnl Are we building the GetStarted directory -----------------------------
AC_MSG_CHECKING([--with-GetStarted])
AC_ARG_WITH(GetStarted, 
	AC_HELP_STRING([--with-GetStarted], [default is no]),
	[GetStarted="yes"],
	[GetStarted="no"]
)
AM_CONDITIONAL(CppAD_GETSTARTED, test "$GetStarted" = "yes")
AC_MSG_RESULT([$GetStarted])

dnl Are we building the Introduction directory ---------------------------
AC_MSG_CHECKING([--with-Introduction])
AC_ARG_WITH(Introduction, 
	AC_HELP_STRING([--with-Introduction], [default is no]),
	[Introduction="yes"],
	[Introduction="no"]
)
AM_CONDITIONAL(CppAD_INTRODUCTION, test "$Introduction" = "yes")
AC_MSG_RESULT([$Introduction])

dnl Are we building the Example directory --------------------------------
AC_MSG_CHECKING([--with-Example])
AC_ARG_WITH(Example, 
	AC_HELP_STRING([--with-Example], [default is no]),
	[Example="yes"],
	[Example="no"]
)
AM_CONDITIONAL(CppAD_EXAMPLE, test "$Example" = "yes")
AC_MSG_RESULT([$Example])

dnl Are we building the TestMore directory -------------------------------
AC_MSG_CHECKING([--with-TestMore])
AC_ARG_WITH(TestMore, 
	AC_HELP_STRING([--with-TestMore], [default is no]),
	[TestMore="yes"],
	[TestMore="no"]
)
AM_CONDITIONAL(CppAD_TESTMORE, test "$TestMore" = "yes")
AC_MSG_RESULT([$TestMore])

dnl Are we building the Speed directory ----------------------------------
AC_MSG_CHECKING([--with-Speed])
AC_ARG_WITH(Speed, 
	AC_HELP_STRING([--with-Speed], [default is no]),
	[Speed="yes"],
	[Speed="no"]
)
AM_CONDITIONAL(CppAD_SPEED, test "$Speed" = "yes")
AC_MSG_RESULT([$Speed])

dnl Are we building the PrintFor directory --------------------------------
AC_MSG_CHECKING([--with-PrintFor])
AC_ARG_WITH(PrintFor, 
	AC_HELP_STRING([--with-PrintFor], [default is no]),
	[PrintFor="yes"],
	[PrintFor="no"]
)
AM_CONDITIONAL(CppAD_PRINTFOR, test "$PrintFor" = "yes")
AC_MSG_RESULT([$PrintFor])

dnl Are we building the SpeedExample directory --------------------------------
AC_MSG_CHECKING([--with-SpeedExample])
AC_ARG_WITH(SpeedExample, 
	AC_HELP_STRING([--with-SpeedExample], [default is no]),
	[SpeedExample="yes"],
	[SpeedExample="no"]
)
AM_CONDITIONAL(CppAD_SPEEDEXAMPLE, test "$SpeedExample" = "yes")
AC_MSG_RESULT([$SpeedExample])

dnl Are we including profiling -------------------------------------------
AC_MSG_CHECKING([--with-profiling])
AC_ARG_WITH(profiling, 
	AC_HELP_STRING([--with-profiling], [default is no]),
	[profiling="yes"],
	[profiling="no"]
)
AM_CONDITIONAL(CppAD_PROFILING, test "$profiling" = "yes")
AC_MSG_RESULT([$profiling])

dnl Are we using standard vectors ----------------------------------------
AC_MSG_CHECKING([--with-stdvector])
AC_ARG_WITH(stdvector, 
	AC_HELP_STRING([--with-stdvector], [default is no]),
	[stdvector="yes"],
	[stdvector="no"]
)
if test "$stdvector" = "yes" ; then
	AC_DEFINE(CppAD_STDVECTOR, 1, [is CppADvector == std::vector])
else
	AC_DEFINE(CppAD_STDVECTOR, 0, [is CppADvector == std::vector])
fi
AC_MSG_RESULT([$stdvector])

dnl The postfix directiory corresponding to this installation of CppAD --
AC_MSG_CHECKING([POSTFIX_DIR])
AC_SUBST(POSTFIX_DIR)
AM_CONDITIONAL(CppAD_POSTFIX, test "$POSTFIX_DIR" != "")
if test "$POSTFIX_DIR" != "" ; then
	AC_MSG_RESULT([$POSTFIX_DIR])
else
	AC_MSG_RESULT([no])
fi

dnl The prefix corresponding to the installation of Adolc ---------------
AC_MSG_CHECKING([ADOLC_DIR])
AC_SUBST(ADOLC_DIR)
AM_CONDITIONAL(CppAD_ADOLC, test "$ADOLC_DIR" != "")
if test "$ADOLC_DIR" != "" ; then
	AC_MSG_RESULT([$ADOLC_DIR])
else
	AC_MSG_RESULT([no])
fi

dnl The directory corresponding to the installation of FADBAD -----------
AC_MSG_CHECKING([FADBAD_DIR])
AC_SUBST(FADBAD_DIR)
AM_CONDITIONAL(CppAD_FADBAD, test "$FADBAD_DIR" != "")
if test "$FADBAD_DIR" != "" ; then
	AC_MSG_RESULT([$FADBAD_DIR])
else
	AC_MSG_RESULT([no])
fi

dnl The directory corresponding to the installation of BOOST ------------
AC_MSG_CHECKING([BOOST_DIR])
AC_SUBST(BOOST_DIR)
AM_CONDITIONAL(CppAD_BOOSTVECTOR, test "$BOOST_DIR" != "")
if test "$BOOST_DIR" != "" ; then
	AC_DEFINE(CppAD_BOOSTVECTOR, 1, 
		[is CppADvector == boost::numeric::ublas::vector]
	)
	AC_MSG_RESULT([$BOOST_DIR])
else
	AC_DEFINE(CppAD_BOOSTVECTOR, 0, 
		[is CppADvector == boost::numeric::ublas::vector]
	)
	AC_MSG_RESULT([no])
fi

dnl The C++ error and warning flags for every compliation ---------------
AC_MSG_CHECKING([CPP_ERROR_WARN])
AC_SUBST(CPP_ERROR_WARN)
if test "$CPP_ERROR_WARN" != "" ; then
	AC_MSG_RESULT([$CPP_ERROR_WARN]) 
else
	AC_MSG_RESULT([no])
fi 
AC_MSG_NOTICE([********** End Checking Command Line arguments ***************])

dnl Check which definition we are using for CppADvector
if test "$stdvector$BOOST_DIR" = "no" ; then
	AC_DEFINE(CppAD_CPPADVECTOR, 1, [is CppADvector == CppAD::vector])
else 
	AC_DEFINE(CppAD_CPPADVECTOR, 0, [is CppADvector == CppAD::vector])
fi

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
AC_CONFIG_SRCDIR(CppAD/CppAD.h)

dnl Use config.h instead of make file command line arguments
AC_CONFIG_HEADER(CppAD/config.h)

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
	Introduction/Makefile
	PrintFor/Makefile
	SpeedExample/Makefile
	Speed/Makefile 
	TestMore/Makefile 
])


dnl create all the requested output files
AC_OUTPUT
