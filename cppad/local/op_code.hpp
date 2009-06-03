/* $Id$ */
# ifndef CPPAD_OP_CODE_INCLUDED
# define CPPAD_OP_CODE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*! 
\file op_code.hpp
\brief Defines the OpCode enum type and functions related to it.

*/

namespace CppAD {

/*!
\brief Type used to distinguish different AD<Base> atomic operations.

Each of the operators ends with the characters Op. Ignoring the Op at the end,
the operators appear in alphabetical order. Binary operation where both
operands have type AD<Base> use the following convention for thier endings:
\verbatim
    Ending  Left-Operand  Right-Operand
      pvOp     parameter       variable  
      vpOp      variable      parameter  
      vvOp      variable       variable  
\endverbatim
For example, AddpvOp represents the addition operator where the left
operand is a parameter and the right operand is a variable.
*/
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

# include <string>
# include <sstream>
# include <iomanip>

namespace CppAD {

/*!
Table containing number of indices for the corresponding operator.

The i-th element in this table specifes the number of indices stored for each
occurance of the operator that is the i-th value in the OpCode enum type.
For example, for the first three OpCode enum values we have
\verbatim
OpCode   j   NumIndTable[j]  Meaning
AbsOp    0                1  index of variable we are taking absolute value of
AcosOp   1                1  index of variable we are taking cosine of
AddpvOp  1                2  indices of parameter and variable we are adding
\endverbatim
Note that the meaning of the indices depends on the operator.
*/
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

/*!
Returns the number of indices for the specified operator.

\param op 
Operator for which we are fetching the number of indices.
*/
inline size_t NumInd( OpCode op)
{
	CPPAD_ASSERT_UNKNOWN( size_t(SubvvOp) == 
		sizeof(NumIndTable) / sizeof(NumIndTable[0]) - 1
	);
	CPPAD_ASSERT_UNKNOWN( size_t(op) <= size_t(SubvvOp) );

	return NumIndTable[(size_t) op];
}

/*!
Table containing number of variables resulting from the corresponding operation.

The i-th element in this table specifes the number of varibles for each
occurance of the operator that is the i-th value in the OpCode enum type.
For example, for the first three OpCode enum values we have
\verbatim
OpCode   j   NumVarTable[j]  Meaning
AbsOp    0                1  variable that is the result of the absolute value
AcosOp   1                2  acos(x) and sqrt(1-x*x) are required for this op
AddpvOp  1                1  variable that is the result of the addition
\endverbatim
*/
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

/*!
Returns the number of variables resulting from the specified operation.

\param op 
Operator for which we are fetching the number of indices.
*/
inline size_t NumVar(OpCode op)
{	// check ensuring conversion to size_t is as expected
	CPPAD_ASSERT_UNKNOWN( size_t(SubvvOp) == 
		sizeof(NumVarTable) / sizeof(NumVarTable[0]) - 2
	);
	// this test ensures that all indices are within the table
	CPPAD_ASSERT_UNKNOWN( size_t(op) <= size_t(SubvvOp) );

	return NumVarTable[(size_t) op];
}

/*!
Prints a single field corresponding to an operator.

A specified leader is printed in front of the value
and then the value is left justified in the following width character.

\tparm Type
is the type of the value we are printing.

\param os
is the stream that we are printing to.

\param leader
are characters printed before the value.

\param value
is the value being printed.

\param width
is the number of character to print the value in.
If the value does not fit in the width, the value is replace
by width '*' characters.
*/
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

/*!
Prints a single operator, its operands, and the corresponding result values.

\tparam Base
Is the base type for these AD<Base> operations.

\tparam Value
Determines the type of the values that we are printing
(expected to be either Base or Pack where
Pack is the type used to pace sparsity patterns.

\param os
is the output stream that the information is printed on.

\param Rec
Is the entire recording for the tape that this operator is in.

\param i_var
is the index for the variable corresponding to the result of this operation
(ignored if NumVar(op) == 0).

\param op
The operator code (OpCode) for this operation.

\param ind
is the vector of indices for this operation
(must have NumInd(op) elements).

\param nfz
is the number of forward sweep calculated values of type Value
that correspond to this operation
(ignored if NumVar(op) == 0).

\param fz
points to the first forward calculated value
that correspond to this operation
(ignored if NumVar(op) == 0).

\param nrz
is the number of reverse sweep calculated values of type Value
that correspond to this operation
(ignored if NumVar(op) == 0).

\param rz
points to the first reverse calculated value
that correspond to this operation
(ignored if NumVar(op) == 0).
*/
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
