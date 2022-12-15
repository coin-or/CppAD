// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin code_gen_fun}
{xrst_spell
   cg
   jrcv
}

Generate Source Code and Compile an AD Function
###############################################

Syntax
******

   # ``include <cppad/example/code_gen_fun.hpp>``

Constructors
============

| ``code_gen_fun`` *fun_name* ()
| ``code_gen_fun`` *fun_name* ( *file_name* )
| ``code_gen_fun`` *fun_name* ( *file_name* , *cg_fun* )
| ``code_gen_fun`` *fun_name* ( *file_name* , *cg_fun* , *eval_jac* )

swap
====
*fun_name* . ``swap`` ( *other_fun* )

function
========
*y* = *fun_name* ( *x* )

jacobian
========
*J* = *fun_name* . ``jacobian`` ( *x* )

sparse_jacobian
===============
*Jrcv* = *fun_name* . ``sparse_jacobian`` ( *x* )

Prototype
*********

Constructors
============
{xrst_literal
   // BEGIN_CTOR_VOID
   // END_CTOR_VOID
}
{xrst_literal
   // BEGIN_CTOR_FILE_NAME
   // END_CTOR_FILE_NAME
}
{xrst_literal
   // BEGIN_CTOR_CG_FUN
   // END_CTOR_CG_FUN
}

Operations
==========
{xrst_literal
   // BEGIN_SWAP_OTHER_FUN
   // END_SWAP_OTHER_FUN
}
{xrst_literal
   // BEGIN_FUN_NAME_X
   // END_FUN_NAME_X
}
{xrst_literal
   // BEGIN_JACOBIAN
   // END_JACOBIAN
}
{xrst_literal
   // BEGIN_SPARSE_JACOBIAN
   // END_SPARSE_JACOBIAN
}

CppAD::cg::CG<double>
*********************
This is the CppAD *Base* type for the function
*cg_fun* .
It is defined by
`CppADCodeGen <https://github.com/joaoleal/CppADCodeGen>`_.
and used to convert the *cg_fun* function object to source code,
compile the source code, and then link the corresponding function evaluation

   *y* = ``cg_fun.Forward`` (0, *x* )

Speed
*****
The conversion to source and linking is expected to take a significant
amount of time and the evaluation of the function is expected to be
much faster; see the following speed tests:

.. csv-table::
   :widths: auto

   cppadcg_det_minor.cpp,:ref:`cppadcg_det_minor.cpp-title`
   cppadcg_sparse_jacobian.cpp,:ref:`cppadcg_sparse_jacobian.cpp-title`

fun_name
********
This is the name of the ``code_gen_fun`` object.

other_fun
*********
This is the name of another ``code_gen_fun`` object.

file_name
*********
This is the absolute or relative path for the
file that contains the dynamic library.
It does not include the files extension at the end that is used
for dynamic libraries on this system.
If *cg_fun* is not present in the constructor,
it must have been present in a previous constructor with the same
*file_name* .

cg_fun
******
This is a CppAD function object that corresponds to a function
:math:`f : \B{R}^n \rightarrow \B{R}^m`.
If this arguments is present in the constructor,
a new dynamic library is created.

eval_jac
********
If this argument is present in the constructor,
it determines which type of Jacobian :math:`f'(x)` will be enabled.
The possible choices for *eval_jac* are:

.. csv-table::
   :widths: auto

   *eval_jac*,Available Jacobian
   ``code_gen_fun::none_enum``,none
   ``code_gen_fun::dense_enum``,*fun_name* . ``jacobian``

The default value for *eval_jac* is none.

swap
****
This exchanges the library in *fun_name* with the library in
*other_fun* .

x
*
is a vector of size *n* specifying the argument value
at which the function will be evaluated.

y
*
This return value has size *m* and is the value of :math:`f(x)`.

jacobian
********

J
=
This return value has size *m* * *n* and is the value of
the Jacobian :math:`f'(x)` where

.. math::

   J[ i \cdot n + j ] =  ( \partial f_i / \partial x_j )  (x)

Speed
=====
The speed test :ref:`cppadcg_det_minor.cpp-name` has the option to pass
the determinant function, or the Jacobian of the determinant function,
to CppADCodeGen (for the same eventual calculation); see
:ref:`cppadcg_det_minor.cpp@PASS_JACOBIAN_TO_CODE_GEN` .
This test indicates that both methods have similar setup
and derivative calculation times.

sparse_jacobian
***************

Jrcv
****
This return value is a :ref:`sparse_rcv-name` sparse matrix representation
of the Jacobian.

Speed
=====
The speed test :ref:`cppadcg_sparse_jacobian.cpp-name` has the option to pass
a function (:ref:`sparse_jac_fun-name` ) or it's Jacobian to CppADCodeGen
(for the same eventual calculation); see
:ref:`cppadcg_sparse_jacobian.cpp@PASS_SPARSE_JACOBIAN_TO_CODE_GEN` .
THis test indicates that both methods have similar setup
and derivative calculation times.
{xrst_toc_hidden
   example/code_gen_fun/function.cpp
   example/code_gen_fun/file.cpp
   example/code_gen_fun/jacobian.cpp
   example/code_gen_fun/jac_as_fun.cpp
   example/code_gen_fun/sparse_jacobian.cpp
   example/code_gen_fun/sparse_jac_as_fun.cpp
}
Examples
********

.. csv-table::
   :widths: auto

   code_gen_fun_function.cpp,:ref:`code_gen_fun_function.cpp-title`
   code_gen_fun_file.cpp,:ref:`code_gen_fun_file.cpp-title`
   code_gen_fun_jacobian.cpp,:ref:`code_gen_fun_jacobian.cpp-title`
   code_gen_fun_jac_as_fun.cpp,:ref:`code_gen_fun_jac_as_fun.cpp-title`
   code_gen_fun_sparse_jacobian.cpp,:ref:`code_gen_fun_sparse_jacobian.cpp-title`
   code_gen_fun_sparse_jac_as_fun.cpp,:ref:`code_gen_fun_sparse_jac_as_fun.cpp-title`

Implementation
**************
see :ref:`code_gen_fun.hpp-name` and :ref:`code_gen_fun.cpp-name`

{xrst_end code_gen_fun}
-----------------------------------------------------------------------------
{xrst_begin code_gen_fun.hpp}

code_gen_fun Class Include File
###############################

See Also
********
:ref:`code_gen_fun-name` , :ref:`code_gen_fun.cpp-name`

Source
******
{xrst_literal
   include/cppad/example/code_gen_fun.hpp
   // BEGIN C++
   // END C++
}

{xrst_end code_gen_fun.hpp}
-----------------------------------------------------------------------------
{xrst_begin code_gen_fun.cpp}

code_gen_fun Class Member Implementation
########################################

See Also
********
:ref:`code_gen_fun-name` , :ref:`code_gen_fun.hpp-name`

Source
******
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end code_gen_fun.cpp}
*/
// BEGIN_C++
# include <cppad/example/code_gen_fun.hpp>

// ---------------------------------------------------------------------------
// code_gen_fun fun_name(file_name, cg_name, eval_jac)
// ---------------------------------------------------------------------------
// BEGIN_CTOR_CG_FUN
code_gen_fun::code_gen_fun(
   const std::string&                     file_name  ,
   CppAD::ADFun< CppAD::cg::CG<double> >& cg_fun     ,
   evaluation_enum                        eval_jac   )
// END_CTOR_CG_FUN
{  // Generate source code
   CppAD::cg::ModelCSourceGen<double> cgen(cg_fun, "model");
   switch(eval_jac)
   {  case none_enum:
      break;

      case dense_enum:
      cgen.setCreateJacobian(true);
      break;

      case sparse_enum:
      cgen.setCreateSparseJacobian(true);
      break;
   }
   CppAD::cg::ModelLibraryCSourceGen<double> libcgen(cgen);

   // Compile source, create the library file, and load the library
   CppAD::cg::DynamicModelLibraryProcessor<double> proc(libcgen, file_name);
   CppAD::cg::ClangCompiler<double> compiler;
   bool loadLib = true;
   dynamic_lib_ = proc.createDynamicLibrary(compiler, loadLib);
   //
   // create the model object
   model_        = dynamic_lib_->model("model");
}
// ---------------------------------------------------------------------------
// code_gen_fun fun_name(file_name)
// ---------------------------------------------------------------------------
// BEGIN_CTOR_FILE_NAME
code_gen_fun::code_gen_fun(const std::string&  file_name )
// END_CTOR_FILE_NAME
{  // file name plus extension used for dynamic libraries on this system
   std::string file_name_ext = file_name +
      CppAD::cg::system::SystemInfo<>::DYNAMIC_LIB_EXTENSION;

   // load the library
   CppAD::cg::DynamicLib<double>* ptr =
      new CppAD::cg::LinuxDynamicLib<double>(file_name_ext);
   dynamic_lib_  = std::unique_ptr< CppAD::cg::DynamicLib<double> >(ptr);
   //
   // create the model object
   model_        = dynamic_lib_->model("model");
}
// ---------------------------------------------------------------------------
// code_gen_fun fun_name
// ---------------------------------------------------------------------------
// BEGIN_CTOR_VOID
code_gen_fun::code_gen_fun(void)
// END_CTOR_VOID
{ }
// --------------------------------------------------------------------------
// fun_name.swap(other_fun)
// --------------------------------------------------------------------------
// BEGIN_SWAP_OTHER_FUN
void code_gen_fun::swap(code_gen_fun& other_fun)
// END_SWAP_OTHER_FUN
{  std::swap(dynamic_lib_, other_fun.dynamic_lib_);
   std::swap(model_, other_fun.model_ );
}
// --------------------------------------------------------------------------
// y = fun_name(x)
// --------------------------------------------------------------------------
// BEGIN_FUN_NAME_X
CppAD::vector<double>
code_gen_fun::operator()(const CppAD::vector<double>& x)
// END_FUN_NAME_X
{  return model_->ForwardZero(x);
}
// --------------------------------------------------------------------------
// J = fun_name.jacobian(x)
// --------------------------------------------------------------------------
// BEGIN_JACOBIAN
CppAD::vector<double>
code_gen_fun::jacobian(const CppAD::vector<double>& x)
// END_JACOBIAN
{  CPPAD_ASSERT_KNOWN( model_->isJacobianAvailable() ,
      "code_gen_fun: dense jacobian not enables during constructor"
   );
   return model_-> Jacobian(x);
}
// --------------------------------------------------------------------------
// Jrcv = fun_name.sparse_jacobian(x)
// --------------------------------------------------------------------------
// BEGIN_SPARSE_JACOBIAN
CppAD::sparse_rcv< CppAD::vector<size_t>, CppAD::vector<double> >
code_gen_fun::sparse_jacobian(const CppAD::vector<double>& x)
// END_SPARSE_JACOBIAN
{  CPPAD_ASSERT_KNOWN( model_->isSparseJacobianAvailable() ,
      "code_gen_fun: sparse jacobian not enabled during constructor"
   );
   // x_std
   size_t n = model_->Domain();
   std::vector<double> x_std(n);
   for(size_t j = 0; j < n; ++j)
      x_std[j] = x[j];
   //
   // 2DO: Prepahs CppAD should have a sparse_rcv constructor (jac, row, col)
   // that uses swap to swap the vectors
   //
   // jac, row, col
   std::vector<double> jac;
   std::vector<size_t> row, col;
   model_-> SparseJacobian(x_std, jac, row, col);
   //
   // sparse_rc
   size_t nr  = model_->Range();
   size_t nc  = model_->Domain();
   size_t nnz = row.size();
   CppAD::sparse_rc< CppAD::vector<size_t> > pattern(nr, nc, nnz);
   for(size_t k = 0; k < nnz; ++k)
      pattern.set(k, row[k], col[k]);
   // sparse_rcv
   CppAD::sparse_rcv< CppAD::vector<size_t>, CppAD::vector<double> >
   Jrcv(pattern);
   for(size_t k = 0; k < nnz; ++k)
      Jrcv.set(k, jac[k]);
   //
   return Jrcv;
}
// END_C++
