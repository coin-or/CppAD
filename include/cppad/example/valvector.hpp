# include <cmath>
# include <iostream>
# include <cassert>
# include <cppad/utility/vector.hpp>
# include <cppad/base_require.hpp>
//
# define VALVECTOR_ASSERT_KNOWN(exp, msg) \
   if( ! (exp ) ) \
   {  std::cerr << "valvector: " << msg << "\n"; \
      assert( exp ); \
   }
//
# define VALVECTOR_STD_MATH_MEMBER(fun) \
   valvector fun(void) const \
   {  valvector result; \
      result.resize( size() ); \
      for(size_t i = 0; i < size(); ++i) \
         result.vec_[i] = std::fun( vec_[i] ); \
      return result; \
   }
//
# define VALVECTOR_STD_MATH_FUNCTION(fun) \
   inline valvector fun(const valvector& x) \
   {  return x.fun(); \
   }
//
# define VALVECTOR_BINARY_NUMERIC_OP(op, compound_op) \
   valvector operator op(const valvector& other) const \
   {  valvector result;   \
      if( size() == 1 ) \
      {  result.resize( other.size() ); \
         for(size_t i = 0; i < other.size(); ++i) \
            result.vec_[i] = vec_[0] op other.vec_[i]; \
      } \
      else if( other.size() == 1 ) \
      {  result.resize( size() ); \
         for(size_t i = 0; i < size(); ++i) \
            result.vec_[i] = vec_[i] op other.vec_[0]; \
      } \
      else \
      {  VALVECTOR_ASSERT_KNOWN(  \
            size() == other.size() , \
            "size error using " #op " operator" \
         ) \
         result.vec_.resize( size() ); \
         for(size_t i = 0; i < size(); ++i) \
            result.vec_[i] = vec_[i] op other.vec_[i]; \
      } \
      return result; \
   } \
   valvector& operator compound_op(const valvector& other) \
   {  if( size() == 1 ) \
      {  scalar_type vec_0 = vec_[0]; \
         resize( other.size() ); \
         for(size_t i = 0; i < other.size(); ++i) \
            vec_[i] = vec_0 op other.vec_[i]; \
      } \
      else if( other.size() == 1 ) \
      {  for(size_t i = 0; i < size(); ++i) \
            vec_[i] = vec_[i] op other.vec_[0]; \
      } \
      else \
      {  VALVECTOR_ASSERT_KNOWN(  \
            size() == other.size() , \
            "size error using " #compound_op " operator" \
         ) \
         for(size_t i = 0; i < size(); ++i) \
            vec_[i] = vec_[i] op other.vec_[i]; \
      } \
      return *this; \
   }
//
// Lexiographic Compare
# define VALVECTOR_LEXICOGRAPHIC_COMPARE_OP(op) \
   bool operator op(const valvector& other) const \
   {  bool result = true;   \
      if( size() == 1 ) \
      {  for(size_t i = 0; i < other.size(); ++i) \
            result &= vec_[0] op other.vec_[i]; \
      } \
      else if( other.size() == 1 ) \
      {  for(size_t i = 0; i < size(); ++i) \
            result &= vec_[i] op other.vec_[0]; \
      } \
      else \
      {  VALVECTOR_ASSERT_KNOWN(  \
            size() == other.size() , \
            "size error using " #op " operator" \
         ) \
         size_t i = 0; \
         while( i < size() && vec_[i] == other.vec_[i] ) \
            ++i; \
         if( i == size() ) \
            --i; \
         result = vec_[i] op other.vec_[i]; \
      } \
      return result; \
   }
//
// valvector
// Forward declare
class valvector;  
//
// CondExpOp
// Forward decalre
namespace CppAD {
   inline valvector CondExpOp(
      enum CompareOp         cop          ,
      const valvector&       left         ,
      const valvector&       right        ,
      const valvector&       exp_if_true  ,
      const valvector&       exp_if_false 
   );
}
//
// valvector
class valvector {
   //
   // friend
   friend inline valvector CppAD::CondExpOp(
      enum CppAD::CompareOp  cop          ,
      const valvector&       left         ,
      const valvector&       right        ,
      const valvector&       exp_if_true  ,
      const valvector&       exp_if_false 
   );
public:
   //
   // scalar_type, vector_type
   typedef double                     scalar_type;
   typedef CppAD::vector<scalar_type> vector_type;
   //
private:
   // vec_
   vector_type vec_;
   //
   // check_size_ok
   void check_size_ok(const valvector& other) const
   {  VALVECTOR_ASSERT_KNOWN(
         size() == other.size() || size() == 1 || other.size() == 1,
         "sizes do not agree in a binary operation"
      )
   }
   //
public:
   //
   // constructors
   valvector(void)
   { }
   valvector(scalar_type s) : vec_(1)
   {  vec_[0] = s; }
   valvector(const vector_type& vec) : vec_(vec)
   { }
   valvector(const valvector& other) : vec_( other.vec_)
   { }
   valvector(valvector&& other)
   {  vec_.swap( other.vec_ ); 
   }  
   valvector(std::initializer_list<scalar_type> list) : vec_(list)
   { }
   //
   // assignments
   valvector& operator=(const valvector& other)
   {  vec_ = other.vec_;
      return *this;
   }
   valvector& operator=(valvector&& other)
   {  vec_.swap( other.vec_ ); 
      return *this;
   }
   //
   // size
   size_t size(void) const
   {  return vec_.size(); }
   //
   // resize
   void resize(size_t n)
   {  vec_.resize(n); }
   //
   // iszero
   bool iszero(void) const
   {  bool        result = true;
      scalar_type zero   = 0;
      for(size_t i = 0; i < size(); ++i)
         result &= vec_[i] == zero; 
      return result;
   }
   //
   // isone
   bool isone(void) const
   {  bool        result = true;
      scalar_type one    = 1;
      for(size_t i = 0; i < size(); ++i)
         result &= vec_[i] == one; 
      return result;
   }
   //
   // output
   std::ostream& output(std::ostream& os)  const
   {  os << "{ ";
      for(size_t i = 0; i < vec_.size(); ++i)
      {  os << vec_[i];
         if( i + 1 < vec_.size() )
            os << ", ";
      }
      os << " }";
      return os;
   }
   //
   // unary operators
   valvector operator+(void)
   {  return *this; }
   valvector operator-(void)
   {  valvector result;
      result.resize( size() );
      for(size_t i = 0; i < size(); ++i)
         result.vec_[i] = - vec_[i];
      return result;
   }
   //
   // Standard Math Fucntons
   VALVECTOR_STD_MATH_MEMBER(acos)
   VALVECTOR_STD_MATH_MEMBER(acosh)
   VALVECTOR_STD_MATH_MEMBER(asin)
   VALVECTOR_STD_MATH_MEMBER(asinh)
   VALVECTOR_STD_MATH_MEMBER(atan)
   VALVECTOR_STD_MATH_MEMBER(atanh)
   VALVECTOR_STD_MATH_MEMBER(cos)
   VALVECTOR_STD_MATH_MEMBER(cosh)
   VALVECTOR_STD_MATH_MEMBER(erf)
   VALVECTOR_STD_MATH_MEMBER(erfc)
   VALVECTOR_STD_MATH_MEMBER(exp)
   VALVECTOR_STD_MATH_MEMBER(expm1)
   VALVECTOR_STD_MATH_MEMBER(fabs)
   VALVECTOR_STD_MATH_MEMBER(log)
   VALVECTOR_STD_MATH_MEMBER(log1p)
   VALVECTOR_STD_MATH_MEMBER(log10)
   VALVECTOR_STD_MATH_MEMBER(sin)
   VALVECTOR_STD_MATH_MEMBER(sinh)
   VALVECTOR_STD_MATH_MEMBER(sqrt)
   VALVECTOR_STD_MATH_MEMBER(tan)
   VALVECTOR_STD_MATH_MEMBER(tanh)
   //
   // Binary Operators
   VALVECTOR_BINARY_NUMERIC_OP(+, +=)
   VALVECTOR_BINARY_NUMERIC_OP(-, -=)
   VALVECTOR_BINARY_NUMERIC_OP(*, *=)
   VALVECTOR_BINARY_NUMERIC_OP(/, /=)
   //
   // Compare operators
   VALVECTOR_LEXICOGRAPHIC_COMPARE_OP(==)
   VALVECTOR_LEXICOGRAPHIC_COMPARE_OP(!=)
   VALVECTOR_LEXICOGRAPHIC_COMPARE_OP(<=)
   VALVECTOR_LEXICOGRAPHIC_COMPARE_OP(>=)
   VALVECTOR_LEXICOGRAPHIC_COMPARE_OP(<)
   VALVECTOR_LEXICOGRAPHIC_COMPARE_OP(>)
};
//
// Numeric Unary Fucntions
namespace CppAD {
   VALVECTOR_STD_MATH_FUNCTION(acos)
   VALVECTOR_STD_MATH_FUNCTION(acosh)
   VALVECTOR_STD_MATH_FUNCTION(asin)
   VALVECTOR_STD_MATH_FUNCTION(asinh)
   VALVECTOR_STD_MATH_FUNCTION(atan)
   VALVECTOR_STD_MATH_FUNCTION(atanh)
   VALVECTOR_STD_MATH_FUNCTION(cos)
   VALVECTOR_STD_MATH_FUNCTION(cosh)
   VALVECTOR_STD_MATH_FUNCTION(erf)
   VALVECTOR_STD_MATH_FUNCTION(erfc)
   VALVECTOR_STD_MATH_FUNCTION(exp)
   VALVECTOR_STD_MATH_FUNCTION(expm1)
   VALVECTOR_STD_MATH_FUNCTION(fabs)
   VALVECTOR_STD_MATH_FUNCTION(log)
   VALVECTOR_STD_MATH_FUNCTION(log1p)
   VALVECTOR_STD_MATH_FUNCTION(log10)
   VALVECTOR_STD_MATH_FUNCTION(sin)
   VALVECTOR_STD_MATH_FUNCTION(sinh)
   VALVECTOR_STD_MATH_FUNCTION(sqrt)
   VALVECTOR_STD_MATH_FUNCTION(tan)
   VALVECTOR_STD_MATH_FUNCTION(tanh)
}
//
// ostream << valvector
inline std::ostream& operator << (
   std::ostream&    os , 
   const valvector& v  )
{  return v.output(os);
}
//
// CondExpOp
namespace CppAD {
   inline valvector CondExpOp(
      enum CompareOp         cop          ,
      const valvector&       left         ,
      const valvector&       right        ,
      const valvector&       exp_if_true  ,
      const valvector&       exp_if_false )
   {  //  
      // result_size
      size_t result_size = std::max(left.size(), right.size());
      result_size        = std::max(result_size, exp_if_true.size());
      result_size        = std::max(result_size, exp_if_false.size());
      //
      // size_ok
      bool size_ok  = true;
      size_ok &= left.size()         ==1 || left.size()         ==result_size;
      size_ok &= right.size()        ==1 || right.size()        ==result_size;
      size_ok &= exp_if_true.size()  ==1 || exp_if_true.size()  ==result_size;
      size_ok &= exp_if_false.size() ==1 || exp_if_false.size() ==result_size;
      VALVECTOR_ASSERT_KNOWN(
         size_ok,
         "argument sizes do not agree in conditional expression"
      );
      //
      // result
      valvector result;
      result.resize(result_size);
      //
      // inc_left, inc_right, inc_true, inc_false
      size_t inc_left  = size_t( left.size() != 1 );
      size_t inc_right = size_t( right.size() != 1 );
      size_t inc_true  = size_t( exp_if_true.size() != 1 );
      size_t inc_false = size_t( exp_if_false.size() != 1 );
      //
      // index_left, index_right, index_true, index_false
      size_t index_left  = 0;
      size_t index_right = 0;
      size_t index_true  = 0;
      size_t index_false = 0;
      for(size_t index_result = 0; index_result < result_size; ++index_result)
      {  switch( cop )
         {
            case CompareLt:
            if( left.vec_[index_left] < right.vec_[index_right] )
               result.vec_[index_result] = exp_if_true.vec_[index_true];
            else
               result.vec_[index_result] = exp_if_false.vec_[index_false];;
            break;
   
            case CompareLe:
            if( left.vec_[index_left] <= right.vec_[index_right] )
               result.vec_[index_result] = exp_if_true.vec_[index_true];
            else
               result.vec_[index_result] = exp_if_false.vec_[index_false];;
            break;

            case CompareEq:
            if( left.vec_[index_left] == right.vec_[index_right] )
               result.vec_[index_result] = exp_if_true.vec_[index_true];
            else
               result.vec_[index_result] = exp_if_false.vec_[index_false];;
            break;
   
            case CompareGe:
            if( left.vec_[index_left] >= right.vec_[index_right] )
               result.vec_[index_result] = exp_if_true.vec_[index_true];
            else
               result.vec_[index_result] = exp_if_false.vec_[index_false];;
            break;

            case CompareGt:
            if( left.vec_[index_left] > right.vec_[index_right] )
               result.vec_[index_result] = exp_if_true.vec_[index_true];
            else
               result.vec_[index_result] = exp_if_false.vec_[index_false];;
            break;
   
            default:
            assert(false);
            result.vec_[index_result] = exp_if_true.vec_[index_true];
         }
         // index_left, index_right, index_true, index_false
         index_left  += inc_left;
         index_right += inc_right;
         index_true  += inc_true;
         index_false += inc_false;
      }
      // result
      return result;
   }
   CPPAD_COND_EXP_REL(valvector)
}

# if 0
//
// Identical
namespace CppAD {
   template <class Vector>
   inline bool IdenticalCon(const valvector<Vector> & x)
   {  return true; 
   }
   template <class Vector>
   inline bool IdenticalZero(const valvector<Vector> & x)
   {  return x.iszero(); 
   }
   template <class Vector>
   inline bool IdenticalOne(const valvector<Vector> & x)
   {  return x.isone(); 
   }
   template <class Vector>
   inline bool IdenticalEqualCon(
      const valvector<Vector> & x, 
      const valvector<Vector> & y)
   {  return x == y; }
}
# endif
