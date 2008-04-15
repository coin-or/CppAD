# ifndef CPPAD_AD_TAPE_INCLUDED
# define CPPAD_AD_TAPE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ADTape$$ $comment CppAD Developer Documentation$$
$aindex head$$

$spell
	taddr_
	inline
	Var
	Prip
	Priv
	Ldp
	Ldv
	Stpv
	Stvv
	Stpp
	Stvp
	Inv
	Vec
	Sto
	VecInd
	Inv
	Ind
	Num
	Op
	Cpp
	const
	bool
	Len
	xy
	xx
	yy
$$

$section ADTape: The CppAD Tape$$

$head Syntax$$
$syntax%ADTape<%Base%> %Tape%$$


$head Description$$
For each $italic Base$$ that is used in connection with
$syntax%AD<%Base%>%$$, 
there must be one and only one $italic id$$ such that
$syntax%ADBase<%Base%>::tape_active(%id%)%$$ is true.
This object is used to record 
$syntax%AD<%Base%>%$$ operations and compute derivatives.

$head Rec$$
the $xref/recorder/$$ object $syntax%%Tape%.Rec%$$ contains
the currently recorded information.
This information is recorded using the following functions:

$subhead Parameter$$
The procedure call
$syntax%
	size_t %Tape%.RecordParOp(const %Base% &%z%)
%$$
creates a $code ParOp$$ record with the value 
specified by $italic z$$.
The return value is the taddr of this operation in the tape.

$subhead Variable Indexed Arrays$$
The procedure call
$syntax%
	size_t %Tape%.AddVec(size_t  %length%, const %Base% *%data%)
%$$
adds a variable indexed array with the specified length and values to the tape.
We use $italic i$$ to denote the value returned by $code AddVec$$.
The value $italic length$$ is added to $italic Rec$$ as follows:
$syntax%
	%length% == %Rec%.GetVecInd(%i%)
%$$
Upon return, 
the $italic Base$$ value of the elements of $italic data$$ 
are stored in $italic Rec$$
in the following way:
for $latex j = 0 , \ldots , length-1$$,
$syntax%
	%data%[%j%] == %Rec%.GetVecInd(%i% + %j% + 1)
%$$


$end
*/

//  BEGIN CppAD namespace
namespace CppAD {


template <class Base>
class ADTape {

	// classes
	friend class AD<Base>;
	friend class ADFun<Base>;
	friend class ADDiscrete<Base>;
	friend class VecAD<Base>;
	friend class VecAD_reference<Base>;

	//
	// functions
	//

	// PrintFor
	friend void PrintFor <Base>
		(const char *text, const AD<Base> &x);

	// CondExpOp
	friend AD<Base> CondExpOp <Base> (
		enum CompareOp  cop          ,
		const AD<Base> &left         , 
		const AD<Base> &right        , 
		const AD<Base> &trueCase     , 
		const AD<Base> &falseCase 
	);

	// arithematic binary operators
	friend AD<Base> operator + <Base>
		(const AD<Base> &left, const AD<Base> &right);
	friend AD<Base> operator - <Base>
		(const AD<Base> &left, const AD<Base> &right);
	friend AD<Base> operator * <Base>
		(const AD<Base> &left, const AD<Base> &right);
	friend AD<Base> operator / <Base>
		(const AD<Base> &left, const AD<Base> &right);

	// comparison operators
	friend bool operator < <Base>
		(const AD<Base> &left, const AD<Base> &right);
	friend bool operator <= <Base>
		(const AD<Base> &left, const AD<Base> &right);
	friend bool operator > <Base>
		(const AD<Base> &left, const AD<Base> &right);
	friend bool operator >= <Base>
		(const AD<Base> &left, const AD<Base> &right);
	friend bool operator == <Base>
		(const AD<Base> &left, const AD<Base> &right);
	friend bool operator != <Base>
		(const AD<Base> &left, const AD<Base> &right);


	// pow
	friend AD<Base> pow <Base>
		(const AD<Base> &x, const AD<Base> &y);

	// Parameter
	friend bool Parameter     <Base> 
		(const AD<Base> &u);
	// Variable
	friend bool Variable      <Base> 
		(const AD<Base> &u);

public:
	// constructor
	ADTape(size_t id) : id_(id)
	{ }

	// destructor
	~ADTape(void)
	{	Rec_.Erase(); }

	// public function only used by CppAD::Independent
	template <typename VectorADBase>
	void Independent(VectorADBase &u);

private:
	// private data
	size_t                       id_;
	size_t         size_independent_;
	recorder<Base>              Rec_;

	/*
	Private functions
	*/

	// add a parameter to the tape
	size_t RecordParOp(const Base &x);
	
	// see CondExp.h
	void RecordCondExp(
		enum CompareOp  cop           ,
		AD<Base>       &returnValue   ,
		const AD<Base> &left          ,
		const AD<Base> &right         ,
		const AD<Base> &trueCase      ,
		const AD<Base> &falseCase
	);

	// see Compare.h
	void RecordCompare(
		enum CompareOp  cop      ,
		bool            result   ,
		const AD<Base> &left     ,
		const AD<Base> &right
	);

	size_t AddVec(
		size_t        length,
		const Base   *data
	);

};
// ---------------------------------------------------------------------------
// Private functions
//

template <class Base>
size_t ADTape<Base>::RecordParOp(const Base &z)
{	size_t z_taddr;
	size_t ind;
	CPPAD_ASSERT_UNKNOWN( NumVar(ParOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumInd(ParOp) == 1 );
	z_taddr = Rec_.PutOp(ParOp);
	ind     = Rec_.PutPar(z);
	Rec_.PutInd(ind);

	return z_taddr;
}

template <class Base>
size_t ADTape<Base>::AddVec(size_t length, const Base *data)
{	CPPAD_ASSERT_UNKNOWN( length > 0 );
	size_t i;
	size_t value_index;

	// store the length in VecInd
	size_t start = Rec_.PutVecInd(length);

	// store indices of the values in VecInd 
	for(i = 0; i < length; i++)
	{
		value_index = Rec_.PutPar( data[i] );
		Rec_.PutVecInd( value_index );
	}
 
	// return the taddr of the length (where the vector starts)
	return start;
}


} // END CppAD namespace

# endif
