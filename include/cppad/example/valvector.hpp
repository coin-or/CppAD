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
# define VALVECTOR_BINARY_NUMERIC_OP(op) \
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
   }
//
# define VALVECTOR_BINARY_COMPARE_OP(op) \
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
         for(size_t i = 0; i < size(); ++i) \
            result &= vec_[i] op other.vec_[i]; \
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
   // Binary Operators
   VALVECTOR_BINARY_NUMERIC_OP(+)
   VALVECTOR_BINARY_NUMERIC_OP(-)
   VALVECTOR_BINARY_NUMERIC_OP(*)
   VALVECTOR_BINARY_NUMERIC_OP(/)
   //
   // Compare operators
   VALVECTOR_BINARY_COMPARE_OP(==)
   VALVECTOR_BINARY_COMPARE_OP(!=)
   VALVECTOR_BINARY_COMPARE_OP(<=)
   VALVECTOR_BINARY_COMPARE_OP(>=)
   VALVECTOR_BINARY_COMPARE_OP(<)
   VALVECTOR_BINARY_COMPARE_OP(>)
};

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
