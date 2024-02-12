# include <cmath>
# include <iostream>
# include <cassert>
# include <cppad/utility/vector.hpp>
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
class valvector;
//
// <<
inline std::ostream& operator << 
(std::ostream&  os , const valvector& a);
//
class valvector {
   friend std::ostream& operator << 
      (std::ostream&  os , const valvector& a);
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
   const valvector& a  )
{  os << "{ ";
   for(size_t i = 0; i < a.vec_.size(); ++i)
   {  os << a.vec_[i];
      if( i + 1 < a.vec_.size() )
         os << ", ";
   }
   os << " }";
   return os;
}

# if 0
// CondExp
template <class Vector>
inline valvector<Vector> CondExp(
const valvector<Vector>&       left         ,
const valvector<Vector>&       right        ,
const valvector<Vector>&       exp_if_true  ,
const valvector<Vector>&       exp_if_false )
{  VALVECTOR_ASSERT_KNOWN(
      false,
      "Conditional expressions not yet implemented"
   );
   // avoid compiler error
   return valvector<Vector>();
}
//
// CondExpRel for Rel = Lt, Le, Eq, Ge, Gt
# define VALVECTOR_COND_EXP(Name) \
   template <class Vector> \
   inline valvector<Vector> CondExp##Name( \
   const valvector<Vector>&       left         , \
   const valvector<Vector>&       right        , \
   const valvector<Vector>&       exp_if_true  , \
   const valvector<Vector>&       exp_if_false ) \
   {  VALVECTOR_ASSERT_KNOWN( \
         false, \
         "Conditional expressions not yet implemented" \
      ); \
      /* avoid compiler error */ \
      return valvector<Vector>(); \
   }
VALVECTOR_COND_EXP(Lt)
VALVECTOR_COND_EXP(Le)
VALVECTOR_COND_EXP(Eq)
VALVECTOR_COND_EXP(Ge)
VALVECTOR_COND_EXP(Gt)
# undef VALVECTOR_COND_EXP
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
