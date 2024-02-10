# include <iostream>
# include <cassert>
//
# define VALVECTOR_ASSERT_KNOWN(exp, msg) \
   if( ! exp ) \
   {  std::cerr << "valvector: " << msg << "\n"; \
      assert( exp ); \
   }
//
# define VALVECTOR_BOOL_OPERATOR(op) \
   void operator op(const valvector& other) const \
   {  VALVECTOR_ASSERT_KNOWN( \
         false, \
         #op " compare operator is not yet implemented" \
      ); \
   }

# define VALVECTOR_BINARY_OPERATOR(op) \
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
// valvector
template <class VectorType> class valvector;
//
// <<
template <class Vector>
std::ostream& operator << 
(std::ostream&  os , const valvector<Vector>& a);
//
template <class Vector>
class valvector {
   friend std::ostream& operator << <Vector> 
      (std::ostream&  os , const valvector<Vector>& a);
private:
   // vec_
   Vector vec_;
   //
public:
   // value_type
   typedef typename Vector::value_type value_type;
   //
   // constructors
   valvector(void)
   { }
   valvector(size_t n) : vec_(n)
   { }
   valvector(const Vector& vec) : vec_(vec)
   { }
   valvector(const valvector& other) : vec_( other.vec_)
   { }
   valvector(valvector&& other)
   {  vec_.swap( other.vec_ ); 
   }  
   valvector(std::initializer_list<value_type> list) : vec_(list)
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
   {  bool       result = true;
      value_type zero   = 0;
      for(size_t i = 0; i < size(); ++i)
         result &= vec_[i] == zero; 
      return result;
   }
   //
   // isone
   bool isone(void) const
   {  bool       result = true;
      value_type one    = 1;
      for(size_t i = 0; i < size(); ++i)
         result &= vec_[i] == one; 
      return result;
   }
   //
   //
   // Binary Operators
   VALVECTOR_BINARY_OPERATOR(+)
   VALVECTOR_BINARY_OPERATOR(-)
   VALVECTOR_BINARY_OPERATOR(*)
   VALVECTOR_BINARY_OPERATOR(/)
   //
   // Compare operators
   bool operator ==(const valvector& other) const
   {  bool       result = true;
      if( size() == 1 )
      {  for(size_t i = 0; i < other.size(); ++i)
            result &= vec_[0] == other.vec_[i]; 
      }
      else if( other.size() == 1 )
      {  for(size_t i = 0; i < size(); ++i)
            result &= vec_[i] == other.vec_[0]; 
      }
      else if( size() != other.size() )
         result = false;
      else
      {  for(size_t i = 0; i < size(); ++i)
            result &= vec_[i] == other.vec_[i]; 
      }
      return result;
   }
   bool operator !=(const valvector& other) const
   {  return ! ( *this == other );
   }
   VALVECTOR_BOOL_OPERATOR(<=)
   VALVECTOR_BOOL_OPERATOR(>=)
   VALVECTOR_BOOL_OPERATOR(<)
   VALVECTOR_BOOL_OPERATOR(>)
};

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
# if 0
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
# endif
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


//
// ostream << valvector
template <class Vector>
std::ostream& operator << (
   std::ostream&            os , 
   const valvector<Vector>& a  )
{  os << "{ ";
   for(size_t i = 0; i < a.vec_.size(); ++i)
   {  os << a.vec_[i];
      if( i + 1 < a.vec_.size() )
         os << ", ";
   }
   os << " }";
   return os;
}
