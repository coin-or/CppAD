// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// -------------------------------------------------------------
# include <cassert>
# include <cstddef>
# include <limits>
# include <cppad/utility/vector.hpp>

// addr_type
typedef size_t addr_t;

// Vector
template <class Base> using Vector = CppAD::vector<Base>;


// op_t
template <class Base>
class op_t {
public:
   typedef void (*op_fun_t)(
      addr_t  arg_index               ,
      addr_t  res_index               ,
      addr_t& n_arg                   ,
      addr_t& n_res                   ,
      const Vector<addr_t>& arg_vec   ,
      Vector<Base>&         value_vec
   );
   //
   addr_t   arg_index_; // index of first argument in arg_vec
   addr_t   res_index_; // index of first result in value_vec
   op_fun_t op_fun_;    // funciton attached to this opertor
   //
   // ctor(void)
   op_t(void)
   {  arg_index_ = std::numeric_limits<addr_t>::max();
      res_index_ = std::numeric_limits<addr_t>::max();
      op_fun_    = nullptr;
   }
   //
   // ctor(arg_index, res_index, op_fun)
   op_t(addr_t arg_index, addr_t res_index, op_fun_t op_fun) :
   arg_index_( arg_index ),
   res_index_( res_index ),
   op_fun_( op_fun )
   { }
   //
   // arg_index
   addr_t arg_index() const
   {  return arg_index_; }
   //
   // res_index
   addr_t res_index() const
   {  return res_index_; }
   //
   // eval
   void eval(
      const Vector<addr_t>& arg_vec      ,
      Vector<Base>&         value_vec    ) const
   {  assert( arg_vec.size() > 0 );
      assert( value_vec.size() > 0 );
      addr_t n_arg, n_res;
      op_fun_(
         arg_index_,  res_index_,
         n_arg,       n_res,
         arg_vec,     value_vec
      );
   }
};

// add_op_t
template <class Base>
class add_op_t {
public:
   // op_fun
   static void op_fun(
      addr_t                arg_index    ,
      addr_t                res_index    ,
      addr_t&               n_arg        ,
      addr_t&               n_res        ,
      const Vector<addr_t>& arg_vec      ,
      Vector<Base>&         value_vec    )
   {  n_arg = 2;
      n_res = 1;
      if( arg_vec.size() == 0 )
         return;
      const Base& left  = value_vec[ arg_vec[0] ];
      const Base& right = value_vec[ arg_vec[1] ];
      value_vec[res_index]  = left + right;
   }
};

// sub_op_t
template <class Base>
class sub_op_t {
public:
   // op_fun
   static void op_fun(
      addr_t                arg_index    ,
      addr_t                res_index    ,
      addr_t&               n_arg        ,
      addr_t&               n_res        ,
      const Vector<addr_t>& arg_vec      ,
      Vector<Base>&         value_vec    )
   {  n_arg = 2;
      n_res = 1;
      if( arg_vec.size() == 0 )
         return;
      const Base& left  = value_vec[ arg_vec[0] ];
      const Base& right = value_vec[ arg_vec[1] ];
      value_vec[res_index]  = left - right;
   }
};


// tape_t
template <class Base>
class tape_t {
private :
   typedef typename op_t<Base>::op_fun_t op_fun_t;
   //
   addr_t               n_ind_;     // number of independent values
   addr_t               res_index_; // index in value_vec of next result
   Vector<addr_t>       arg_vec_;   // index of operator arguments in value_vec
   Vector< op_t<Base> > op_vec_;    // operators that define this function
   Vector<addr_t>       not_used1_;
   Vector<Base>         not_used2_;
public :
   // set_ind
   void set_ind(addr_t n_ind)
   {  n_ind_     = n_ind;
      res_index_ = n_ind;
      op_vec_.resize(0);
   }
   //
   // next_op
   addr_t next_op(op_fun_t op_fun, const Vector<addr_t>& op_arg)
   {  //
      // res_index
      addr_t res_index = res_index_;
      //
      // op
      addr_t arg_index = arg_vec_.size();
      op_t op(arg_index, res_index, op_fun);
      //
      // n_arg, n_res
      addr_t n_arg, n_res;
      op_fun(arg_index, res_index, n_arg, n_res, not_used1_, not_used2_);
      assert( op_arg.size() == n_arg );
      //
      // op_vec_
      op_vec_.push_back(op);
      //
      // arg_vec_
      for(size_t i = 0; i < n_arg; ++i)
         arg_vec_.push_back( op_arg[i] );
      //
      // res_index_
      res_index_ = res_index + n_res;
      //
      return res_index;
   }
   //
   // n_res
   addr_t n_res(void) const
   {  return res_index_; }
   //
   // eval
   void eval(Vector<Base>& value_vec)
   {  assert( value_vec.size() == res_index_ );
      addr_t n_op = op_vec_.size();
      for(addr_t i = 0; i < n_op; ++i)
      {  op_vec_[i].eval(arg_vec_, value_vec);
      }
   }
};

int main()
{  bool ok = true;
   //
   // f_0 (x) = x[0] + x[1]
   // f_1 (x) = x[0] - x[1]
   //
   // f
   tape_t<double> tape;
   addr_t n_ind = 2;
   tape.set_ind(n_ind);
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   op_arg[0] = 0; // x[0]
   op_arg[1] = 1; // x[1]
   //
   // res_arg
   Vector<addr_t> res_index(2);
   //
   // tape
   res_index[0] = tape.next_op(add_op_t<double>::op_fun, op_arg);
   res_index[1] = tape.next_op(sub_op_t<double>::op_fun, op_arg);
   //
   // x
   Vector<double> x(2);
   x[0] = 5.0;
   x[1] = 6.0;
   //
   // value_vec
   size_t n_res = size_t( tape.n_res() );
   Vector<double> value_vec(n_res);
   for(size_t i = 0; i < 2; ++i)
      value_vec[i] = x[i];
   //
   // value_vec
   tape.eval(value_vec);
   //
   // y
   Vector<double> y(2);
   for(size_t i = 0; i < 2; ++i)
      y[i] = value_vec[ res_index[i] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1];
   ok &= y[1] == x[0] - x[1];
   //
   if( ok )
   {  std::cout << "op_class: OK\n";
      return 0;
   }
   std::cout << "op_class: Error\n";
   return 1;
}
