# ifndef CppADADIncluded
# define CppADADIncluded

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

// define the template classes that are used by the AD template class
# include <CppAD/local/Operator.h>
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
	inline AD& operator Op (const VecADelem<Base> &right) \
	{	return *this Op right.ADBase(); }

// use this marco for binary operators 
# define CppADBinaryMember(Op)                                     \
	inline AD operator Op (const AD &right) const;             \
	inline AD operator Op (int right) const;                   \
	inline AD operator Op (const Base &right) const;           \
	inline AD operator Op (const VecADelem<Base> &right) const;

// use this marco for comparison operators 
# define CppADCompareMember(Op)                                      \
	inline bool operator Op (const AD &right) const;             \
	inline bool operator Op (int right) const;                   \
	inline bool operator Op (const Base &right) const;           \
	inline bool operator Op (const VecADelem<Base> &right) const;

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
class AD {
	friend bool GreaterThanZero <Base> 
		(const AD<Base> &u);
	friend bool LessThanZero  <Base> 
		(const AD<Base> &u);
	friend bool LessThanOrZero  <Base> 
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

	friend bool IdenticalEqual <Base> 
		(const AD<Base> &u, const AD<Base> &v);

	friend AD<Base> CondExpOp  <Base> (
		enum CompareOp  cop       ,
		const AD<Base> &left      , 
		const AD<Base> &right     , 
		const AD<Base> &trueCase  , 
		const AD<Base> &falseCase 
	);

	friend class ADTape<Base>;
	friend class ADDiscrete<Base>;
	friend class ADFun<Base>;
	friend class VecAD<Base>;
	friend class VecADelem<Base>;

	// output
	friend std::ostream& operator << <Base>
		(std::ostream &os, const AD<Base> &x);
	friend void PrintFor <Base>
		(const char *text, const AD<Base> &x);

	// CppAD/Example/NearEqualExt.h
	friend bool NearEqual <Base> (
		const AD<Base> &x, const AD<Base> &y, double r, double a);
	friend bool NearEqual <Base> (
		const Base &x, const AD<Base> &y, double r, double a);
	friend bool NearEqual <Base> (
		const AD<Base> &x, const Base &y, double r, double a);

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

	// copy constructor 
	inline AD(const AD &x);

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

	// destructor
	~AD(void)
	{ }

	// interface so these functions need not be friends
	inline AD Abs(void) const;
	inline AD Log(void) const;
	inline AD Exp(void) const;
	inline AD Cos(void) const;
	inline AD Sin(void) const;
	inline AD Atan(void) const;
	inline AD Sqrt(void) const;
	inline AD Asin(void) const;
	inline AD Acos(void) const;

	/*
	Functions declared public so can be accessed by user through
	a macro interface not not intended for direct use.
	Macro interface is documented in BoolFun.h.
	Developer documentation for these fucntions is in ADBoolFun.h
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
	{	static ADTape<Base> tape;
		return &tape;
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
# include <CppAD/local/Ordered.h>
# include <CppAD/local/Identical.h>
# include <CppAD/local/Integer.h>
# include <CppAD/local/CondExp.h>

# undef CppADCompareFriend
# undef CppADAssignMember
# undef CppADBinaryMember

# endif
