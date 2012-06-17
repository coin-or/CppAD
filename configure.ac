dnl $Id$
dnl ---------------------------------------------------------------------------
dnl CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
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
dnl   package   version              bug-report
AC_INIT(CppAD, 20120617, cppad@list.coin-or.org)
AC_SUBST(PACKAGE_URL, "http://www.coin-or.org/CppAD")
AC_SUBST(PACKAGE_DESCRIPTION, "Differentiation of C++ Algorithms")

dnl By defalut disable maintainer mode when running configure;
dnl --enable-maintainer-mode will enable it.
dnl This avoids the autoconf and automake tools being launched by make
AM_MAINTAINER_MODE

dnl Set the default install prefix to the installers home directory
dnl cygwin begin delete:
AC_PREFIX_DEFAULT(${HOME})
dnl cygwin end delete:
dnl cygwin replace: AC_PREFIX_DEFAULT(/usr)

dnl --------------------------------------------------------------------------
dnl Check for programs

AC_CHECK_PROG(have_pkg_config, [pkg-config], [yes], [no])
AM_CONDITIONAL(CppAD_PKG_CONFIG, test "$have_pkg_config" = "yes")

AC_CHECK_PROG(have_ar, [ar], [yes], [no])
AM_CONDITIONAL(CppAD_HAVE_AR, test "$have_ar" = "yes")
if test "$have_ar" = "no"; then
	AC_MSG_WARN(
	[cannot build any libraries because ar is missing, speed/* is not used] 
	)
fi

dnl ----------------------------------------------------------------------
AC_MSG_NOTICE([*** Check command line arguments that are longer used ****])

if test "$CPP_ERROR_WARN" != "" ; then
    AC_MSG_ERROR([The CPP_ERROR_WARN configure option is now CXX_FLAGS])
fi
dnl
AC_ARG_WITH(SpeedExample, AC_HELP_STRING(
	[--with-SpeedExample], [is not necessary, see install instructions.]),
	[SpeedExample="yes"], [SpeedExample="no"]
)
if test "$SpeedExample" = "yes" ; then
    AC_MSG_ERROR([--with-SpeedExample not necessary, see install instructions.])
fi
dnl
AC_ARG_WITH(profiling, AC_HELP_STRING(
	[--with-profiling], [is not necessary, see install instructions.]),
	[profiling="yes"], [profiling="no"]
)
if test "$profiling" = "yes" ; then
    AC_MSG_ERROR([--with-profiling not necessary, see install instructions.])
fi
dnl 
AC_ARG_WITH(Speed, AC_HELP_STRING(
	[--with-Speed], [is not necessary, see install instructions.]),
	[Speed="yes"], [Speed="no"]
)
if test "$Speed" = "yes" ; then
    AC_MSG_ERROR([--with-Speed not necessary, see install instructions.])
fi
dnl 
AC_ARG_WITH(Introduction, AC_HELP_STRING(
	[--with-Introduction], [is not necessary, see install instructions.]),
	[Introduction="yes"], [Introduction="no"]
)
if test "$Introduction" = "yes" ; then
    AC_MSG_ERROR([--with-Introduction not necessary, see install instructions.])
fi
dnl 
AC_ARG_WITH(Example, AC_HELP_STRING(
	[--with-Example], [is not necessary, see install instructions.]),
	[Example="yes"], [Example="no"]
)
if test "$Example" = "yes" ; then
    AC_MSG_ERROR([--with-Example not necessary, see install instructions.])
fi
dnl 
AC_ARG_WITH(TestMore, AC_HELP_STRING(
	[--with-TestMore], [is not necessary, see install instructions.]),
	[TestMore="yes"], [TestMore="no"]
)
if test "$TestMore" = "yes" ; then
    AC_MSG_ERROR([--with-TestMore not necessary, see install instructions.])
fi
dnl 
AC_ARG_WITH(PrintFor, AC_HELP_STRING(
	[--with-PrintFor], [is not necessary, see install instructions.]),
	[PrintFor="yes"], [PrintFor="no"]
)
if test "$PrintFor" = "yes" ; then
    AC_MSG_ERROR([--with-PrintFor not necessary, see install instructions.])
fi
dnl --------------------------------------------------------------------------
AC_MSG_NOTICE([******  Check command line arguments that are used ***********])
dnl the prefix directory corresponding to CppAD -------------------------
dnl The following seems to work but NONE is not in autoconf specifications
AC_MSG_CHECKING([--prefix])
if test "$prefix" = "NONE" ; then
	AC_MSG_RESULT([$HOME])
else
	AC_MSG_RESULT([$prefix])
fi

dnl Are we installing the documentation -----------------------------------
dnl cygwin begin delete:
AC_MSG_CHECKING([--with-Documentation])
AC_ARG_WITH(Documentation, 
	AC_HELP_STRING([--with-Documentation], [default is no]),
	[Documentation="yes"],
	[Documentation="no"]
)
AM_CONDITIONAL(CppAD_DOCUMENTATION, test "$Documentation" = "yes")
AC_MSG_RESULT([$Documentation])
dnl cygwin end delete:
dnl cygwin replace: AM_CONDITIONAL(CppAD_DOCUMENTATION, test "yes" = "yes")

dnl Are we using standard vectors ----------------------------------------
AC_MSG_CHECKING([--with-stdvector])
AC_ARG_WITH(stdvector, 
	AC_HELP_STRING([--with-stdvector], [default is no]),
	[stdvector="yes"],
	[stdvector="no"]
)
if test "$stdvector" = "yes" ; then
	AC_SUBST(CPPAD_STDVECTOR, 1)
else
	AC_SUBST(CPPAD_STDVECTOR, 0)
fi
AC_MSG_RESULT([$stdvector])

dnl Are we using boost vectors ----------------------------------------
AC_MSG_CHECKING([--with-boostvector])
AC_ARG_WITH(boostvector, 
	AC_HELP_STRING([--with-boostvector], [default is no]),
	[boostvector="yes"],
	[boostvector="no"]
)
AC_MSG_RESULT([$boostvector])
if test "$boostvector$stdvector" = "yesyes" ; then
	AC_MSG_ERROR([cannot have both --with-stdvector and --with-boostvector])
fi
if test "$boostvector" = "yes" ; then
	AC_SUBST(CPPAD_BOOSTVECTOR, 1)
else
	AC_SUBST(CPPAD_BOOSTVECTOR, 0)
fi

dnl The postfix directiory corresponding to CppAD ------------------------
AC_MSG_CHECKING([POSTFIX_DIR])
AC_ARG_VAR(POSTFIX_DIR, 
	[postfix directory for CppAD installation]
)
AM_CONDITIONAL(CppAD_POSTFIX, test "$POSTFIX_DIR" != "")
if test "$POSTFIX_DIR" != "" ; then
	AC_MSG_RESULT([$POSTFIX_DIR])
else
	AC_MSG_RESULT([no])
fi

dnl The prefix corresponding to the installation of Adolc ---------------
AC_MSG_CHECKING([ADOLC_DIR])
AC_ARG_VAR(ADOLC_DIR, 
	[value of configure prefix directory during Adolc install]
)
AM_CONDITIONAL(CppAD_ADOLC, test "$ADOLC_DIR" != "")
if test "$ADOLC_DIR" != "" ; then
	AC_MSG_RESULT([$ADOLC_DIR])
	if test "$have_ar" = "no"; then
		AC_MSG_ERROR([ar is missing and ADOLC_DIR is present])
	fi
else
	AC_MSG_RESULT([no])
fi

dnl The prefix corresponding to the installation of Eigen ---------------
AC_MSG_CHECKING([EIGEN_DIR])
AC_ARG_VAR(EIGEN_DIR, 
	[value of configure prefix directory during Eigen install]
)
AM_CONDITIONAL(CppAD_EIGEN, test "$EIGEN_DIR" != "")
if test "$EIGEN_DIR" != "" ; then
	AC_MSG_RESULT([$EIGEN_DIR])
else
	AC_MSG_RESULT([no])
fi

dnl The directory corresponding to the installation of FADBAD -----------
AC_MSG_CHECKING([FADBAD_DIR])
AC_ARG_VAR(FADBAD_DIR,
	[parent directory directly above FADBAD++ directory]
)
AM_CONDITIONAL(CppAD_FADBAD, test "$FADBAD_DIR" != "")
if test "$FADBAD_DIR" != "" ; then
	AC_MSG_RESULT([$FADBAD_DIR])
	if test "$have_ar" = "no"; then
		AC_MSG_ERROR([ar is missing and FADBAD_DIR is present])
	fi
else
	AC_MSG_RESULT([no])
fi

dnl The directory corresponding to the installation of SACADO -----------
AC_MSG_CHECKING([SACADO_DIR])
AC_ARG_VAR(SACADO_DIR,
	[value of the configure prefix directory during Sacado install]
)
AM_CONDITIONAL(CppAD_SACADO, test "$SACADO_DIR" != "")
if test "$SACADO_DIR" != "" ; then
	AC_MSG_RESULT([$SACADO_DIR])
	if test "$have_ar" = "no"; then
		AC_MSG_ERROR([ar is missing and SACADO_DIR is present])
	fi
else
	AC_MSG_RESULT([no])
fi

dnl The directory corresponding to the installation of IPOPT -----------
AC_MSG_CHECKING([IPOPT_DIR])
AC_ARG_VAR(IPOPT_DIR,
	[value of the configure prefix directory during Ipopt install]
)
AM_CONDITIONAL(CppAD_IPOPT_CPPAD, test "$IPOPT_DIR" != "")
if test "$IPOPT_DIR" != "" ; then
	AC_MSG_RESULT([$IPOPT_DIR])
	if test "$have_ar" = "no"; then
		AC_MSG_ERROR([ar is missing and IPOPT_DIR is present])
	fi
else
	AC_MSG_RESULT([no])
fi

dnl The directory corresponding to the installation of BOOST ------------
AC_MSG_CHECKING([BOOST_DIR])
AC_ARG_VAR(BOOST_DIR,
	[parent directory directly above boost directory]
)
AM_CONDITIONAL(CppAD_BOOST_DIR, test "$BOOST_DIR" != "")
if test "$BOOST_DIR" != "" ; then
	AC_MSG_RESULT([$BOOST_DIR])
else
	AC_MSG_RESULT([no])
fi

dnl The C++ compiler flags CXX_FLAGS -------------------------------------
AC_SUBST(CXXFLAGS, "")
#
AC_MSG_CHECKING([CXX_FLAGS])
AC_ARG_VAR(CXX_FLAGS,
	[ CXXFLAGS is ignored, use CXX_FLAGS for compiler flags]
)
# cxx_flags for fadbad and eigen
if test "$CXX_FLAGS" != "" ; then
	AC_MSG_RESULT([$CXX_FLAGS]) 
	cxx_flags_fadbad=`echo "$CXX_FLAGS" | sed -e 's|-Wshadow||'`
	cxx_flags_eigen="$cxx_flags_fadbda -Wno-long-long" 
else
	AC_MSG_RESULT([no])
	cxx_flags_fadbad=""
	cxx_flags_eigen=""
fi 
AC_SUBST(CXX_FLAGS_FADBAD, "$cxx_flags_fadbad")
AC_SUBST(CXX_FLAGS_EIGEN,  "$cxx_flags_eigen")

dnl The compiler OpenMP flags OPENMP_FLAGS ----------------------------------
AC_MSG_CHECKING([OPENMP_FLAGS])
AC_ARG_VAR(OPENMP_FLAGS,
	[ flags that instruct compiler to interpret OpenMP directrives ]
)
if test "$OPENMP_FLAGS" != "" ; then
	AC_MSG_RESULT([$OPENMP_FLAGS]) 
else
	AC_MSG_RESULT([no])
fi 
AM_CONDITIONAL(CppAD_OPENMP, test "$OPENMP_FLAGS" != "")

dnl The compiler OpenMP flags MAX_NUM_THREADS -------------------------------
AC_MSG_CHECKING([MAX_NUM_THREADS])
AC_ARG_VAR(MAX_NUM_THREADS,
	[ maximum number of threads that can be used with CppAD ]
)
if test "$MAX_NUM_THREADS" != "" ; then
	AC_MSG_RESULT([$MAX_NUM_THREADS]) 
  	if test "$MAX_NUM_THREADS" -lt "4" ; then
		AC_MSG_ERROR(
			[ MAX_NUM_THREADS=$MAX_NUM_THREADS is less than 4 ]
		)
	fi
	max_num_threads="$MAX_NUM_THREADS"
else
	max_num_threads="48"
	AC_MSG_RESULT([no, using default value $max_num_threads])
fi 
AC_SUBST(CPPAD_MAX_NUM_THREADS, [$max_num_threads])

dnl The type used for addresses in AD tapes [ default = unsigned int ] ---
AC_MSG_CHECKING([TAPE_ADDR_TYPE])
AC_ARG_VAR(TAPE_ADDR_TYPE, [type used for addresses in AD tapes])
if test "$TAPE_ADDR_TYPE" == "" ; then
	addr_t="unsigned int"
else
	addr_t="$TAPE_ADDR_TYPE"
fi
AC_SUBST(CPPAD_TAPE_ADDR_TYPE, [$addr_t])
AC_MSG_RESULT([$addr_t])
ok="no"
if test "$addr_t" == "size_t" ; then
	ok="yes"
fi
if test "$addr_t" == "int" ; then
	AC_MSG_WARN(
		[TAPE_ADDR_TYPE=int is for CppAD developers (not users)]
	)
	ok="yes"
fi
if test "$addr_t" == "unsigned int" ; then
	ok="yes"
fi
if test "$addr_t" == "unsigned short int" ; then
	ok="yes"
fi
if test "$ok" == "no" ; then
  	AC_MSG_ERROR(
		[$addr_t is not a valid choice for TAPE_ADDR_TYPE]
	)
fi
AC_MSG_NOTICE([********** End Checking Command Line arguments ***************])

dnl Determine if size_t has same size as unsigned int 
AC_CHECK_SIZEOF([size_t])
AC_CHECK_SIZEOF([unsigned int])
if test "$ac_cv_size_t" == "$ac_cv_unsigned_int" ; then
	AC_SUBST(CPPAD_SIZE_T_SAME_UNSIGNED_INT, 1)
else
	AC_SUBST(CPPAD_SIZE_T_SAME_UNSIGNED_INT, 0)
fi

dnl Determine which definition we are using for CPPAD_TEST_VECTOR
if test "$stdvector$boostvector" = "nono" ; then
	AC_SUBST(CPPAD_CPPADVECTOR, 1)
else 
	AC_SUBST(CPPAD_CPPADVECTOR, 0)
fi

dnl the gettimeofday check automatically generates a yes / no configure msg
AC_CHECK_FUNC(
	gettimeofday,
	[gettimeofday="yes"],
	[gettimeofday="no"]
)
if test "$gettimeofday" = "yes" ; then
	AC_SUBST(CPPAD_GETTIMEOFDAY, 1)
else
	AC_SUBST(CPPAD_GETTIMEOFDAY, 0)
fi

dnl Run any macros required for proper operation of generated makefiles
dnl nostdinc: Do not define standard include directories in generated makefiles
AM_INIT_AUTOMAKE(nostdinc)

dnl Use LIBTOOL for cppad_ipopt ?
dnl AC_PROG_LIBTOOL

dnl Determine if ranlib is present and where it is if present (set RANLIB)
AC_PROG_RANLIB

dnl AC_PROG_CXX([compiler-search-list]) outputs CXX as C++ compiler to use
AC_PROG_CXX
AC_MSG_CHECKING([whether using Microsoft C++ compiler])
AM_CONDITIONAL(CppAD_MS_COMPILER, test "$CXX" == "cl" || test "$CXX" == "icl")
if test "$CXX" == "cl" || test "$CXX" == "icl"; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi


dnl Have configure check source directory to see if cppad/CppAD.h is there
AC_CONFIG_SRCDIR(cppad/CppAD.h)

dnl AC_PROG_CP outputs CPP as command to run C preprocessor
AC_PROG_CPP

dnl Do nothing if complier supports inline, otherwise try to find a subsitute
dnl definition or, if cannot, define inline as empty
AC_C_INLINE

dnl determine FCLIBS, flags necessary for linking C++ with modern fortran
AC_FC_LIBRARY_LDFLAGS

dnl absolute path corresponding to top_builddig and top_srcdir in makefile.am
dnl (used by cppad-uninstalled.pc.in).
abs_top_builddir=`pwd`
AC_SUBST(ABS_TOP_BUILDDIR, "$abs_top_builddir")
abs_top_srcdir=`cd $srcdir ; pwd`
AC_SUBST(ABS_TOP_SRCDIR, "$abs_top_srcdir")
dnl --------------------------------------------------------------------------
dnl Check for C libraries

dnl check for pthread library
AC_CHECK_LIB(
	[pthread],
	[pthread_barrier_wait],
	[pthread="yes"],
	[pthread="no"]
)
AM_CONDITIONAL(CppAD_PTHREAD, test "$pthread" = "yes")
if test "$pthread" == "yes" ; then
	AC_SUBST(PTHREAD_LIB,"-lpthread")
else
	AC_SUBST(PTHREAD_LIB,"")
fi

dnl check for dl library
AC_SUBST(DL_LIB, "")
AC_CHECK_LIB(
	[dl],
	[dlopen],
	[DL_LIB="-ldl"],
	[DL_LIB=""]
)
dnl --------------------------------------------------------------------------
dnl check for boost thread library
AC_LANG_PUSH([C++])
LDFLAGS_save=$LDFLAGS
CXXFLAGS_save=$CXXFLAGS
boost_thread_found='no'
if test "$BOOST_DIR" != '' ; then
	CXXFLAGS="-I$BOOST_DIR/include $CXXFLAGS"
fi
if test "$BOOST_DIR" == '' && test "$boost_thread_found" == 'no' ; then
	bthread_lib='-lboost_thread-mt'
	LDFLAGS="$LDFLAGS_save $bthread_lib"
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM(
			[# include <boost/thread.hpp>],
			[boost::barrier wait(1);]
		)],
		[boost_thread_found='yes'],
		[boost_thread_found='no']
	)
fi
if test "$BOOST_DIR" != '' && test "$boost_thread_found" == 'no' ; then
	bthread_lib="$BOOST_DIR/lib/libboost_thread-mt.so"
	LDFLAGS="$LDFLAGS_save $bthread_lib"
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM(
			[# include <boost/thread.hpp>],
			[boost::barrier wait(1);]
		)],
		[boost_thread_found='yes'],
		[boost_thread_found='no']
	)
fi
if test "$BOOST_DIR" != '' && test "$boost_thread_found" == 'no' ; then
	bthread_lib="$BOOST_DIR/lib/libboost_thread-mt.a"
	LDFLAGS="$LDFLAGS_save $bthread_lib"
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM(
			[# include <boost/thread.hpp>],
			[boost::barrier wait(1);]
		)],
		[boost_thread_found='yes'],
		[boost_thread_found='no']
	)
fi
if test "$BOOST_DIR" == '' && test "$boost_thread_found" == 'no' ; then
	bthread_lib='-lboost_thread'
	LDFLAGS="$LDFLAGS_save $bthread_lib"
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM(
			[# include <boost/thread.hpp>],
			[boost::barrier wait(1);]
		)],
		[boost_thread_found='yes'],
		[boost_thread_found='no']
	)
fi
if test "$BOOST_DIR" != '' && test "$boost_thread_found" == 'no' ; then
	bthread_lib="$BOOST_DIR/lib/libboost_thread.so"
	LDFLAGS="$LDFLAGS_save $bthread_lib"
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM(
			[# include <boost/thread.hpp>],
			[boost::barrier wait(1);]
		)],
		[boost_thread_found='yes'],
		[boost_thread_found='no']
	)
fi
if test "$BOOST_DIR" != '' && test "$boost_thread_found" == 'no' ; then
	bthread_lib="$BOOST_DIR/lib/libboost_thread.a"
	LDFLAGS="$LDFLAGS_save $bthread_lib"
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM(
			[# include <boost/thread.hpp>],
			[boost::barrier wait(1);]
		)],
		[boost_thread_found='yes'],
		[boost_thread_found='no']
	)
fi
AM_CONDITIONAL(CppAD_BTHREAD, test "$boost_thread_found" = 'yes')
if test "$boost_thread_found" == 'yes' ; then
	AC_SUBST(BTHREAD_LIB,"$bthread_lib")
	AC_MSG_RESULT([boost::thread library... yes])
else
	AC_SUBST(BTHREAD_LIB,'')
	AC_MSG_RESULT([boost::thread library... no])
fi
LDFLAGS=$LDFLAGS_save
CXXFLAGS=$CXXFLAGS_save
AC_LANG_POP([C++])
#
dnl --------------------------------------------------------------------------
dnl Library directories
if test -e $IPOPT_DIR/lib/coin ; then
	IPOPT_LIBS="-L $IPOPT_DIR/lib/coin -L $IPOPT_DIR/lib/coin/ThirdParty"
	IPOPT_LD_PATH="$IPOPT_DIR/lib/coin:$IPOPT_DIR/lib/coin/ThirdParty"
else
	IPOPT_LIBS="-L $IPOPT_DIR/lib"
	IPOPT_LD_PATH="$IPOPT_DIR/lib"
fi
AC_SUBST(CPPAD_IPOPT_LD_PATH, "$IPOPT_LD_PATH")
#
AC_SUBST(CPPAD_IPOPT_LIBS, "$IPOPT_LIBS -lipopt")
if test "$have_pkg_config" == "yes"; then
	dnl set CPPAD_IPOPT_LIBS
	PKG_CONFIG_PATH="$IPOPT_DIR/lib/pkgconfig:$IPOPT_DIR/share/pkgconfig"
	export PKG_CONFIG_PATH
	if pkg-config --libs ipopt >& /dev/null ; then
		CPPAD_IPOPT_LIBS=`pkg-config --libs ipopt`
	else
		dnl check for blas library
		AC_LANG_PUSH([Fortran])
		AC_CHECK_LIB(
			[blas],
			[ddot],
			[CPPAD_IPOPT_LIBS="$CPPAD_IPOPT_LIBS -lblas"],
		)
		dnl check for lapack library
		AC_CHECK_LIB(
			[lapack],
			[dgesv],
			[CPPAD_IPOPT_LIBS="$CPPAD_IPOPT_LIBS -llapack"],
		)
		AC_LANG_POP([Fortran])
	fi
else
	dnl check for blas library
	AC_LANG_PUSH([Fortran])
	AC_CHECK_LIB(
		[blas],
		[ddot],
		[CPPAD_IPOPT_LIBS="$CPPAD_IPOPT_LIBS -lblas"],
	)
	dnl check for lapack library
	AC_CHECK_LIB(
		[lapack],
		[dgesv],
		[CPPAD_IPOPT_LIBS="$CPPAD_IPOPT_LIBS -llapack"],
	)
	AC_LANG_POP([Fortran])
fi
AC_MSG_RESULT([cppad_ipopt_nlp libraries: $CPPAD_IPOPT_LIBS])
dnl -----------------------------------------------------------------------
dnl AC_CONFIG_FILES(file-list) for each file in the list, configure will 
dnl read file.in, do its substitutions, and create file
AC_CONFIG_FILES([ 
	cppad/configure.hpp
	doc.omh
	cppad_ipopt/example/test.sh
	cppad_ipopt/speed/test.sh
	cppad_ipopt/test/test.sh
	example/test_one.sh
	omh/install_unix.omh
	omh/install_windows.omh
	pkgconfig/cppad.pc
	pkgconfig/cppad-uninstalled.pc
	test_more/test_one.sh

	makefile
	example/makefile 
	cppad_ipopt/src/makefile
	cppad_ipopt/example/makefile
	cppad_ipopt/speed/makefile
	cppad_ipopt/test/makefile
	introduction/get_started/makefile 
	introduction/exp_apx/makefile
	multi_thread/makefile
	multi_thread/test_multi/makefile
	print_for/makefile
	speed/adolc/makefile
	speed/cppad/makefile 
	speed/double/makefile 
	speed/example/makefile
	speed/fadbad/makefile
	speed/profile/makefile
	speed/profile/gprof.sed
	speed/sacado/makefile
	speed/src/makefile
	test_more/makefile 
])
dnl END AC_CONFIG_FILES 
dnl the comment line above is used by ./new_stable.sh

AC_CONFIG_COMMANDS(
	[cppad_ipopt_example_test.sh],
	[chmod +x cppad_ipopt/example/test.sh],
)	[]
AC_CONFIG_COMMANDS(
	[cppad_ipopt_speed_test.sh],
	[chmod +x cppad_ipopt/speed/test.sh],
	[]
)
AC_CONFIG_COMMANDS(
	[cppad_ipopt_test_test.sh],
	[chmod +x cppad_ipopt/test/test.sh],
	[]
)
AC_CONFIG_COMMANDS(
	[example_test_one.sh],
	[chmod +x example/test_one.sh],
	[]
)
AC_CONFIG_COMMANDS(
	[test_more_test_one.sh],
	[chmod +x test_more/test_one.sh],
	[]
)

dnl create all the requested output files
AC_OUTPUT
