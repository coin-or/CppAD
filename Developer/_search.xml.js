// ------------------------------------------------------- 
// Copyright (C) Bradley M. Bell 2003, All rights reserved 
// ------------------------------------------------------- 
Keyword = 
[
"Developer  CppAD Developer Documentation   description",
"Distribute  Building and Testing a Distribution  ",
"NewFeature  Adding a New Feature to the CppAD Distribution  ",
"ADMacro  Macros Used by CppAD Implementation   definition cppadnull cppadstandardmathfun cppadstandardmathbinaryfun cppadfoldbinaryoperator",
"GreaterThanZero  Check if a Value is Greater Than Zero   > positive",
"LessThanZero  Check if a Value is Less Than Zero   > negative",
"LessThanOrZero  Check if a Value is Less Than Zero   >= negative",
"IdenticalPar  Check if a Value is Identically a Parameter  ",
"IdenticalZero  Check if a Value is Identically Zero  ",
"IdenticalOne  Check if a Value is Identically One  ",
"IdenticalEqual  Check if a Value is Identically Equal Another  ",
"Integer  Convert a Value to an Integer  ",
"OpCode  The Tape Operator Codes  ",
"printOp  Print the Information Corresponding to One Tape Operation   operator trace",
"NumInd  Number of Ind field Values Corresponding to an Op Code  ",
"NumVar  Number of Variables Corresponding to an Op Code  ",
"TapeRec  A CppAD Program as Recorded on Tape   erase put get num replace memory",
"ADTape  ADTape: The CppAD Tape   description state rec erase",
"ADBoolFun  CppAD Boolean Valued User Defined Functions Implementation   unarybool binarybool",
"Op  Compute Values and Derivatives for Taped Operations  ",
"ForAbsOp  Forward Mode Absolute Value Function  ",
"RevAbsOp  Reverse Mode Absolute Value Function  ",
"ForAddOp  Forward Mode Addition Operators  ",
"RevAddOp  Reverse Mode Addition Operator  ",
"ForAcosOp  Forward Mode Acos Function  ",
"RevAcosOp  Reverse Mode Acos Function  ",
"ForAsinOp  Forward Mode Asin Function  ",
"RevAsinOp  Reverse Mode Asin Function  ",
"ForAtanOp  Forward Mode Atan Function  ",
"RevAtanOp  Reverse Mode Atan Function  ",
"ForDivvvOp  Forward Mode Division Operator   divide",
"RevDivvvOp  Reverse Mode Division Operator   divide fordiv",
"ForExpOp  Forward Mode Exponential Function  ",
"RevExpOp  Reverse Mode Exponential Function  ",
"ForLogOp  Forward Mode Logarithm Function  ",
"RevLogOp  Reverse Mode Logarithm Function  ",
"ForMulvvOp  Forward Mode Multiplication Operator   multiply",
"RevMulvvOp  Reverse Mode Multiplication Operator   multiply",
"ForSinCos  Forward Mode Sine and Cosine Functions  ",
"RevSinCos  Reverse Mode Sine and Cosine Functions  ",
"ForSqrtOp  Forward Mode Square Root Function  ",
"RevSqrtOp  Reverse Mode Square Root Function   forward forsqrtop",
"ForSubvvOp  Forward Mode Subtraction Operators  ",
"RevSubvvOp  Reverse Mode Subtraction Operator   minus",
"ADForward  Forward Computation of Taylor Coefficients   derivative second order",
"ADReverse  Reverse Mode Computation of Derivatives of Taylor Coefficients  ",
"ExtendBuffer  Extends the Length of a Buffer  "
]

var MaxList = 100;
var Choice  = "";
var Nstring = -1;
var Nkeyword = Keyword.length;
Initialize();

function Initialize()
{
	var i;
	var line;
	for(i = 0; (i < Nkeyword) && (i < MaxList) ; i++)
	{
		line       = Keyword[i].split(/\s+/)
		line[0]    = line[0].toUpperCase();
		line       = line.join(" ");
		Keyword[i] = line;
	}
	Search()
}
function Search()
{
	var string  = document.search.string.value;
	if( Nstring == string.length )
		return;
	Nstring     = string.length;

	var word    = string.match(/\S+/g);
	var nword   = 0;
	if(word != null )
		nword   = word.length;

	var pattern = new Array(nword);
	for(var j = 0; j < nword; j++)
		pattern[j] = new RegExp(word[j], "i");

	var nlist = 0;
	var list  = "";
	Choice    = "";

	for(i = 0; (i < Nkeyword) && (nlist < MaxList) ; i++)
	{
		var match = true;
		for(j = 0; j < nword; j++)
			match = match && pattern[j].test(Keyword[i]);

		if( match )
		{
			line     = Keyword[i].split(/\s+/);

			if( Choice == "" )
				Choice = line[0];

			line  = line.join(" ");
			list  = list + line + "\n";
			nlist = nlist + 1;
		}
	}
	document.search.choice.value  = Choice.toLowerCase();
	document.search.list.value    = list;
}
function choose()
{
parent.location = document.search.choice.value.toLowerCase() + ".xml";
}
