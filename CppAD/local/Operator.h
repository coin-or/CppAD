# ifndef CppADOperatorIncluded
# define CppADOperatorIncluded

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
/*
----------------------------------------------------------------------------
$begin OpCode$$
$aindex subhead$$
$spell
	Ltfpp
	Lttpp
	Ltfpv
	Lttpv
	Ltfvp
	Lttvp
	Ltfvv
	Lttvv

	Lefpp
	Letpp
	Lefpv
	Letpv
	Lefvp
	Letvp
	Lefvv
	Letvv

	Eqfpp
	Eqtpp
	Eqfpv
	Eqtpv
	Eqfvp
	Eqtvp
	Eqfvv
	Eqtvv

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

$table
$bold Syntax$$ 
$cnext $syntax%OpCode%  %op%$$   $rnext
$tend

$fend 20$$

$head Description$$
The value $italic op$$ is one of the following enum type values:
$codep */

namespace CppAD {
	enum OpCode {
		AbsOp,    //  abs(variable)
		AcosOp,   // asin(variable)
		AddpvOp,  //      parameter  + variable
		AddvpOp,  //      variable   + parameter
		AddvvOp,  //      variable   + variable
		AsinOp,   // asin(variable)
		AtanOp,   // atan(variable)
		CExpOp,   // CondExp(cop, left, right, trueCase, falseCase)
		CosOp,    //  cos(variable)
		DisOp,    //  dis(variable,    index)
		DivpvOp,  //      parameter  / variable
		DivvpOp,  //      variable   / parameter
		DivvvOp,  //      variable   / variable
		EqfppOp,  //      parameter == parameter if false
		EqtppOp,  //      parameter == parameter if true
		EqfpvOp,  //      parameter == variable  is false
		EqtpvOp,  //      parameter == variable  is true
		EqfvpOp,  //      variable  == parameter is false
		EqtvpOp,  //      variable  == parameter is true
		EqfvvOp,  //      variable  == variable  is false
		EqtvvOp,  //      variable  == variable  is true
		ExpOp,    //  exp(variable)
		LdpOp,    //    z[parameter]
		LdvOp,    //    z[variable]
		InvOp,    //                             independent variable
		LefppOp,  //      parameter <= parameter is false
		LetppOp,  //      parameter <= parameter is true
		LefpvOp,  //      parameter <= variable  is false
		LetpvOp,  //      parameter <= variable  is true
		LefvpOp,  //      variable  <= parameter is false
		LetvpOp,  //      variable  <= parameter is true
		LefvvOp,  //      variable  <= variable  is false
		LetvvOp,  //      variable  <= variable  is true
		LogOp,    //  log(variable)
		LtfppOp,  //      parameter < parameter is false
		LttppOp,  //      parameter < parameter is true
		LtfpvOp,  //      parameter < variable  is false
		LttpvOp,  //      parameter < variable  is true
		LtfvpOp,  //      variable  < parameter is false
		LttvpOp,  //      variable  < parameter is true
		LtfvvOp,  //      variable  < variable  is false
		LttvvOp,  //      variable  < variable  is true
		MulpvOp,  //      parameter  * variable
		MulvpOp,  //      variable   * parameter
		MulvvOp,  //      variable   * variable
		NonOp,    //                             space holder
		ParOp,    //      parameter
		PripOp,   //      text         parameter
		PrivOp,   //      text         parameter
		SinOp,    //  sin(variable)
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
$begin printOp$$
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

$table
$bold Syntax$$ $cnext 
$syntax%void printOp(
	std::ostream          &%os%     , 
	const TapeRec<%Base%>   *%Rec%    ,   
	size_t                 %i_var%  , 
	OpCode                 %op%     ,
	const size_t          *%ind%    , 
	size_t                 %nfz%    ,
	const  %Base%           *%fz%     ,
	size_t                 %nrz%    ,
	const  %Base%           *%rz%     )%$$
$tend

$fend 25$$

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
is the number of forward calculated Taylor coefficients 
that correspond to this operation
(ignored if $syntax%NumVar(%op%) == 0%$$).

$head fz$$
points to the first forward calculated Taylor coefficient 
that correspond to this operation
(ignored if $syntax%NumVar(%op%) == 0%$$).

$head nrz$$
is the number of reverse calculated Taylor coefficient derivatives
that correspond to this operation
(ignored if $syntax%NumVar(%op%) == 0%$$).

$head rz$$
points to the first reverse calculated Taylor coefficient derivative
that correspond to this operation
(ignored if $syntax%NumVar(%op%) == 0%$$).

$end
---------------------------------------------------------------------------
$begin NumInd$$
$spell
	NumInd
	Op
$$

$index NumInd$$
$section Number of Ind field Values Corresponding to an Op Code$$

$table
$bold Syntax$$
$cnext $syntax%size_t NumInd(OpCode %op%)%$$
$tend

$fend 25$$

$head Description$$
The syntax
$syntax%
	size_t NumInd(operator %op%)
%$$
returns the number of Ind field values corresponding to each operator value.

$end
------------------------------------------------------------------------
$begin NumVar$$
$spell
	NumVar
	Op
$$

$index NumVar$$
$section Number of Variables Corresponding to an Op Code$$

$table
$bold Syntax$$
$cnext $syntax%size_t NumVar(OpCode %op%)%$$
$tend

$fend 25$$

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

const size_t NumIndTable[] = {
	1, // AbsOp
	1, // AcosOp
	2, // AddpvOp
	2, // AddvpOp
	2, // AddvvOp
	1, // AsinOp
	1, // AtanOp
	6, // CEpppOp
	1, // CosOp
	2, // DisOp
	2, // DivpvOp
	2, // DivvpOp
	2, // DivvvOp
	2, // EqfppOp
	2, // EqtppOp
	2, // EqfpvOp
	2, // EqtpvOp
	2, // EqfvpOp
	2, // EqtvpOp
	2, // EqfvvOp
	2, // EqtvvOp
	1, // ExpOp
	3, // LdpOp
	3, // LdvOp
	0, // InvOp
	2, // LefppOp
	2, // LetppOp
	2, // LefpvOp
	2, // LetpvOp
	2, // LefvpOp
	2, // LetvpOp
	2, // LefvvOp
	2, // LetvvOp
	1, // LogOp
	2, // LtfppOp
	2, // LttppOp
	2, // LtfpvOp
	2, // LttpvOp
	2, // LtfvpOp
	2, // LttvpOp
	2, // LtfvvOp
	2, // LttvvOp
	2, // MulvvOp
	2, // MulpvOp
	2, // MulvpOp
	0, // NonOp
	1, // ParOp
	2, // PripOp
	2, // PrivOp
	1, // SinOp
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
	CppADUnknownError( size_t(SubvvOp) == 
		sizeof(NumIndTable) / sizeof(NumIndTable[0]) - 1
	);
	CppADUnknownError( size_t(op) <= size_t(SubvvOp) );

	return NumIndTable[(size_t) op];
}

const size_t NumVarTable[] = {
	1, // AbsOp
	2, // AcosOp
	1, // AddpvOp
	1, // AddvpOp
	1, // AddvvOp
	2, // AsinOp
	2, // AtanOp
	1, // CExpOp
	2, // CosOp
	1, // DisOp
	1, // DivpvOp
	1, // DivvpOp
	1, // DivvvOp
	0, // EqfppOp
	0, // EqtppOp
	0, // EqfpvOp
	0, // EqtpvOp
	0, // EqfvpOp
	0, // EqtvpOp
	0, // EqfvvOp
	0, // EqtvvOp
	1, // ExpOp
	1, // LdpOp
	1, // LdvOp
	1, // InvOp
	0, // LefppOp
	0, // LetppOp
	0, // LefpvOp
	0, // LetpvOp
	0, // LefvpOp
	0, // LetvpOp
	0, // LefvvOp
	0, // LetvvOp
	1, // LogOp
	0, // LtfppOp
	0, // LttppOp
	0, // LtfpvOp
	0, // LttpvOp
	0, // LtfvpOp
	0, // LttvpOp
	0, // LtfvvOp
	0, // LttvvOp
	1, // MulvvOp
	1, // MulpvOp
	1, // MulvpOp
	1, // NonOp
	1, // ParOp
	1, // PripOp
	1, // PrivOp
	2, // SinOp
	1, // SqrtOp
	1, // StppOp
	1, // StvpOp
	1, // StpvOp
	1, // StvvOp
	1, // SubpvOp
	1, // SubvpOp
	1  // SubvvOp
};

inline size_t NumVar(OpCode op)
{
	CppADUnknownError( size_t(SubvvOp) == 
		sizeof(NumVarTable) / sizeof(NumVarTable[0]) - 1
	);
	CppADUnknownError( size_t(op) <= size_t(SubvvOp) );

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
		for(i = 0; i < width; i++)
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

template <class Base>
void printOp(
	std::ostream          &os     , 
	const TapeRec<Base>   *Rec    ,
	size_t                 i_var  , 
	OpCode                 op     ,
	const size_t          *ind    ,
	size_t                 nfz    ,
	const  Base           *fz     ,
	size_t                 nrz    ,
	const  Base           *rz     )
{	
	static char *CompareOpName[] = { "Lt", "Le", "Eq", "Ge", "Gt" };
	static char *OpName[] = {
		"Abs"   ,
		"Acos"  ,
		"Addpv" ,
		"Addvp" ,
		"Addvv" ,
		"Asin"  ,
		"Atan"  ,
		"CExp"  ,
		"Cos"   ,
		"DisOp" ,
		"Divpv" ,
		"Divvp" ,
		"Divvv" ,
		"Eqfpp" ,
		"Eqtpp" ,
		"Eqfpv" ,
		"Eqtpv" ,
		"Eqfvp" ,
		"Eqtvp" ,
		"Eqfvv" ,
		"Eqtvv" ,
		"Exp"   ,
		"Ldp"   ,
		"Ldv"   ,
		"Inv"   ,
		"Lefpp" ,
		"Letpp" ,
		"Lefpv" ,
		"Letpv" ,
		"Lefvp" ,
		"Letvp" ,
		"Lefvv" ,
		"Letvv" ,
		"Log"   ,
		"Ltfpp" ,
		"Lttpp" ,
		"Ltfpv" ,
		"Lttpv" ,
		"Ltfvp" ,
		"Lttvp" ,
		"Ltfvv" ,
		"Lttvv" ,
		"Mulpv" ,
		"Mulvp" ,
		"Mulvv" ,
		"Non"   ,
		"Par"   ,
		"Prip"  ,
		"Priv"  ,
		"Sin"   ,
		"Sqrt"  ,
		"Stpp"  ,
		"Stvp"  ,
		"Stpv"  ,
		"Stvv"  ,
		"Subpv" ,
		"Subvp" ,
		"Subvv"
	};
	CppADUnknownError( 
		size_t(SubvvOp) == sizeof(OpName) / sizeof(OpName[0]) - 1
	);
	printOpField(os,  "i=",      i_var, 5);
	printOpField(os, "op=", OpName[op], 6); 
	switch( op )
	{
		case LdpOp:
		CppADUnknownError( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], 5);
		printOpField(os, "x_j=", ind[1], 5);
		break;

		case LdvOp:
		CppADUnknownError( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], 5);
		printOpField(os, "x_i=", ind[1], 5);
		break;

		case StppOp:
		CppADUnknownError( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], 5);
		printOpField(os, "x_j=", ind[1], 5);
		printOpField(os, "  y=", *(Rec->GetPar(ind[2])), 5);
		break;

		case StpvOp:
		CppADUnknownError( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], 5);
		printOpField(os, "x_j=", ind[1], 5);
		printOpField(os, "y_i=", ind[2], 5);
		break;

		case StvpOp:
		CppADUnknownError( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], 5);
		printOpField(os, "x_i=", ind[1], 5);
		printOpField(os, "  y=", *(Rec->GetPar(ind[2])), 5);
		break;

		case StvvOp:
		CppADUnknownError( NumInd(op) == 3 );
		printOpField(os, "off=", ind[0], 5);
		printOpField(os, "x_i=", ind[1], 5);
		printOpField(os, "y_i=", ind[2], 5);
		break;

		case EqtvvOp:
		case EqfvvOp:
		case LetvvOp:
		case LefvvOp:
		case LttvvOp:
		case LtfvvOp:
		case AddvvOp:
		case DivvvOp:
		case MulvvOp:
		case SubvvOp:
		case DisOp:
		CppADUnknownError( NumInd(op) == 2 );
		printOpField(os, "x_i=", ind[0], 5);
		printOpField(os, "y_i=", ind[1], 5);
		break;

		case EqtpvOp:
		case EqfpvOp:
		case LetpvOp:
		case LefpvOp:
		case LttpvOp:
		case LtfpvOp:
		case AddpvOp:
		case SubpvOp:
		case MulpvOp:
		case DivpvOp:
		CppADUnknownError( NumInd(op) == 2 );
		printOpField(os, "  p=", *(Rec->GetPar(ind[0])), 5);
		printOpField(os, "y_i=", ind[1], 5);
		break;

		case EqtvpOp:
		case EqfvpOp:
		case LetvpOp:
		case LefvpOp:
		case LttvpOp:
		case LtfvpOp:
		case AddvpOp:
		case DivvpOp:
		case MulvpOp:
		case SubvpOp:
		CppADUnknownError( NumInd(op) == 2 );
		printOpField(os, "x_i=", ind[0], 5);
		printOpField(os, "  p=", *(Rec->GetPar(ind[1])), 5);
		break;

		case EqfppOp:
		case EqtppOp:
		case LefppOp:
		case LetppOp:
		case LtfppOp:
		case LttppOp:
		CppADUnknownError( NumInd(op) == 2 );
		printOpField(os, "  x=", *(Rec->GetPar(ind[0])), 5);
		printOpField(os, "  y=", *(Rec->GetPar(ind[1])), 5);
		break;

		case AbsOp:
		case AcosOp:
		case AsinOp:
		case AtanOp:
		case CosOp:
		case ExpOp:
		case LogOp:
		case SinOp:
		case SqrtOp:
		CppADUnknownError( NumInd(op) == 1 );
		printOpField(os, "x_i=", ind[0], 5);
		break;

		case ParOp:
		CppADUnknownError( NumInd(op) == 1 );
		printOpField(os, "  p=", *(Rec->GetPar(ind[0])), 5);
		break;

		case PripOp:
		CppADUnknownError( NumInd(op) == 2 );
		printOpField(os, "txt=", *(Rec->GetTxt(ind[0])), 5);
		printOpField(os, "  p=", *(Rec->GetPar(ind[1])), 5);
		break;

		case PrivOp:
		CppADUnknownError( NumInd(op) == 2 );
		printOpField(os, "txt=", *(Rec->GetTxt(ind[0])), 5);
		printOpField(os, "x_i=", ind[1], 5);
		break;

		case InvOp:
		case NonOp:
		CppADUnknownError( NumInd(op) == 0 );
		break;

		case CExpOp:
		CppADUnknownError(ind[1] != 0);
		CppADUnknownError( NumInd(op) == 6 );
		printOpField(os, "", CompareOpName[ ind[0] ], 3);
		if( ind[1] & 1 )
			printOpField(os, "vleft=", ind[2], 5);
		else	printOpField(os, "pleft=", *(Rec->GetPar(ind[2])), 5);
		if( ind[1] & 2 )
			printOpField(os, "vright=", ind[3], 5);
		else	printOpField(os, "pright=", *(Rec->GetPar(ind[3])), 5);
		if( ind[1] & 4 )
			printOpField(os, "vtrue=", ind[4], 5);
		else	printOpField(os, "ptrue=", *(Rec->GetPar(ind[4])), 5);
		if( ind[1] & 8 )
			printOpField(os, "vfalse=", ind[5], 5);
		else	printOpField(os, "pfalse=", *(Rec->GetPar(ind[5])), 5);
		break;

		default:
		CppADUnknownError(0);
	}
	size_t k;
	if( NumVar(op) > 0 )
	{ 
		for(k = 0; k < nfz; k++)
			std::cout << ", fz[" << k << "]=" << fz[k];
		for(k = 0; k < nrz; k++)
			std::cout << ", rz[" << k << "]=" << rz[k];
	}
	std::cout << std::endl;
}

} // END CppAD namespace

# endif
