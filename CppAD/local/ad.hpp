# ifndef CPPAD_AD_INCLUDED
# define CPPAD_AD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// simple AD operations that must be defined for AD as well as base class
# include <CppAD/local/ordered.hpp>
# include <CppAD/local/identical.hpp>

// define the template classes that are used by the AD template class
# include <CppAD/local/op_code.hpp>
# include <CppAD/local/tape_rec.hpp>
# include <CppAD/local/ad_tape.hpp>

// use this marco for assignment and computed assignment
# define CPPAD_ASSIGN_MEMBER(Op)                              \
	inline AD& operator Op (const AD &right);             \
	inline AD& operator Op (int right)                    \
	{	return *this Op AD(right); }                  \
	inline AD& operator Op (const Base &right)            \
	{	return *this Op AD(right); }                  \
	inline AD& operator Op (const VecAD_reference<Base> &right) \
	{	return *this Op right.ADBase(); }

// use this marco for binary operators 
# define CPPAD_BINARY_MEMBER(Op)                                   \
	inline AD operator Op (const AD &right) const;             \
	inline AD operator Op (int right) const;                   \
	inline AD operator Op (const Base &right) const;           \
	inline AD operator Op (const VecAD_reference<Base> &right) const;

// use this marco for comparison operators 
# define CPPAD_COMPARE_MEMBER(Op)                                    \
	inline bool operator Op (const AD &right) const;             \
	inline bool operator Op (int right) const;                   \
	inline bool operator Op (const Base &right) const;           \
	inline bool operator Op (const VecAD_reference<Base> &right) const;

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
class AD {
	// one argument functions
	friend bool GreaterThanZero <Base> 
		(const AD<Base> &u);
	friend bool LessThanZero  <Base> 
		(const AD<Base> &u);
	friend bool LessThanOrZero  <Base> 
		(const AD<Base> &u);
	friend bool GreaterThanOrZero  <Base> 
		(const AD<Base> &u);
	friend bool Parameter     <Base> 
		(const AD<Base> &u);
	friend bool Variable      <Base> 
		(const AD<Base> &u);
	friend bool IdenticalPar  <Base> 
		(const AD<Base> &u);
	friend bool IdenticalZero <Base> 
		(const AD<Base> &u);
	friend bool IdenticalOne  <Base> 
		(const AD<Base> &u);
	friend int  Integer       <Base> 
		(const AD<Base> &u);

	// IdenticalEqual function
	friend bool IdenticalEqual <Base> 
		(const AD<Base> &u, const AD<Base> &v);

	// NearEqual function
	friend bool NearEqual <Base> (
	const AD<Base> &x, const AD<Base> &y, const Base &r, const Base &a);

	friend bool NearEqual <Base> (
	const Base &x, const AD<Base> &y, const Base &r, const Base &a);

	friend bool NearEqual <Base> (
	const AD<Base> &x, const Base &y, const Base &r, const Base &a);

	// CondExp function
	friend AD<Base> CondExpOp  <Base> (
		enum CompareOp  cop       ,
		const AD<Base> &left      , 
		const AD<Base> &right     , 
		const AD<Base> &trueCase  , 
		const AD<Base> &falseCase 
	);

	// classes
	friend class ADTape<Base>;
	friend class ADDiscrete<Base>;
	friend class ADFun<Base>;
	friend class VecAD<Base>;
	friend class VecAD_reference<Base>;

	// output operations
	friend std::ostream& operator << <Base>
		(std::ostream &os, const AD<Base> &x);
	friend void PrintFor <Base>
		(const char *text, const AD<Base> &x);

public:
	// type of value
	typedef Base value_type;

	// comparison operators
	CPPAD_COMPARE_MEMBER( <  )
	CPPAD_COMPARE_MEMBER( <= )
	CPPAD_COMPARE_MEMBER( >  )
	CPPAD_COMPARE_MEMBER( >= )
	CPPAD_COMPARE_MEMBER( == )
	CPPAD_COMPARE_MEMBER( != )

	// binary operators
	CPPAD_BINARY_MEMBER(+)
	CPPAD_BINARY_MEMBER(-)
	CPPAD_BINARY_MEMBER(*)
	CPPAD_BINARY_MEMBER(/)

	// default constructor
	inline AD(void);

	// construction from base type
	inline AD(const Base &b);

	// use default copy constructor
	// inline AD(const AD &x);

	// contructor from VecAD<Base>::reference
	inline AD(const VecAD_reference<Base> &x);

	// construction from some other type
	template <class T>
	inline AD(const T &t);

	// base type corresponding to an AD object
	friend Base Value <Base> (const AD<Base> &x);

	// binary operators implemented as member functions
	CPPAD_ASSIGN_MEMBER(  = )
	CPPAD_ASSIGN_MEMBER( += )
	CPPAD_ASSIGN_MEMBER( -= )
	CPPAD_ASSIGN_MEMBER( *= )
	CPPAD_ASSIGN_MEMBER( /= )

	// unary operators
	inline AD operator +(void) const;
	inline AD operator -(void) const;

	// destructor
	~AD(void)
	{ }

	// interface so these functions need not be friends
	inline AD Abs(void) const;
	inline AD acos(void) const;
	inline AD asin(void) const;
	inline AD atan(void) const;
	inline AD cos(void) const;
	inline AD cosh(void) const;
	inline AD exp(void) const;
	inline AD log(void) const;
	inline AD sin(void) const;
	inline AD sinh(void) const;
	inline AD sqrt(void) const;

	/*
	Functions declared public so can be accessed by user through
	a macro interface not not intended for direct use.
	Macro interface is documented in BoolFun.h.
	Developer documentation for these fucntions is in BoolFunLink.h
	*/
	static inline bool UnaryBool(
		bool FunName(const Base &x),
		const AD<Base> &x
	);
	static inline bool BinaryBool(
		bool FunName(const Base &x, const Base &y),
		const AD<Base> &x , const AD<Base> &y
	);

	// There is only one tape so construct it here
	static ADTape<Base> *Tape(void)
	{	// If we return &tape, instead of creating and returning ptr,
		// there seems to be a bug in g++ with -O2 option.
		static ADTape<Base> tape;
		static ADTape<Base> *ptr = &tape;
		return ptr;
	}

	// Make this object correspond to a new variable on the tape
	inline void MakeVariable( size_t taddr_ )
	{	CppADUnknownError( Parameter(*this) ); // currently a parameter
		CppADUnknownError( taddr_ > 0 );       // make sure valid taddr

		taddr = taddr_;
		id    = *ADTape<Base>::Id();
	}

	// Make this object correspond to a parameter
	inline void MakeParameter( void )
	{	CppADUnknownError( Variable(*this) ); // currently a variable
		CppADUnknownError( id == *ADTape<Base>::Id() ); 

		id = 0;
	}

private:
	// value corresponding to this object
	Base value;

	// taddr in tape for this variable 
	size_t taddr;

	// identifier corresponding to taddr
	// This is a parameter if and only if id != *ADTape<Base>::Id()
	size_t id;
}; 
// ---------------------------------------------------------------------------

} // END CppAD namespace

// operations that expect the AD template class to be defined

# undef CPPAD_ASSIGN_MEMBER
# undef CPPAD_BINARY_MEMBER
# undef CPPAD_COMPARE_MEMBER

# endif
