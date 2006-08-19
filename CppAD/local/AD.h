# ifndef CppADADIncluded
# define CppADADIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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

// simple AD operations that must be defined for AD as well as base class
# include <CppAD/local/Ordered.h>
# include <CppAD/local/Identical.h>

// define the template classes that are used by the AD template class
# include <CppAD/local/OpCode.h>
# include <CppAD/local/TapeRec.h>
# include <CppAD/local/ADTape.h>

// macro for base type binary operators is used in multiple files
// so declared in CppAD instead of here

// use this macro for all the Base logical operators
# define CppADCompareFriend(Op)                                  \
                                                                 \
	friend bool operator Op <Base>                           \
		 (const AD<Base> &left, const AD<Base> &right);  \
                                                                 \
	friend bool operator Op <Base>                           \
		 (const Base &left, const AD<Base> &right);      \
                                                                 \
	friend bool operator Op <Base>                           \
		 (const AD<Base> &left, const Base &right)


// use this marco for assignment and computed assignment
# define CppADAssignMember(Op)                                \
	inline AD& operator Op (const AD &right);             \
	inline AD& operator Op (int right)                    \
	{	return *this Op AD(right); }                  \
	inline AD& operator Op (const Base &right)            \
	{	return *this Op AD(right); }                  \
	inline AD& operator Op (const VecAD_reference<Base> &right) \
	{	return *this Op right.ADBase(); }

// use this marco for binary operators 
# define CppADBinaryMember(Op)                                     \
	inline AD operator Op (const AD &right) const;             \
	inline AD operator Op (int right) const;                   \
	inline AD operator Op (const Base &right) const;           \
	inline AD operator Op (const VecAD_reference<Base> &right) const;

// use this marco for comparison operators 
# define CppADCompareMember(Op)                                      \
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
	CppADCompareMember( <  )
	CppADCompareMember( <= )
	CppADCompareMember( >  )
	CppADCompareMember( >= )
	CppADCompareMember( == )
	CppADCompareMember( != )

	// binary operators
	CppADBinaryMember(+)
	CppADBinaryMember(-)
	CppADBinaryMember(*)
	CppADBinaryMember(/)

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
	CppADAssignMember(  = )
	CppADAssignMember( += )
	CppADAssignMember( -= )
	CppADAssignMember( *= )
	CppADAssignMember( /= )

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

# undef CppADCompareFriend
# undef CppADAssignMember
# undef CppADBinaryMember

# endif
