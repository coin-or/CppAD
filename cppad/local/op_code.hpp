# ifndef CPPAD_OP_CODE_INCLUDED
# define CPPAD_OP_CODE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
----------------------------------------------------------------------------
$begin OpCode$$ $comment CppAD Developer Documentation$$
$aindex subhead$$
$spell
	Addpv
	Addvp
	Addvv
	Subpv
	Subvp
	Subvv
	Mulpv
	Mulvp
	Mulvv
	Divpv
	Divvp
	Divvv
	namespace
	Powvp
	Powpv
	Powvv
	Prip
	Priv
	CExp
	Stpp
	Stvp
	Inv
	Ldv
	Ldp
	Stvp
	Stpp
	Stvv
	Stpv
	Sto
	enum
	Mul
	Div
	CondExp
	Cos
	Cosh
	Ind
	Acos
	Asin
	Atan
	Sqrt
	op
	zs
	zst
	zt
	xy
	yy
	Cpp
$$

$mindex tape operator$$
$section The Tape Operator Codes$$

$head Syntax$$
$syntax%OpCode% %op%$$ $pre
$$


$head Description$$
The value $italic op$$ is one of the following enum type values:
$codep */

namespace CppAD {

	// alphabetical order (ignoring the Op at the end)
	enum OpCode {
		AbsOp,    //  abs(variable)
		AcosOp,   // asin(variable)
		AddpvOp,  //      parameter  + variable
		AddvpOp,  //      variable   + parameter
		AddvvOp,  //      variable   + variable
		AsinOp,   // asin(variable)
		AtanOp,   // atan(variable)
		CExpOp,   // CondExp(cop, left, right, trueCase, falseCase)
		ComOp,    // Compare(cop, result, left, right)
		CosOp,    //  cos(variable)
		CoshOp,   // cosh(variable)
		DisOp,    //  dis(variable,    index)
		DivpvOp,  //      parameter  / variable
		DivvpOp,  //      variable   / parameter
		DivvvOp,  //      variable   / variable
		ExpOp,    //  exp(variable)
		LdpOp,    //    z[parameter]
		LdvOp,    //    z[variable]
		InvOp,    //                             independent variable
		LogOp,    //  log(variable)
		MulpvOp,  //      parameter  * variable
		MulvpOp,  //      variable   * parameter
		MulvvOp,  //      variable   * variable
		NonOp,    //                             space holder
		ParOp,    //      parameter
		PowvpOp,  //  pow(variable,    parameter)
		PowpvOp,  //  pow(parameter,   variable)
		PowvvOp,  //  pow(variable,    variable)
		PripOp,   //      text         parameter
		PrivOp,   //      text         parameter
		SinOp,    //  sin(variable)
		SinhOp,   // sinh(variable)
		SqrtOp,   // sqrt(variable)
		StppOp,   //    z[parameter] = parameter
		StvpOp,   //    z[variable]  = parameter
		StpvOp,   //    z[parameter] = variable
		StvvOp,   //    z[variable]  = variable
		SubpvOp,  //      parameter  - variable
		SubvpOp,  //      variable   - parameter
		SubvvOp   //      variable   - variable
	};
}
/* $$

$end
------------------------------------------------------------------------ 
$begin printOp$$ $comment CppAD Developer Documentation$$
$spell
	var
	Num
	ind
	Op
	std
	ostream
	const
	nfz
	fz
	rz
	nrz
	Taylor
$$

$index printOp$$
$index operator, print$$
$index trace, tape operation$$
$section Print the Information Corresponding to One Tape Operation$$

$head Syntax$$
$syntax%void printOp(
	std::ostream &%os% ,
	const player<%Base%> *%Rec% ,
	size_t %i_var% ,
	OpCode %op% ,
	const size_t *%ind% ,
	size_t %nfz% ,
	const %Value% *%fz% ,
	size_t %nrz% ,
	const %Value% *%rz% )%$$


$head Base$$
Determines the type of tape we are printing from.

$head Value$$
Determines the type of the values that we are printing
(expected to be either $italic Base$$ or $italic Pack$$ where
$italic Pack$$ is the type used to pace sparsity patterns).

$head os$$
is the output stream that the information is printed on.

$head Rec$$
Is the entire recording for the tape that this operator is in.

$head i_var$$
is the variable index for the result of this operation
(ignored if $syntax%NumVar(%op%) == 0%$$).

$head op$$
The operator code for this operation.

$head ind$$
is the vector of indices for this operation
(must have  $syntax%NumInd(%op%)%$$ elements).

$head nfz$$
is the number of forward calculated values of type $italic Value$$
that correspond to this operation
(ignored if $syntax%NumVar(%op%) == 0%$$).

$head fz$$
points to the first forward calculated value
that correspond to this operation
(ignored if $syntax%NumVar(%op%) == 0%$$).

$head nrz$$
is the number of reverse calculated values of type $italic Value$$
that correspond to this operation
(ignored if $syntax%NumVar(%op%) == 0%$$).

$head rz$$
points to the first reverse calculated value
that correspond to this operation
(ignored if $syntax%NumVar(%op%) == 0%$$).

$end
---------------------------------------------------------------------------
$begin NumInd$$ $comment CppAD Developer Documentation$$
$spell
	NumInd
	Op
$$

$index NumInd$$
$section Number of Ind field Values Corresponding to an Op Code$$

$head Syntax$$
$syntax%size_t NumInd(OpCode %op%)%$$


$head Description$$
The syntax
$syntax%
	size_t NumInd(operator %op%)
%$$
returns the number of Ind field values corresponding to each operator value.

$end
------------------------------------------------------------------------
$begin NumVar$$ $comment CppAD Developer Documentation$$
$spell
	NumVar
	Op
$$

$index NumVar$$
$section Number of Variables Corresponding to an Op Code$$

$head Syntax$$
$syntax%size_t NumVar(OpCode %op%)%$$


$head Description$$
The syntax
$syntax%
	size_t NumVar(operator %op%)
%$$
returns the number of variable
field values corresponding to each operator value.

$end
------------------------------------------------------------------------ */

# include <string>
# include <sstream>
# include <iomanip>

namespace CppAD {

// alphabetical order (ignoring the Op at the end)
const size_t NumIndTable[] = {
	1, // AbsOp
	1, // AcosOp
	2, // AddpvOp
	2, // AddvpOp
	2, // AddvvOp
	1, // AsinOp
	1, // AtanOp
	6, // CExpOp
	4, // ComOp
	1, // CosOp
	1, // CoshOp
	2, // DisOp
	2, // DivpvOp
	2, // DivvpOp
	2, // DivvvOp
	1, // ExpOp
	3, // LdpOp
	3, // LdvOp
	0, // InvOp
	1, // LogOp
	2, // MulvvOp
	2, // MulpvOp
	2, // MulvpOp
	0, // NonOp
	1, // ParOp
	2, // PowvpOp
	2, // PowpvOp
	2, // PowvvOp
	2, // PripOp
	2, // PrivOp
	1, // SinOp
	1, // SinhOp
	1, // SqrtOp
	3, // StppOp
	3, // StvpOp
	3, // StpvOp
	3, // StvvOp
	2, // SubpvOp
	2, // SubvpOp
	2  // SubvvOp
};

inline size_t NumInd(OpCode op)
{
	CPPAD_ASSERT_UNKNOWN( size_t(SubvvOp) == 
		sizeof(NumIndTable) / sizeof(NumIndTable[0]) - 1
	);
	CPPAD_ASSERT_UNKNOWN( size_t(op) <= size_t(SubvvOp) );

	return NumIndTable[(size_t) op];
}

// alphabetical order (ignoring the Op at the end)
const size_t NumVarTable[] = {
	1, // AbsOp
	2, // AcosOp
	1, // AddpvOp
	1, // AddvpOp
	1, // AddvvOp
	2, // AsinOp
	2, // AtanOp
	1, // CExpOp
	0, // ComOp
	2, // CosOp
	2, // CoshOp
	1, // DisOp
	1, // DivpvOp
	1, // DivvpOp
	1, // DivvvOp
	1, // ExpOp
	1, // LdpOp
	1, // LdvOp
	1, // InvOp
	1, // LogOp
	1, // MulvvOp
	1, // MulpvOp
	1, // MulvpOp
	1, // NonOp
	1, // ParOp
	3, // PowvpOp
	3, // PowpvOp
	3, // PowvvOp
	0, // PripOp
	0, // PrivOp
	2, // SinOp
	2, // SinhOp
	1, // SqrtOp
	0, // StppOp
	0, // StvpOp
	0, // StpvOp
	0, // StvvOp
	1, // SubpvOp
	1, // SubvpOp
	1, // SubvvOp
	0  // Not used: avoids warning by g++ 4.3.2 when pycppad builds
};

inline size_t NumVar(OpCode op)
{	// check ensuring conversion to size_t is as expected
	CPPAD_ASSERT_UNKNOWN( size_t(SubvvOp) == 
		sizeof(NumVarTable) / sizeof(NumVarTable[0]) - 2
	);
	// this test ensures that all indices are within the table
	CPPAD_ASSERT_UNKNOWN( size_t(op) <= size_t(SubvvOp) );

	return NumVarTable[(size_t) op];
}

template <class Type>
void printOpField(
	std::ostream      &os , 
	const char *   leader ,
	const Type     &value , 
	size_t          width )
{
	std::ostringstream buffer;
	std::string        str;

	// first print the leader
	os << leader;

	// print the value into an internal buffer
	buffer << std::setw(width) << value;
	str = buffer.str();

	// length of the string
	size_t len = str.size();
	if( len > width )
	{	size_t i;
		for(i = 0; i < width-1; i++)
			os << str[i];
		os << "*";
		return;
	}

	// count number of spaces at begining
	size_t nspace = 0; 
	while(str[nspace] == ' ' && nspace < len)
		nspace++;

	// left justify the string
	size_t i = nspace;
	while( i < len )
		os << str[i++];

	i = width - len + nspace;
	while(i--)
		os << " "; 
}

template <class Base, class Value>
void printOp(
	std::ostream          &os     , 
	const player<Base>   *Rec     ,
	size_t                 i_var  , 
	OpCode                 op     ,
	const size_t          *ind    ,
	size_t                 nfz    ,
	const  Value          *fz     ,
	size_t                 nrz    ,
	const  Value          *rz     )
{	
	static char *CompareOpName[] = { "Lt", "Le", "Eq", "Ge", "Gt", "Ne" };
	static char *OpName[] = {
		"Abs"   ,
		"Acos"  ,
		"Addpv" ,
		"Addvp" ,
		"Addvv" ,
		"Asin"  ,
		"Atan"  ,
		"CExp"  ,
		"Com"   ,
		"Cos"   ,
		"Cosh"  ,
		"DisOp" ,
		"Divpv" ,
		"Divvp" ,
		"Divvv" ,
		"Exp"   ,
		"Ldp"   ,
		"Ldv"   ,
		"Inv"   ,
		"Log"   ,
		"Mulpv" ,
		"Mulvp" ,
		"Mulvv" ,
		"Non"   ,
		"Par"   ,
		"Powvp" ,
		"Powpv" ,
		"Powvv" ,
		"Prip"  ,
		"Priv"  ,
		"Sin"   ,
		"Sinh"  ,
		"Sqrt"  ,
		"Stpp"  ,
		"Stvp"  ,
		"Stpv"  ,
		"Stvv"  ,
		"Subpv" ,
		"Subvp" ,
		"Subvv"
	};
	CPPAD_ASSERT_UNKNOWN( 
		size_t(SubvvOp) == sizeof(OpName) / sizeof(OpName[0]) - 1
	);

	// print operator
	printOpField(os,  "i=",      i_var, 5);
	if( op == CExpOp )
	{	printOpField(os, "op=", OpName[op], 4); 
		printOpField(os, "", CompareOpName[ ind[0] ], 3);
	}
	else if( op == ComOp )
	{	printOpField(os, "op=", OpName[op], 3); 
		printOpField(os, "", CompareOpName[ ind[0] ], 4);
	}
	else	printOpField(os, "op=", OpName[op], 7); 

	// print other fields
	size_t ncol = 5;
	switch( op )
	{
		case LdpOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, "  p=", *(Rec->GetPar(ind[1])), ncol);
		break;

		case LdvOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, "  v=", ind[1], ncol);
		break;

		case StppOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, " pl=", *(Rec->GetPar(ind[1])), ncol);
		printOpField(os, " pr=", *(Rec->GetPar(ind[2])), ncol);
		break;

		case StpvOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, " pl=", *(Rec->GetPar(ind[1])), ncol);
		printOpField(os, " vr=", ind[2], ncol);
		break;

		case StvpOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, " vl=", ind[1], ncol);
		printOpField(os, " pr=", *(Rec->GetPar(ind[2])), ncol);
		break;

		case StvvOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], ncol);
		printOpField(os, " vl=", ind[1], ncol);
		printOpField(os, " vr=", ind[2], ncol);
		break;

		case AddvvOp:
		case DivvvOp:
		case MulvvOp:
		case PowvvOp:
		case SubvvOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 2 );
		printOpField(os, " vl=", ind[0], ncol);
		printOpField(os, " vr=", ind[1], ncol);
		break;

		case AddpvOp:
		case SubpvOp:
		case MulpvOp:
		case PowpvOp:
		case DivpvOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 2 );
		printOpField(os, " pl=", *(Rec->GetPar(ind[0])), ncol);
		printOpField(os, " vr=", ind[1], ncol);
		break;

		case AddvpOp:
		case DivvpOp:
		case MulvpOp:
		case PowvpOp:
		case SubvpOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 2 );
		printOpField(os, " vl=", ind[0], ncol);
		printOpField(os, " pr=", *(Rec->GetPar(ind[1])), ncol);
		break;

		case AbsOp:
		case AcosOp:
		case AsinOp:
		case AtanOp:
		case CosOp:
		case CoshOp:
		case ExpOp:
		case LogOp:
		case SinOp:
		case SinhOp:
		case SqrtOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 1 );
		printOpField(os, "  v=", ind[0], ncol);
		break;

		case ParOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 1 );
		printOpField(os, "  p=", *(Rec->GetPar(ind[0])), ncol);
		break;

		case PripOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 2 );
		printOpField(os, "txt=", *(Rec->GetTxt(ind[0])), ncol);
		printOpField(os, "  p=", *(Rec->GetPar(ind[1])), ncol);
		break;

		case PrivOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 2 );
		printOpField(os, "txt=", *(Rec->GetTxt(ind[0])), ncol);
		printOpField(os, "  v=", ind[1], ncol);
		break;

		case InvOp:
		case NonOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 0 );
		break;

		case DisOp:
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 2 );
		printOpField(os, "  v=", ind[0], ncol);
		printOpField(os, "  f=", ind[1], ncol);
		break;
	

		case CExpOp:
		CPPAD_ASSERT_UNKNOWN(ind[1] != 0);
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 6 );
		if( ind[1] & 1 )
			printOpField(os, " vl=", ind[2], ncol);
		else	printOpField(os, " pl=", *(Rec->GetPar(ind[2])), ncol);
		if( ind[1] & 2 )
			printOpField(os, " vr=", ind[3], ncol);
		else	printOpField(os, " pr=", *(Rec->GetPar(ind[3])), ncol);
		if( ind[1] & 4 )
			printOpField(os, " vt=", ind[4], ncol);
		else	printOpField(os, " pt=", *(Rec->GetPar(ind[4])), ncol);
		if( ind[1] & 8 )
			printOpField(os, " vf=", ind[5], ncol);
		else	printOpField(os, " pf=", *(Rec->GetPar(ind[5])), ncol);
		break;

		case ComOp:
		CPPAD_ASSERT_UNKNOWN(ind[1] != 0);
		CPPAD_ASSERT_UNKNOWN( NumInd(op) == 4 );
		if( ind[1] & 1 )
			printOpField(os, "res=", 1, ncol);
		else	printOpField(os, "res=", 0, ncol);
		if( ind[1] & 2 )
			printOpField(os, " vl=", ind[2], ncol);
		else	printOpField(os, " pl=", *(Rec->GetPar(ind[2])), ncol);
		if( ind[1] & 4 )
			printOpField(os, " vr=", ind[3], ncol);
		else	printOpField(os, " pr=", *(Rec->GetPar(ind[3])), ncol);
		break;

		default:
		CPPAD_ASSERT_UNKNOWN(0);
	}
	size_t k;
	if( NumVar(op) > 0 )
	{ 
		for(k = 0; k < nfz; k++)
			std::cout << "| fz[" << k << "]=" << fz[k];
		for(k = 0; k < nrz; k++)
			std::cout << "| rz[" << k << "]=" << rz[k];
	}
	std::cout << std::endl;
}

} // END CppAD namespace

# endif
