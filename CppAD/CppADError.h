# ifndef CppADErrorIncluded
# define CppADErrorIncluded

// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT

/*
-------------------------------------------------------------------------------
$begin CppADError$$
$spell
	cppad
	yy
	mm
	dd
	iostream
	config
	ifndef
	namespace
	Cpp
	exp
	bool
	ifdef
	std
	cerr
	std
	endl
	endif
$$

$index CppADError$$
$index error, assert macro$$
$index assert, error macro$$
$index macro, error assert$$

$section CppAD Error Detection and Reporting$$

$table
$bold Syntax$$
$cnext $code # include <CppAD/CppADError.h>$$  $rnext
$cnext $syntax%CppADUnknownError(%exp%)%$$             $rnext
$cnext $syntax%CppADUsageError(%exp%, %text%)%$$ 
$tend

$fend 25$$

$head Description$$
The macros defined by the file $code CppAD/CppADError.h$$
are used to support two types of error detection and reporting:

$subhead CppADUsageError$$
The first error type occurs when a routine in the $code CppAD$$ namespace
detects a violation of it's specifications. 
In this case the error is thought of as an error using CppAD 
and the macro $code CppADUsageError$$ is invoked.

$subhead CppADUnknownError$$
The second type occurs when a routine in the $code CppAD$$ namespace
detects an error but does not know the cause of the error.
In this case the error is thought of as coming from an unknown source
and the macro $code CppADUnknownError$$ is invoked.

$head Include$$
The file $code CppAD/CppADError.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.

$head Exp$$
As an argument to either 
$code CppADUsageError$$ or $code CppADUnknownError$$, 
the expression specified by $italic exp$$
must result in a $code bool$$ value.
The resulting value should be true. 
If it is false, an error has occurred.
This expression may be execute any number of times (including zero times)
so it must have not side effects.

$head Text$$
As an argument to $code CppADUsageError$$,
the text specified by $italic text$$ is a $code '\0'$$ terminated
character string that contains a description of the error
in the case where $italic exp$$ is false.

$head Replacing Defaults$$
You can define you own version of 
$code CppADUsageError$$ or $code CppADUnknownError$$ 
provided that you meet the corresponding specifications
given above.
In addition, you must included your definition before including $code CppAD.h$$
or any of the other CppAD header files.
This will ensure that  your definition for 
$code CppADUsageError$$ or $code CppADUnknownError$$ 
will prevent the corresponding default definition 
from being interpreted by the preprocessor 
(see default definitions extracted from $code CppAD/CppADError.h$$ below).

$subhead CppADUsageError$$
Below is the default CppAD usage error handler.
The preprocessor symbol $code PACKAGE_STRING$$
is equal to $syntax%cppad-%yy%-%mm%-%dd%$$
where $syntax%%yy%-%mm%-%dd%$$ is the year, month, and date
corresponding to this version of CppAD.
$codep */

# include <assert.h>
# include <iostream>
# include <CppAD/config.h>

# ifndef CppADUsageError
# ifdef NDEBUG
# define CppADUsageError(exp, text)  // do nothing
# else
# define CppADUsageError(exp, text)                       \
    {    if( ! (exp) )                                    \
             std::cerr << PACKAGE_STRING                  \
                       << " usage error"                  \
                       << ":\n" << text << std::endl;     \
             assert(exp);                                 \
    }
# endif
# endif

/* $$

$subhead CppADUnknownError$$
Below is the default CppAD handler for errors from an unknown source:
$codep */

# ifndef CppADUnknownError
# ifdef NDEBUG
# define CppADUnknownError(exp)        // do nothing
# else
# define CppADUnknownError(exp)                              \
    {    if( ! (exp) )                                       \
             std::cerr << PACKAGE_STRING                     \
                       << " error from an unknown source"    \
                       << ":\n" << std::endl;                \
             assert(exp);                                    \
    }
# endif
# endif

/* $$


$end
-------------------------------------------------------------------------------
*/

# endif
