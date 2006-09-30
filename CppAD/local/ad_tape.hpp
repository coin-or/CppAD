# ifndef CPPAD_AD_TAPE_INCLUDED
# define CPPAD_AD_TAPE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
	taddr
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
there must be one and only one object with type
$syntax%ADTape<%Base%>%$$.
This object is used to record 
$syntax%AD<%Base%>%$$ operations and compute derivatives.

$head state$$
The field
$syntax%
	TapeState %Tape%.state
%$$
contains the current state of the tape.
The function
$syntax%
	TapeState %Tape%.State(void) const
%$$
can be used to access this value.

$head Rec$$
the $xref/TapeRec/$$ object $syntax%%Tape%.Rec%$$ contains
the currently recorded information.
This information is recorded using the following functions:

$subhead Empty OpCode$$
The procedure call
$syntax%
	void %Tape%.RecordNonOp()
%$$
places a NonOp in the next tape location.
This is useful for operations that must reserve extra
calculation space for forward and backward modes.

$subhead Printing OpCode$$
The procedure call
$syntax%
	void %Tape%.RecordPripOp(const char *%text%, const %Base% &%x%)
%$$
places, in the next tape location,
a PripOp that prints the parameter
value $italic x$$ to standard output.
The procedure call
$syntax%
	void %Tape%.RecordPrivOp(const char *%text%, %x_taddr%)
%$$
places, in the next tape location,
a PrivOp that prints the variable
corresponding to $italic x_taddr$$ to standard output.
These operators enables the user to determine the value of intermediate
variables during forward and reverse mode.


$subhead Parameter$$
The procedure call
$syntax%
	size_t %Tape%.RecordParOp(const %Base% &%z%)
%$$
creates a $code ParOp$$ record with the value 
specified by $italic z$$.
The return value is the taddr of this operation in the tape.

$subhead Independent$$
The procedure call
$syntax%
	void %Tape%.RecordInvOp(AD<%Base%> &%z%)
%$$
creates a tape record corresponding to a new independent variable.
The field $syntax%%z%.value%$$ is an input and all the other
fields of $italic z$$ are outputs.
Upon return from $code RecordInvOp$$, 
$italic z$$ is in the list of variables and
$syntax%%z%.taddr%$$ 
is the taddr of the new tape record. 
$pre

$$
The tape state must be Empty.
On input, the value $syntax%%z%.taddr%$$ must be zero. 

$subhead Loading Vector Element$$
The procedure call
$syntax%
	void %Tape%.RecordLoadOp(
		OpCode      %op%,
		AD<%Base%> &%z%,
		size_t    %offset%,
		size_t    %x_taddr%
	)
%$$
creates a tape record corresponding to the value of a $code VecAD$$ element.
$syntax%

%op%
%$$
Must be one of the following values:
$code LdvOp$$, $code LdpOp$$.
$syntax%

%offset%
%$$
is the offset where this $code VecAD$$ array
starts in the cumulative array containing all the $code VecAD$$ arrays.
It indexes the length of this $code VecAD$$ array 
and the rest of the array follows.
$syntax%

%x_taddr%
%$$
provides the information necessary to retriever the taddr in for this 
element within the $code VecAD$$ array.
This has the following meaning depending on the value of $italic op$$:
$table
$bold op$$    
	$cnext $bold x_taddr$$ $rnext
$code LdpOp$$ 
	$cnext location of the index in $syntax%%Rec%.GetPar%$$ $rnext
$code LdvOp$$ 
	$cnext location of the taddr as a variable in the tape
$tend
$syntax%

%z%.taddr
%$$
is modified so that it corresponds to the new tape record when 
$code RecordLoadOp$$ returns.
Upon return from $code RecordLoadOp$$, 
$italic z$$ is in the list of variables and
$syntax%%z%.taddr%$$ 
is the taddr in the tape for this $italic op$$ operator. 


$subhead Storing Vector Element$$
The procedure call
$syntax%
	void %Tape%.RecordStoreOp(
		OpCode      %op%,
		size_t    %offset%,
		size_t    %x_taddr%,
		size_t    %y_taddr%
	)
%$$
creates a tape record corresponding to storing a new value for
a $code VecAD$$ element.
$syntax%

%op%
%$$
Must be one of the following values:
$code StvvOp$$, $code StpvOp$$.
$syntax%

%offset%
%$$
is the offset where this $code VecAD$$ array
starts in the cumulative array containing all the $code VecAD$$ arrays.
It indexes the length of this $code VecAD$$ array 
and the rest of the array follows.
$syntax%

%x_taddr%
%$$
provides the information necessary to retrieve the taddr for this 
$code VecAD$$ element within this $code VecAD$$ array.
This has the following meaning depending on the value of $italic op$$:
$table
$bold op$$    
	$cnext $bold x_taddr$$ $rnext
$code StppOp$$
	$cnext location of the index in $syntax%%Rec%.GetPar%$$ $rnext
$code StpvOp$$
	$cnext location of the index in $syntax%%Rec%.GetPar%$$ $rnext
$code StvpOp$$ 
	$cnext location of the taddr as a variable in the tape  $rnext
$code StvvOp$$ 
	$cnext location of the taddr as a variable in the tape 
$tend
$syntax%

%y_taddr%
%$$
provides the information necessary to retrieve the value for this 
$code VecAD$$ element within this $code VecAD$$ array.
This has the following meaning depending on the value of $italic op$$:
$table
$bold op$$    
	$cnext $bold y_taddr$$ $rnext
$code StppOp$$
	$cnext location of the value in $syntax%%Rec%.GetPar%$$ $rnext
$code StvpOp$$
	$cnext location of the value in $syntax%%Rec%.GetPar%$$ $rnext
$code StpvOp$$ 
	$cnext location of the taddr as a variable in the tape  $rnext
$code StvvOp$$ 
	$cnext location of the taddr as a variable in the tape  
$tend

$subhead Op(Variable, Variable)$$
The procedure call 
$syntax%
inline void %Tape%.RecordOp(
	OpCode           %op%,
	AD<%Base%>       &%z%,
	size_t      %x_taddr%,
	size_t      %y_taddr%
)%$$
places a new dependent variable $italic z$$ in the tape
and sets $syntax%%z%.taddr%$$ to the corresponding taddr.
The tape record specifies the operation
$syntax%
	%z% = %op%(%x%, %y%)
%$$ 
where $italic x_taddr$$ is the taddr of $italic x$$ in the tape
and $italic y_taddr$$ is the taddr of $italic y$$ in the tape
(neither of these indices can be zero).
The field $syntax%%z%.value%$$ is an input and all the other
fields of $italic z$$ are outputs.
Upon return from $code RecordOp$$, 
$italic z$$ is in the list of variables and
$syntax%%z%.taddr%$$ 
is to the taddr of the new tape record. 
$pre

$$
The procedure call 
$syntax%
inline void %Tape%.RecordOp(
	OpCode           %op%,
	size_t      %x_taddr%,
	size_t      %y_taddr%
)%$$
is the same as above except that no variable results from the 
tape operation; i.e., $syntax%NumVar(%op%)%$$ is zero.

$subhead Op(Variable, Parameter)$$
The procedure call 
$syntax%
inline void %Tape%.RecordOp(
	OpCode           %op%,
	AD<%Base%>       &%z%,
	size_t      %x_taddr%,
	const %Base%     &%p% 
)%$$
places a new dependent variable $italic z$$ in the tape
and sets $syntax%%z%.taddr%$$ to the corresponding taddr.
The tape record specifies the operation
$syntax%
	%z% = %op%(%x%, %p%)
%$$ 
where $italic x_taddr$$ is the taddr of $italic x$$ in the tape
(this taddr can not be zero).
The field $syntax%%z%.value%$$ is an input and all the other
fields of $italic z$$ are outputs.
Upon return from $code RecordOp$$, 
$italic z$$ is in the list of variables and
$syntax%%z%.taddr%$$ 
is to the taddr of the new tape record. 
The value $italic p$$ corresponds to a parameter.
$pre

$$
The procedure call 
$syntax%
inline void %Tape%.RecordOp(
	OpCode           %op%,
	size_t      %x_taddr%,
	const %Base%     &%p% 
)%$$
is the same as above except that no variable results from the 
tape operation; i.e., $syntax%NumVar(%op%)%$$ is zero.

$subhead Op(Parameter, Variable)$$
The procedure call 
$syntax%
inline void %Tape%.RecordOp(
	OpCode           %op%,
	AD<%Base%>       &%z%,
	const %Base%     &%p%,
	size_t      %y_taddr%
)%$$
places a new dependent variable $italic z$$ in the tape
and sets $syntax%%z%.taddr%$$ to the corresponding taddr.
The tape record specifies the operation
$syntax%
	%z% = %op%(%p%, %y%)
%$$ 
where $italic y_taddr$$ is the taddr of $italic y$$ in the tape
(this taddr can not be zero).
The field $syntax%%z%.value%$$ is an input and all the other
fields of $italic z$$ are outputs.
Upon return from $code RecordOp$$, 
$italic z$$ is in the list of variables and
$syntax%%z%.taddr%$$ 
is to the taddr of the new tape record. 
The value $italic p$$ corresponds to a parameter.
$pre

$$
The procedure call 
$syntax%
inline void %Tape%.RecordOp(
	OpCode           %op%,
	const %Base%     &%p%,
	size_t      %y_taddr%
)%$$
is the same as above except that no variable results from the 
tape operation; i.e., $syntax%NumVar(%op%)%$$ is zero.

$subhead Op(Parameter, Parameter)$$
The procedure call
$syntax%
inline void %Tape%.RecordOp(
	OpCode           %op%,
	const %Base%     &%x%,
	const %Base%     &%y%
)%$$
records an operation between two parameters where
$italic x$$ is the left operand and $italic y$$ is the right operand.
No variable results from the tape operation; 
i.e., $syntax%NumVar(%op%)%$$ is zero.

$subhead Op(Variable)$$
The procedure call 
$syntax%
inline void %Tape%.RecordOp(
	OpCode           %op%,
	AD<%Base%>       &%z%,
	size_t      %x_taddr%
)%$$
places a new dependent variable $italic z$$ in the tape
and sets $syntax%%z%.taddr%$$ to the corresponding taddr.
The tape record specifies the operation
$syntax%
	%z% = %op%(%x%)
%$$ 
where $italic x_taddr$$ is the taddr of $italic x$$ in the tape
(this taddr can not be zero) and $italic op$$ specifies the
unary function.
The field $syntax%%z%.value%$$ is an input and all the other
fields of $italic z$$ are outputs.
Upon return from $code RecordOp$$, 
$italic z$$ is in the list of variables and
$syntax%%z%.taddr%$$ 
is to the taddr of the new tape record. 

$subhead User Defined Functions$$
The procedure call 
$syntax%
void %Tape%.RecordDisOp(
	AD<%Base%>       &%z%,
	size_t      %x_taddr%,
	size_t      %y_taddr%
)%$$
places a new dependent variable $italic z$$ in the tape
and sets $syntax%%z%.taddr%$$ to the corresponding taddr.
The tape record specifies the operation
$syntax%
	%z% = %f%(%x%)%
%$$ 
where $italic x_taddr$$ is the taddr of $italic x$$ in the tape
and $italic y_taddr$$ is the taddr corresponding to the 
Discrete function $italic f$$
(the value $italic x_taddr$$ cannot be zero).
The field $syntax%%z%.value%$$ is an input and all the other
fields of $italic z$$ are outputs.
Upon return from $code RecordDisOp$$, 
$italic z$$ is in the list of variables and
$syntax%%z%.taddr%$$ 
is to the taddr of the new tape record. 

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
the elements of $italic data$$ are stored in $italic Rec$$
in the following way:
for $latex j = 0 , \ldots , length-1$$,
$syntax%
	%data%[%j%] == %Rec%.GetVecInd(%i% + %j% + 1)
%$$


$head Erase$$
$index Erase$$
The operation 
$syntax%
	void %Tape%.Erase(void)
%$$
erases all the information stored in the CppAD tape.
After this operation the tape is in the Empty state.
When the tape is in the Empty state,
all $syntax%AD<%Type>%$$ objects are parameters (not variables). 
This is the initial state of the tape and an $code Erase$$
is often preformed as soon as one is done with 
a CppAD function that is stored in the tape.

$end
*/

//  BEGIN CppAD namespace
namespace CppAD {


// declare outside class so can be used by AD class
enum TapeState {
	Empty,
	Recording
};

template <class Base>
class ADTape {

	// classes
	friend class AD<Base>;
	friend class ADFun<Base>;
	friend class ADDiscrete<Base>;
	friend class VecAD<Base>;
	friend class VecAD_reference<Base>;

	// functions
	friend void PrintFor <Base>
		(const char *text, const AD<Base> &x);

	friend AD<Base> CondExpOp <Base> (
		enum CompareOp  cop          ,
		const AD<Base> &left         , 
		const AD<Base> &right        , 
		const AD<Base> &trueCase     , 
		const AD<Base> &falseCase 
	);

	friend bool Parameter     <Base> 
		(const AD<Base> &u);
	friend bool Variable      <Base> 
		(const AD<Base> &u);

public:
	// constructor
	ADTape(void) 
	{	state  = Empty; }

	// destructor
	~ADTape(void)
	{ }

	enum TapeState State(void) const
	{	return state; }

	// public function only used by CppAD::Independent
	template <typename VectorADBase>
	void Independent(VectorADBase &u);

private:
	// private data
	TapeState                 state;
	size_t         size_independent;
	TapeRec<Base>               Rec;

	/*
	Private functions
	*/
	// Identifier for the current tape
	static size_t *Id(void)
	{	// tape id is always greater than zero
		static size_t id = 1;
		return &id;
	}

	// add an empty operator at next tape location
	void RecordNonOp(void);

	// add a parameter to the tape
	size_t RecordParOp(const Base &x);
	
	// add tape entry corresponding to a parameter value
	void RecordInvOp(AD<Base> &z);

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

	// load ADVec element 
	void RecordLoadOp( 
		OpCode         op,
		AD<Base>       &z, 
		size_t     offset,
		size_t    x_taddr
	);

	// store ADVec element 
	void RecordStoreOp( 
		OpCode         op,
		size_t     offset,
		size_t    x_taddr,
		size_t    y_taddr
	);

	// add a tape entry specified by operator
	inline void RecordOp( 
		OpCode         op, 
		AD<Base>       &z, 
		size_t    x_taddr,
		size_t    y_taddr
	);
	inline void RecordOp( 
		OpCode         op, 
		size_t    x_taddr,
		size_t    y_taddr
	);
	inline void RecordOp( 
		OpCode         op, 
		AD<Base>       &z, 
		size_t    x_taddr,
		const Base     &y
	);
	inline void RecordOp( 
		OpCode         op, 
		size_t    x_taddr,
		const Base     &y
	);
	inline void RecordOp( 
		OpCode         op, 
		AD<Base>       &z, 
		const Base     &x,
		size_t    y_taddr
	);
	inline void RecordOp( 
		OpCode         op, 
		const Base     &x,
		size_t    y_taddr
	);
	inline void RecordOp( 
		OpCode         op, 
		const Base     &x,
		const Base     &y
	);
	inline void RecordOp( 
		OpCode         op, 
		AD<Base>       &z, 
		size_t    x_taddr
	);
	void RecordDisOp( 
		AD<Base>       &z, 
		size_t    x_taddr,
		size_t    y_taddr
	);
	void RecordPripOp(
		const char  *text,
		const Base     &x
	);
	void RecordPrivOp(
		const char  *text,
		size_t    x_taddr
	);
	size_t AddVec(
		size_t        length,
		const Base   *data
	);

	// erase the tape
	void Erase(void);
};
// ---------------------------------------------------------------------------
// Private functions
//
template  <class Base>
void ADTape<Base>::RecordNonOp(void)
{
	CppADUnknownError( NumInd(NonOp) == 0 );

	// Op value for this instruction
	Rec.PutOp(NonOp);

	// no Ind values for this instruction
	CppADUnknownError( NumInd(NonOp) == 0 );
}

template <class Base>
size_t ADTape<Base>::RecordParOp(const Base &z)
{	size_t z_taddr;
	size_t ind;

	CppADUnknownError( NumInd(ParOp) == 1 );
	z_taddr = Rec.PutOp(ParOp);
	ind     = Rec.PutPar(z);
	Rec.PutInd(ind);

	return z_taddr;
}

template  <class Base>
void ADTape<Base>::RecordInvOp(AD<Base> &z)
{
	size_t z_taddr;

	// in the independent variable case, should not already be in tape
	CppADUnknownError( Parameter(z) );
	CppADUnknownError( state == Empty );


	// Make z correspond to a next variable in tape
	z_taddr = Rec.PutOp(InvOp);
	z.id    = *Id();
	z.taddr = z_taddr;

	// no Ind values for this instruction
	CppADUnknownError( NumInd(InvOp) == 0 );

	// check that z is an independent variable
	CppADUnknownError( Variable(z) );
}


template  <class Base>
void ADTape<Base>::RecordLoadOp(
	OpCode         op,
	AD<Base>       &z,
	size_t     offset,
	size_t     x_taddr
)
{	size_t z_taddr;

	CppADUnknownError( (op == LdvOp) | (op == LdpOp) );
	CppADUnknownError( state == Recording );
	CppADUnknownError( NumInd(op) == 3 );

	// Make z correspond to a next variable in tape
	z_taddr = Rec.PutOp(op);
	z.id    = *Id();
	z.taddr = z_taddr;

	// Ind values for this instruction
	// (space reserved by third taddr is set by f.Forward(0, *) )
	Rec.PutInd(offset, x_taddr, 0);

	// check that z is a dependent variable
	CppADUnknownError( Variable(z) );
}

template  <class Base>
void ADTape<Base>::RecordStoreOp(
	OpCode         op,
	size_t     offset,
	size_t    x_taddr,
	size_t    y_taddr
)
{
	CppADUnknownError( 
		(op == StppOp) | 
		(op == StvpOp) | 
		(op == StpvOp) | 
		(op == StvvOp) 
	);
	CppADUnknownError( state == Recording );
	CppADUnknownError( NumInd(op) == 3 );
	CppADUnknownError( NumVar(op) == 0 );
	CppADUnknownError( (op==StppOp) | (op==StpvOp) | (x_taddr!=0) );
	CppADUnknownError( (op==StppOp) | (op==StvpOp) | (y_taddr!=0) );

	// Put operator in the tape
	Rec.PutOp(op);

	// Ind values for this instruction
	Rec.PutInd(offset, x_taddr, y_taddr);
}


template  <class Base>
inline void ADTape<Base>::RecordOp(
	OpCode         op,
	AD<Base>       &z,
	size_t    x_taddr,
	size_t    y_taddr
)
{
	size_t z_taddr;

	CppADUnknownError( state == Recording );
	CppADUnknownError( (x_taddr != 0) & (y_taddr != 0) );
	CppADUnknownError( (op != InvOp) & (op != DisOp) );
	CppADUnknownError( NumInd(op) == 2 );


	// Make z correspond to a next variable in tape
	z_taddr = Rec.PutOp(op);
	z.id    = *Id();
	z.taddr = z_taddr;

	// Ind values for this instruction
	Rec.PutInd(x_taddr, y_taddr);

	// check that z is a dependent variable
	CppADUnknownError( Variable(z) );
}

template  <class Base>
inline void ADTape<Base>::RecordOp(
	OpCode         op,
	size_t    x_taddr,
	size_t    y_taddr
)
{

	CppADUnknownError( state == Recording );
	CppADUnknownError( (x_taddr != 0) & (y_taddr != 0) );
	CppADUnknownError( (op != InvOp) & (op != DisOp) );
	CppADUnknownError( NumInd(op) == 2 );
	CppADUnknownError( NumVar(op) == 0 );

	// record operator
	Rec.PutOp(op);

	// Ind values for this instruction
	Rec.PutInd(x_taddr, y_taddr);
}

template  <class Base>
inline void ADTape<Base>::RecordOp(
	OpCode           op,
	AD<Base>         &z,
	size_t      x_taddr,
	const Base       &y
)
{
	size_t z_taddr;

	CppADUnknownError( state == Recording );
	CppADUnknownError( x_taddr != 0 );
	CppADUnknownError( (op != InvOp) & (op != DisOp) );
	CppADUnknownError( NumInd(op) == 2 );


	// Make z correspond to a next variable in tape
	z_taddr = Rec.PutOp(op);
	z.id    = *Id();
	z.taddr = z_taddr;

	// Ind values for this instruction
	Rec.PutInd(x_taddr, Rec.PutPar(y));

	// check that z is a dependent variable
	CppADUnknownError( Variable(z) );
}

template  <class Base>
inline void ADTape<Base>::RecordOp(
	OpCode           op,
	size_t      x_taddr,
	const Base       &y
)
{

	CppADUnknownError( state == Recording );
	CppADUnknownError( x_taddr != 0 );
	CppADUnknownError( (op != InvOp) & (op != DisOp) );
	CppADUnknownError( NumInd(op) == 2 );
	CppADUnknownError( NumVar(op) == 0 );

	// record this operation
	Rec.PutOp(op);

	// Ind values for this instruction
	Rec.PutInd(x_taddr, Rec.PutPar(y));
}

template  <class Base>
inline void ADTape<Base>::RecordOp(
	OpCode           op,
	AD<Base>         &z,
	const Base       &x,
	size_t      y_taddr
)
{
	size_t z_taddr;

	CppADUnknownError( state == Recording );
	CppADUnknownError( y_taddr != 0 );
	CppADUnknownError( (op != InvOp) & (op != DisOp) );
	CppADUnknownError( NumInd(op) == 2 );


	// Make z correspond to a next variable in tape
	z_taddr = Rec.PutOp(op);
	z.id    = *Id();
	z.taddr = z_taddr;

	// Ind values for this instruction
	Rec.PutInd(Rec.PutPar(x), y_taddr);

	// check that z is a dependent variable
	CppADUnknownError( Variable(z) );
}

template  <class Base>
inline void ADTape<Base>::RecordOp(
	OpCode           op,
	const Base       &x,
	size_t      y_taddr
)
{

	CppADUnknownError( state == Recording );
	CppADUnknownError( y_taddr != 0 );
	CppADUnknownError( (op != InvOp) & (op != DisOp) );
	CppADUnknownError( NumInd(op) == 2 );
	CppADUnknownError( NumVar(op) == 0 );

	// record this operation
	Rec.PutOp(op);

	// Ind values for this instruction
	Rec.PutInd(Rec.PutPar(x), y_taddr);
}

template  <class Base>
inline void ADTape<Base>::RecordOp(
	OpCode           op,
	const Base       &x,
	const Base       &y
)
{

	CppADUnknownError( state == Recording );
	CppADUnknownError( (op != InvOp) & (op != DisOp) );
	CppADUnknownError( NumInd(op) == 2 );
	CppADUnknownError( NumVar(op) == 0 );

	// record this operation
	Rec.PutOp(op);

	// Ind values for this instruction
	Rec.PutInd(Rec.PutPar(x), Rec.PutPar(y));
}

template  <class Base>
inline void ADTape<Base>::RecordOp(
	OpCode           op,
	AD<Base>         &z,
	size_t      x_taddr
)
{
	size_t z_taddr;

	CppADUnknownError( state == Recording );
	CppADUnknownError( x_taddr != 0 );
	CppADUnknownError( (op != InvOp) & (op != DisOp) );
	CppADUnknownError( NumInd(op) == 1 );


	// Make z correspond to a next variable in tape
	z_taddr = Rec.PutOp(op);
	z.id    = *Id();
	z.taddr = z_taddr;

	// Ind value for this instruction
	Rec.PutInd(x_taddr);

	// check that z is a dependent variable
	CppADUnknownError( Variable(z) );
}

template  <class Base>
void ADTape<Base>::RecordDisOp(
	AD<Base>         &z,
	size_t      x_taddr,
	size_t      y_taddr
)
{
	size_t z_taddr;

	CppADUnknownError( state == Recording );
	CppADUnknownError( x_taddr != 0 );
	CppADUnknownError( NumInd(DisOp) == 2 );

	// Make z correspond to a next variable in tape
	z_taddr = Rec.PutOp(DisOp);
	z.id    = *Id();
	z.taddr = z_taddr;

	// Ind values for this instruction
	Rec.PutInd(x_taddr, y_taddr);

	// check that z is a dependent variable
	CppADUnknownError( Variable(z) );
}

template <class Base>
void ADTape<Base>::RecordPripOp(const char *text, const Base &x)
{

	CppADUnknownError( state == Recording );
	CppADUnknownError( NumInd(PripOp) == 2 );

	// put this operator in the tape
	Rec.PutOp(PripOp);

	// Ind values for this instruction 
	Rec.PutInd(Rec.PutTxt(text), Rec.PutPar(x));
}

template <class Base>
void ADTape<Base>::RecordPrivOp(const char *text, size_t x_taddr)
{

	CppADUnknownError( state == Recording );
	CppADUnknownError( NumInd(PripOp) == 2 );

	// put this operator in the tape
	Rec.PutOp(PrivOp);

	// Ind values for this instruction 
	Rec.PutInd(Rec.PutTxt(text), x_taddr);
}


template <class Base>
size_t ADTape<Base>::AddVec(size_t length, const Base *data)
{	CppADUnknownError( length > 0 );
	size_t i;
	size_t vecInd;

	// store the length in VecInd
	size_t start = Rec.PutVecInd(length);

	// store indices of the values in VecInd 
	for(i = 0; i < length; i++)
	{
		vecInd = Rec.PutPar( data[i] );
		Rec.PutVecInd( vecInd );
	}
 
	// return the taddr of the length (where the vector starts)
	return start;
}

template <class Base>
void ADTape<Base>::Erase(void)
{	// make all the existing AD objects parmaeters
	*Id() += 1;
	CppADUnknownError( *Id() > 0 );

	// change the state of the tape
	state = Empty;

	// erase the memory stored in Rec structure
	Rec.Erase();

	return;
}


} // END CppAD namespace

# endif
