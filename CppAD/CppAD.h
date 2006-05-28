# ifndef CppADCppADIncluded
# define CppADCppADIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation Copyright (C) 2003-06 Bradley M. Bell

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

// ---------------------------------------------------------------------------
// CppAD general purpose library routines (can be included separately)

# include <CppAD/PowInt.h>
# include <CppAD/LuSolve.h>
# include <CppAD/NearEqual.h>
# include <CppAD/Runge45.h>
# include <CppAD/RombergOne.h>
# include <CppAD/RombergMul.h>
# include <CppAD/Rosen34.h>
# include <CppAD/OdeErrControl.h>
# include <CppAD/OdeGear.h>
# include <CppAD/OdeGearControl.h>
# include <CppAD/Poly.h>
# include <CppAD/SpeedTest.h>
# include <CppAD/CppAD_vector.h>

# include <CppAD/CheckSimpleVector.h>
# include <CppAD/CheckNumericType.h>
# include <CppAD/TrackNewDel.h>
# include <CppAD/local/CppADError.h>
// --------------------------------------------------------------------------
// System routines that can be used by rest of CppAD with out including 

# include <cstddef>
# include <iostream>
# include <complex>
# include <cmath>

// ---------------------------------------------------------------------------
// definitions needed by rest of includes

// definitions that come from the installation
# include <CppAD/config.h>

// definitions that are local to the CppAD include files
# include <CppAD/local/Define.h>

// vectors used with CppAD
# include <CppAD/local/CppADvector.h>

// Declare classes and fucntions that are used before defined
# include <CppAD/local/Declare.h>

// ---------------------------------------------------------------------------
// files containing only implementation details

# include <CppAD/local/AD.h>       // the differentaible object class

// ---------------------------------------------------------------------------
// files containing user interface documentation

# include <CppAD/local/Op.h>      // executes taped operations
# include <CppAD/local/UserAD.h>  // AD class methods available to the user
# include <CppAD/local/ADFun.h>   // ADFun objects

// ---------------------------------------------------------------------------
// library routines that require the rest of CppAD
# include <CppAD/local/LuRatio.h>
# include <CppAD/local/BenderQuad.h>

// undo definitions in Define.h
# include <CppAD/local/Undef.h>   

# endif
