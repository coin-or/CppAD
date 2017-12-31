dnl --------------------------------------------------------------------------
dnl CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
dnl
dnl CppAD is distributed under multiple licenses. This distribution is under
dnl the terms of the
dnl                     Eclipse Public License Version 1.0.
dnl
dnl A copy of this license is included in the COPYING file of this distribution.
dnl Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
dnl ---------------------------------------------------------------------------
dnl
dnl Process this file with autoconf to produce a configure script.
dnl   package   version              bug-report
AC_INIT([cppad], [20180000.0], [cppad@list.coin-or.org])
AM_SILENT_RULES([yes])

dnl By defalut disable maintainer mode when running configure;
dnl --enable-maintainer-mode will enable it.
dnl This avoids the autoconf and automake tools being launched by make
AM_MAINTAINER_MODE

dnl Set the default install prefix to the installers home directory
dnl cygwin begin delete:
AC_PREFIX_DEFAULT(${HOME})
dnl cygwin end delete:
dnl cygwin replace: AC_PREFIX_DEFAULT(/usr)

dnl Run any macros required for proper operation of generated makefiles
dnl Note this must come before AC_PROG_CC; see
dnl http://lists.gnu.org/archive/html/automake/2010-08/msg00113.html
dnl
dnl nostdinc: Do not define standard include directories in generated makefiles
dnl subdir-objects: create objects in sub-directories, same structure source
dnl foreign: not GNU standard; no NEWS, AUTHORS, ...
AM_INIT_AUTOMAKE([nostdinc subdir-objects -Wall foreign])

# check if we have ar program
have_ar='yes'
AM_PROG_AR([have_ar='no'])
AC_MSG_NOTICE([have_ar = $have_ar])

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
AC_ARG_WITH(sparse_set,
	AC_HELP_STRING([--with-sparse_set], [no longer available]),
	[sparse_set="yes"],
	[sparse_set="no"]
)
if test "$sparse_set" = "yes" ; then
	AC_MSG_ERROR([--with-sparse_set is no longer available])
fi
AC_ARG_WITH(sparse_list,
	AC_HELP_STRING([--with-sparse_list], [no longer necessary]),
	[sparse_set="yes"],
	[sparse_set="no"]
)
if test "$sparse_set" = "yes" ; then
	AC_MSG_ERROR([--with-sparse_set is no longer necessary])
fi
AC_ARG_WITH(deprecated, AC_HELP_STRING(
	[--with-deprecated],
	[include deprectated features that are on the chopping block]
	),
	[deprecated="yes"],
	[deprecated="no"]
)
AM_CONDITIONAL(CppAD_DEPRECATED, test "$deprecated" = 'yes' )
if test "$deprecated" = "yes" ; then
	AC_MSG_ERROR(
		[autotools version does not install features on the chopping block]
	)
fi
dnl --------------------------------------------------------------------------
AC_MSG_NOTICE([******  Check command line arguments that are used ***********])

dnl -----------------------------------------------------------------------
dnl The case where enable_msvc != no is defined and supported by Ted Ralphs
AC_MSG_CHECKING([--enable-msvc])
AC_ARG_ENABLE([msvc],
  [AC_HELP_STRING([--enable-msvc],
                  [Prefer (i)cl/ifort/link over GNU on MinGW/Cygwin.])],
  [enable_msvc=$enableval],
  [enable_msvc=no]
)
AC_MSG_RESULT([$enable_msvc])


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
	AC_SUBST(cppad_stdvector, 1)
else
	AC_SUBST(cppad_stdvector, 0)
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
if test "$boostvector" = "yes" ; then
	if test "$stdvector" != "no" ; then
		AC_MSG_ERROR([cannot --with both stdvector and boostvector])
	fi
	AC_SUBST(cppad_boostvector, 1)
else
	AC_SUBST(cppad_boostvector, 0)
fi

dnl Are we using eigen vectors ----------------------------------------
AC_MSG_CHECKING([--with-eigenvector])
AC_ARG_WITH(eigenvector,
	AC_HELP_STRING([--with-eigenvector], [default is no]),
	[eigenvector="yes"],
	[eigenvector="no"]
)
AC_MSG_RESULT([$eigenvector])
if test "$eigenvector" = "yes" ; then
	if test "$stdvector" != "no" ; then
		AC_MSG_ERROR([cannot --with both stdvector and eigenvector])
	fi
	if test "$boostvector" != "no" ; then
		AC_MSG_ERROR([cannot --with both boostvector and eigenvector])
	fi
	AC_SUBST(cppad_eigenvector, 1)
else
	AC_SUBST(cppad_eigenvector, 0)
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
	AC_SUBST(cppad_has_adolc, 1)
else
	AC_MSG_RESULT([no])
	AC_SUBST(cppad_has_adolc, 0)
fi

dnl The prefix corresponding to the installation of Eigen ---------------
AC_MSG_CHECKING([EIGEN_DIR])
AC_ARG_VAR(EIGEN_DIR,
	[value of configure prefix directory during Eigen install]
)
AM_CONDITIONAL(CppAD_EIGEN_DIR, test "$EIGEN_DIR" != "")
if test "$EIGEN_DIR" != "" ; then
	AC_MSG_RESULT([$EIGEN_DIR])
	AC_SUBST(EIGEN_INCLUDE, [-I$EIGEN_DIR/include])
	AC_SUBST(cppad_has_eigen, 1)
else
	AC_MSG_RESULT([no])
	AC_SUBST(EIGEN_INCLUDE, [])
	AC_SUBST(cppad_has_eigen, 0)
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
	AC_SUBST(cppad_has_fadbad, 1)
else
	AC_MSG_RESULT([no])
	AC_SUBST(cppad_has_fadbad, 0)
fi

dnl The directory corresponding to the installation of SACADO -----------
AC_MSG_CHECKING([SACADO_DIR])
AC_ARG_VAR(SACADO_DIR,
	[value of the configure prefix directory during Sacado install]
)
AM_CONDITIONAL(CppAD_SACADO, test "$SACADO_DIR" != "")
if test "$SACADO_DIR" != "" ; then
	AC_MSG_ERROR([CppAD autotools build no longer support sacado, use cmake])
else
	AC_MSG_RESULT([no])
	AC_SUBST(cppad_has_sacado, 0)
fi

dnl The directory corresponding to the installation of IPOPT -----------
AC_MSG_CHECKING([IPOPT_DIR])
AC_ARG_VAR(IPOPT_DIR,
	[value of the configure prefix directory during Ipopt install]
)
AM_CONDITIONAL(CppAD_IPOPT, test "$IPOPT_DIR" != "")
if test "$IPOPT_DIR" != "" ; then
	AC_MSG_RESULT([$IPOPT_DIR])
	if test "$have_ar" = "no"; then
		AC_MSG_ERROR([ar is missing and IPOPT_DIR is present])
	fi
	AC_SUBST(cppad_has_ipopt, 1)
else
	AC_MSG_RESULT([no])
	AC_SUBST(cppad_has_ipopt, 0)
fi

dnl The directory corresponding to the installation of BOOST ------------
AC_MSG_CHECKING([BOOST_DIR])
AC_ARG_VAR(BOOST_DIR,
	[parent directory directly above boost directory]
)
AM_CONDITIONAL(CppAD_BOOST_DIR, test "$BOOST_DIR" != "")
if test "$BOOST_DIR" != "" ; then
	AC_MSG_RESULT([$BOOST_DIR])
	AC_SUBST(BOOST_INCLUDE, [-I$BOOST_DIR/include])
	AC_SUBST(cppad_has_boost, 1)
else
	AC_MSG_RESULT([no])
	AC_SUBST(BOOST_INCLUDE,[])
	AC_SUBST(cppad_has_boost, 0)
fi

dnl The C++ compiler flags CXX_FLAGS -------------------------------------
dnl see AC_SUBST(CXXFLAGS, "") next to AC_PROG_CXX
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
AC_SUBST(cppad_max_num_threads, [$max_num_threads])

dnl type used for addresses in AD tapes [ default = unsigned int ] -----------
AC_MSG_CHECKING([TAPE_ADDR_TYPE])
AC_ARG_VAR(TAPE_ADDR_TYPE, [type used for addresses in AD tapes])
if test "$TAPE_ADDR_TYPE" == "" ; then
	addr_t="unsigned int"
else
	addr_t="$TAPE_ADDR_TYPE"
fi
AC_SUBST(cppad_tape_addr_type, [$addr_t])
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

dnl type used for tape identifier [ default = unsigned int ] -----------
AC_MSG_CHECKING([TAPE_ID_TYPE])
AC_ARG_VAR(TAPE_ID_TYPE, [type used for addresses in AD tapes])
if test "$TAPE_ID_TYPE" == "" ; then
	tape_id_t="unsigned int"
else
	tape_id_t="$TAPE_ID_TYPE"
fi
AC_SUBST(cppad_tape_id_type, [$tape_id_t])
AC_MSG_RESULT([$tape_id_t])
ok="no"
if test "$tape_id_t" == "size_t" ; then
	ok="yes"
fi
if test "$tape_id_t" == "int" ; then
	AC_MSG_WARN(
		[TAPE_ID_TYPE=int is for CppAD developers (not users)]
	)
	ok="yes"
fi
if test "$tape_id_t" == "unsigned int" ; then
	ok="yes"
fi
if test "$tape_id_t" == "unsigned short int" ; then
	ok="yes"
fi
if test "$ok" == "no" ; then
	AC_MSG_ERROR(
		[$tape_id_t is not a valid choice for TAPE_ID_TYPE]
	)
fi
AC_MSG_NOTICE([********** End Checking Command Line arguments ***************])

dnl --------------------------------------------------------------------------
if test "$enable_msvc" == "no"
then
	cxx_list="g++ clang++ CC pgCC icpc gpp cxx cc++"
	cc_list="gcc clang cc pgcc icc"
	fc_list="gfortran ifort g95 fort77 f77 g77 pgf90 pgf77 ifc frt af77"
else
	dnl This case defined and supported by Ted Ralphs
	cxx_list="icl cl g++"
	cc_list="icl cl gcc"
	fc_list="ifort gfortran"
fi
dnl --------------------------------------------------------------------------

dnl AC_PROG_CXX([compiler-search-list]) outputs CXX as C++ compiler to use
AC_PROG_CXX([$cxx_list])
dnl erase any setting of CXXFLAGS, see CXX_FLAGS documentation
AC_SUBST(CXXFLAGS, "")

dnl AC_PROG_CC([compiler-search-list]) outputs CC as C++ compiler to use
AC_PROG_CC([$cc_list])
dnl erase any setting of CFLAGS
AC_SUBST(CFLAGS, "")

dnl AC_PROG_FC ([compiler-search-list], [dialect])
AC_PROG_FC([$fc_list])
dnl not compiling any fortran so no need to touch flags

dnl Check for pkgconfig
AC_CHECK_PROG(have_pkg_config, [pkg-config], [yes], [no])
AM_CONDITIONAL(CppAD_PKG_CONFIG, test "$have_pkg_config" = "yes")

dnl check if we can build libraries
AM_CONDITIONAL(CppAD_HAVE_AR, test "$have_ar" = 'yes' )
if test "$have_ar" = 'no'; then
	AC_MSG_WARN(
	[cannot build any libraries because ar is missing; e.g,, speed/* not built]
	)
fi

dnl test_one.sh.in settings for cppad_cxx_flags (no longer used ?)
AC_SUBST(cppad_cxx_flags, "-g -O0")

dnl autotools version does not support installing deprecated features
AC_SUBST(cppad_deprecated_01,  0)

dnl autotools version does not colpack
AC_SUBST(cppad_has_colpack, 0)

dnl autotools version will assumes mkstemp is available.
AC_SUBST(cppad_has_mkstemp, 1)
AC_SUBST(cppad_has_tmpnam_s, 0)

dnl autotools version cannot use c++11 compiler
dnl so supress corresponding features
AC_SUBST(cppad_cplusplus_201100_ok, 0)

dnl autotools version does not check if using g++
AC_SUBST(cppad_compiler_is_gnucxx, 0)

dnl autotools version is slower because it does not specialize is_pod
dnl for the  Fundamental types
AC_SUBST(is_pod_specialize_98,
	"template <> inline bool is_pod<size_t>(void) { return true; }"
)
AC_SUBST(is_pod_specialize_11, "")

dnl Determine which definition we are using for CPPAD_TESTVECTOR
if test "$stdvector$boostvector$eigenvector" = "nonono" ; then
	AC_SUBST(cppad_cppadvector, 1)
else
	AC_SUBST(cppad_cppadvector, 0)
fi

dnl the gettimeofday check automatically generates a yes / no configure msg
AC_CHECK_FUNC(
	gettimeofday,
	[gettimeofday="yes"],
	[gettimeofday="no"]
)
if test "$gettimeofday" = "yes" ; then
	AC_SUBST(cppad_has_gettimeofday, 1)
else
	AC_SUBST(cppad_has_gettimeofday, 0)
fi

dnl Use LIBTOOL for cppad_ipopt ?
dnl AC_PROG_LIBTOOL

dnl Determine if ranlib is present and where it is if present (set RANLIB)
AC_PROG_RANLIB

AC_MSG_CHECKING([whether using Microsoft C++ compiler])
AM_CONDITIONAL(CppAD_MS_COMPILER, test "$CXX" == "cl" || test "$CXX" == "icl")
if test "$CXX" == "cl" || test "$CXX" == "icl"; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi



dnl Have configure check source directory to see if cppad/cppad.hpp is there
AC_CONFIG_SRCDIR(cppad/cppad.hpp)

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
	PKG_CONFIG_PATH="$PKG_CONFIG_PATH:$IPOPT_DIR/lib64/pkgconfig"
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
dnl --------------------------------------------------------------------------
dnl Names set here so same cppad.pc.in works for both with cmake and autoconf.
dnl (note that autoconf configuration does not allow for Colpack).
dnl
AC_SUBST(cppad_description, "Differentiation of C++ Algorithms")
AC_SUBST(cppad_version, ${PACKAGE_VERSION} )
AC_SUBST(cppad_url, "http://www.coin-or.org/CppAD")
if test "$prefix" == "NONE" ; then
	cppad_pkgconfig_cflags_value="-I$HOME/include"
	cppad_pkgconfig_libs_value="-L$HOME/lib"
else
	cppad_pkgconfig_cflags_value="-I$prefix/include"
	cppad_pkgconfig_libs_value="-L$prefix/lib"
fi
if test "$POSTFIX_DIR" != "" ; then
	cppad_pkgconfig_cflags_value="$cppad_pkgconfig_cflags_value/$POSTFIX_DIR"
	cppad_pkgconfig_libs_value="$cppad_pkgconfig_libs_value/$POSTFIX_DIR"
fi
if test "$IPOPT_DIR" == "" ; then
	cppad_pkgconfig_libs_value=""
	cppad_pkgconfig_requires_value=""
else
	cppad_pkgconfig_libs_value="$cppad_pkgconfig_libs_value -lcppad_ipopt"
	cppad_pkgconfig_requires_value="ipopt"
fi
AC_SUBST(cppad_pkgconfig_cflags,   "$cppad_pkgconfig_cflags_value")
AC_SUBST(cppad_pkgconfig_libs,     "$cppad_pkgconfig_libs_value")
AC_SUBST(cppad_pkgconfig_requires, "$cppad_pkgconfig_requires_value")
AC_SUBST(cppad_prefix,             "$prefix")
AC_SUBST(cppad_abs_includedir,     "$includedir")
dnl
dnl names set here so cppad.pc-uninstalled works both with cmake and autoconf
AC_SUBST(cppad_SOURCE_DIR,  ${ABS_TOP_SRCDIR} )
dnl
dnl names set here so */test_one.sh.in works both with cmake and autoconf
dnl (no longer used ?)
AC_SUBST(adolc_prefix,      ${ADOLC_DIR} )
AC_SUBST(eigen_prefix,      ${EIGEN_DIR} )
AC_SUBST(ipopt_prefix,      ${IPOPT_DIR} )
dnl
dnl -----------------------------------------------------------------------
dnl AC_CONFIG_FILES(file-list) for each file in the list, configure will
dnl read file.in, do its substitutions, and create file
AC_CONFIG_FILES([
	cppad/configure.hpp
	cppad/local/is_pod.hpp
	cppad_ipopt/example/test.sh
	cppad_ipopt/speed/test.sh
	cppad_ipopt/test/test.sh
	example/ipopt_solve/test.sh
	pkgconfig/cppad.pc
	pkgconfig/cppad-uninstalled.pc

	makefile
	cppad_ipopt/src/makefile
	cppad_ipopt/example/makefile
	cppad_ipopt/speed/makefile
	cppad_ipopt/test/makefile
	example/general/makefile
	example/abs_normal/makefile
	example/atomic/makefile
	example/deprecated/makefile
	example/ipopt_solve/makefile
	example/optimize/makefile
	example/sparse/makefile
	example/utility/makefile
	example/get_started/makefile
	introduction/makefile
	example/multi_thread/makefile
	example/print_for/makefile
	speed/adolc/makefile
	speed/cppad/makefile
	speed/double/makefile
	speed/example/makefile
	speed/fadbad/makefile
	speed/profile/makefile
	speed/profile/gprof.sed
	speed/sacado/makefile
	speed/src/makefile
	test_more/compare_c/makefile
	test_more/general/makefile
])
dnl END AC_CONFIG_FILES

dnl scripts that set library path before running test
AC_CONFIG_COMMANDS(
	[example_ipopt_solve_test.sh],
	[chmod +x example/ipopt_solve/test.sh],
)	[]
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

dnl create all the requested output files
AC_OUTPUT
