# ifndef CppADTapeRecIncluded
# define CppADTapeRecIncluded

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
$begin TapeRec$$

$spell
	inline
	VecInd
	sizeof
	Num
	Ind
	Cpp
	Rec
	const
	Op
$$

$section A CppAD Program as Recorded on Tape$$
$index tape, record$$
$index record, tape$$
$index TapeRec$$

$table
$bold Syntax$$ $cnext 
$syntax%TapeRec<%Base%> %Rec%$$
$rnext $cnext
$syntax%TapeRec<%Base%> %Rec%(const TapeRec<%Base%> &%Other%)%$$
$tend

$fend 20$$

$head Default Constructors$$
The default constructor 
$syntax%
	TapeRec<%Base%> %Rec%
%$$
creates a program recording called $italic Rec$$ with no contents and some
default setting for the size of its buffers.

$head Copy Constructor$$
The copy constructor
$syntax%
	TapeRec<%Base%> %Rec%(const TapeRec<%Base%> &%Other%)
%$$
creates $italic Rec$$ as a program recording with all the same
information as $italic Other$$ and with the smallest possible buffer sizes
that will hold that information.

$head Erase$$
$index TapeRec, Erase$$
$index Erase, TapeRec$$
The syntax 
$syntax%
	void %Rec%.Erase(void)
%$$
erases the contents of $italic Rec$$.
The buffers used to store information are not erased
and can be used for a new recording.

$head Put$$
$index TapeRec, Put$$
$index Put, TapeRec$$

$subhead Op$$
The function call
$syntax%
	inline size_t %Rec%.PutOp(OpCode %op%)
%$$
places the value $italic op$$ at the end of the current Op recording
and returns the offset for the corresponding variable 
(if the operator has a resulting variable).
With each call, this index increments by the number of variables required
for the previous call to $code PutOp$$.

$subhead Ind$$
The function call
$syntax%
	inline void %Rec%.PutInd(size_t %ind0%)
	inline void %Rec%.PutInd(size_t %ind0%, size_t %ind1%)
	inline void %Rec%.PutInd(size_t %ind0%, size_t %ind1%, size_t %ind2%)
	inline void %Rec%.PutInd(
		size_t %ind0%, size_t %ind1%, size_t %ind2%, size_t %ind3%)
%$$
places the values passed to $code PutInd$$ at the end of the
Ind recording and in the order passed; i.e., $italic ind0$$
comes before $italic ind1$$ e.t.c.


$subhead Par$$
The function call
$syntax%
	inline size_t %Rec%.PutPar(const %Base% &%par%)
%$$
places the value $italic par$$ in the Par recording
and returns its index with in the recording.
This value is not necessarily placed at the end of the recording
so there is no specified pattern to the return values.

$subhead VecInd$$
The function call
$syntax%
	inline size_t %Rec%.PutVecInd(size_t %vecInd%)
%$$
places the value $italic vecInd$$ at the end of the current VecInd recording
and returns its index with in the recording.
This index starts at zero after each $code Erase$$ or default constructor
and increments by one for each call to this function.

$head Get$$
$index TapeRec, Get$$
$index Get, TapeRec$$

$subhead Op$$
The syntax
$syntax%
	OpCode %Rec%.GetOp(size_t %i%) const
%$$
returns the value of $italic op$$ in the $th i+1$$ call to 
$syntax%%Rec%.PutOp(%op%)%$$.

$subhead VecInd$$
The syntax
$syntax%
	OpCode %Rec%.GetVecInd(size_t %i%) const
%$$
returns the value of $italic vecInd$$ in the corresponding call 
$syntax%%Rec%.PutVecInd(%vecInd%)%$$
where $italic i$$ is the return value of $code PutVecInd$$. 

$subhead Ind$$
The syntax
$syntax%
	const size_t *%Rec%.GetInd(size_t %n%, size_t %i%) const
%$$
returns a pointer to a copy of $italic n$$ values in the Ind recording
starting at the index $italic i$$.

$subhead Par$$
The syntax
$syntax%
	const %Base% *%Rec%.GetPar(size_t %i%) const
%$$
returns a pointer to a value equal to
$italic par$$ in the corresponding call 
$syntax%%Rec%.PutPar(%par%)%$$ 
where $italic i$$ is the return value of $code PutPar$$.
(If $code NDEBUG$$ is not defined, $code GetPar$$ checks that
the argument $italic i$$ is valid.)

$head Num$$
$index TapeRec, Num$$
$index Num, TapeRec$$

$subhead Op$$
The syntax
$syntax%
	size_t %Rec%.NumOp(void) const
%$$
returns the number of Op values
that are currently stored in $italic Rec$$.
This increments by one each time $code PutOp$$ is called; i.e.,
it is the number of calls to $code PutOp$$.

$subhead Ind$$
The syntax
$syntax%
	size_t %Rec%.NumInd(void) const
%$$
returns the number of Ind values
that are currently stored in $italic Rec$$.
This increments by one for each value that is stored by $code PutInd$$.

The syntax
$syntax%
	size_t %Rec%.NumVecInd(void) const
%$$
returns the number of VecInd values
that are currently stored in $italic Rec$$.
This increments by one each time $code PutVecInd$$ is called.

$subhead Par$$
The syntax
$syntax%
	size_t %Rec%.NumPar(void) const
%$$
returns the number of Par values
that are currently stored in $italic Rec$$.
This increment by one or zero each time $code PutPar$$ is called.

$head Replace$$
$index TapeRec, Replace$$
$index Replace, TapeRec$$

$subhead Ind$$
The syntax
$syntax%
	size_t %Rec%.ReplaceInd(size_t %index%, size_t %value%)
%$$
Replaces the single value with index $italic index$$ in the sequence of
Ind values stored by calls to $syntax%%Rec%.PutInd%$$.
The argument $italic index$$ must be less than $syntax%%Rec%.NumInd()%$$.


$head Memory$$
$index TapeRec, Memory$$
$index Memory, TapeRec$$
The syntax
$syntax%
	size_t %Rec%.Memory(void) const
%$$
returns the number of memory units ($code sizeof$$) required to store
the information in $italic Rec$$.



$end
------------------------------------------------------------------------------
*/

# include <CppAD/local/ExtendBuffer.h>

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
class TapeRec {

public:
	// default
	TapeRec(void) 
	{	
		TotalNumberVar = 0;

		NumberOp       = 0;
		LengthOp       = 10;

		NumberVecInd   = 0;
		LengthVecInd   = 10;

		NumberInd      = 0;
		LengthInd      = 10;

		NumberPar      = 0;
		LengthPar      = 10;

		NumberTxt      = 0;
		LengthTxt      = 10;

		try {
			Ind = new size_t[LengthOp];
			VecInd = new size_t[LengthVecInd];
			Op  = new OpCode[LengthOp];
			Par = new Base[LengthPar];
			Txt = new char[LengthTxt];
		}
		catch (...) {
			CppADUsageError(
				0,
				"Cannot allocate a small amount of memory "
				"for the initial tape"
			);
			abort();
		}
	}

	// constructor from another recording
	TapeRec(const TapeRec &Other)
	{	size_t i;

		// Var
		TotalNumberVar  = Other.TotalNumberVar;

		// Op
		NumberOp        = Other.NumberOp;
		LengthOp        = Other.NumberOp;

		// VecInd
		NumberVecInd    = Other.NumberVecInd;
		LengthVecInd    = Other.NumberVecInd;

		// Ind
		NumberInd       = Other.NumberInd;
		LengthInd       = Other.NumberInd;

		// Par
		NumberPar       = Other.NumberPar;
		LengthPar       = Other.NumberPar;

		// Txt
		NumberTxt       = Other.NumberTxt;
		LengthTxt       = Other.NumberTxt;

		// Allocate the memory
		try {
			if( LengthOp == 0 )
				Op = CppADNull;
			else	Op = new OpCode[LengthOp];
			if( LengthVecInd == 0 )
				VecInd = CppADNull;
			else	VecInd = new size_t[LengthVecInd];
			if( LengthInd == 0 )
				Ind = CppADNull;
			else	Ind = new size_t[LengthInd];
			if( LengthPar == 0 )
				Par = CppADNull;
			else	Par = new Base[LengthPar];
			if( LengthTxt == 0 )
				Txt = CppADNull;
			else	Txt = new char[LengthTxt];
		}
		catch(...)
		{	CppADUsageError(
			0,
			"Cannot obtain necessary memory for current recording"
			);
			abort();
		}

		// Copy the data
		i = NumberOp;
		while(i--)
			Op[i] = Other.Op[i];
		i = NumberVecInd;
		while(i--)
			VecInd[i] = Other.VecInd[i];
		i = NumberInd;
		while(i--)
			Ind[i] = Other.Ind[i];
		i = NumberPar;
		while(i--)
			Par[i] = Other.Par[i];
		i = NumberTxt;
		while(i--)
			Txt[i] = Other.Txt[i];

	}

	// destructor
	~TapeRec(void)
	{	if( LengthOp > 0 )
			delete [] Op;
		if( LengthVecInd > 0 )
			delete [] VecInd; 
		if( LengthInd > 0 )
			delete [] Ind; 
		if( LengthPar > 0 )
			delete [] Par;
		if( LengthTxt > 0 )
			delete [] Txt;
	}

	// erase all information in recording
	void Erase(void)
	{	TotalNumberVar  = 0;
		NumberOp        = 0;
		NumberVecInd    = 0;
		NumberInd       = 0;
		NumberPar       = 0;
		NumberTxt       = 0;
	}

	// add information to recording
	inline size_t PutOp(OpCode op);
	inline size_t PutVecInd(size_t vecInd);
	inline size_t PutPar(const Base &par);
	inline void PutInd(size_t ind0); 
	inline void PutInd(size_t ind0, size_t ind1); 
	inline void PutInd(size_t ind0, size_t ind1, size_t ind2); 
	inline void PutInd(
		size_t ind0, 
		size_t ind1, 
		size_t ind2, 
		size_t ind3, 
		size_t ind4, 
		size_t ind5
	); 
	inline size_t PutTxt(const char *text);

	/*
	retrieve information from recording
	*/
	OpCode GetOp (size_t i) const
	{	CppADUnknownError(i < NumberOp);
		return Op[i];
	}
	size_t GetVecInd (size_t i) const
	{	CppADUnknownError(i < NumberVecInd);
		return VecInd[i];
	}
	const Base *GetPar(size_t i) const
	{	CppADUnknownError(i < NumberPar);
		return Par + i;
	}
	const size_t *GetInd(size_t n, size_t i) const
	{	CppADUnknownError(i + n <= NumberInd)
		return Ind + i;
	}
	const char *GetTxt(size_t i) const
	{	CppADUnknownError(i < NumberTxt);
		return Txt + i;
	}
	
	/*
	replace information 
	*/
	void ReplaceInd(size_t i, size_t value)
	{	CppADUnknownError( i < NumberInd);
		Ind[i] = value;
	}

	// number of values
	size_t TotNumVar(void) const
	{	return TotalNumberVar; }
	size_t NumOp(void) const
	{	return NumberOp; }
	size_t NumVecInd(void) const
	{	return NumberVecInd; }
	size_t NumInd(void) const
	{	return NumberInd; }
	size_t NumPar(void) const
	{	return NumberPar; }
	size_t NumTxt(void) const
	{	return NumberTxt; }

	// amount of memory used 
	size_t Memory(void) const
	{	return LengthOp * sizeof(OpCode) 
		     + LengthVecInd * sizeof(size_t)
		     + LengthInd * sizeof(size_t)
		     + LengthPar * sizeof(Base)
		     + LengthTxt * sizeof(char);
	}

private:
	size_t    TotalNumberVar;

	size_t    NumberOp;
	size_t    LengthOp;
	OpCode   *Op;

	size_t    NumberVecInd;
	size_t    LengthVecInd;
	size_t   *VecInd;

	size_t    NumberInd;
	size_t    LengthInd;
	size_t   *Ind;

	size_t    NumberPar;
	size_t    LengthPar;
	Base     *Par;

	size_t    NumberTxt;
	size_t    LengthTxt;
	char     *Txt;
};

template <class Base>
inline size_t TapeRec<Base>::PutOp(OpCode op)
{	size_t varIndex = TotalNumberVar;
	
	CppADUnknownError( LengthOp > 0 );
	CppADUnknownError( NumberOp <= LengthOp );
	if( NumberOp == LengthOp )
	{	LengthOp  = 2 * LengthOp;
		ExtendBuffer(LengthOp, NumberOp, Op);
	}
	CppADUnknownError( NumberOp < LengthOp );
	Op[NumberOp++]  = op;
	TotalNumberVar += NumVar(op);

	return varIndex;
}

template <class Base>
inline size_t TapeRec<Base>::PutVecInd(size_t vecInd)
{	
	CppADUnknownError( LengthVecInd > 0 );
	CppADUnknownError( NumberVecInd <= LengthVecInd );
	if( NumberVecInd == LengthVecInd )
	{	LengthVecInd      = 2 * LengthVecInd;
		ExtendBuffer(LengthVecInd, NumberVecInd, VecInd);
	}
	CppADUnknownError( NumberVecInd < LengthVecInd );
	VecInd[NumberVecInd++] = vecInd;

	return NumberVecInd - 1;
}

template <class Base>
inline size_t TapeRec<Base>::PutPar(const Base &par)
{	size_t i;
	
	CppADUnknownError( LengthPar > 0 );
	CppADUnknownError( NumberPar <= LengthPar );
	
	// check last three values to see if same one came up
	if( NumberPar >= 3 )
	{	i = NumberPar;
		while(i > NumberPar - 3)
		{	--i;
			if( IdenticalEqual(Par[i], par) )
				return i;
		}
	}
	
	// place a new value in the table
	if( NumberPar == LengthPar )
	{	LengthPar    = 2 * LengthPar;
		ExtendBuffer(LengthPar, NumberPar, Par);
	}
	CppADUnknownError( NumberPar < LengthPar );
	Par[NumberPar++] = par;

	return NumberPar - 1;
}

template <class Base>
inline void TapeRec<Base>::PutInd(size_t ind0)
{ 
	CppADUnknownError( LengthInd > 0 );
	CppADUnknownError( NumberInd <= LengthInd );
	if( NumberInd == LengthInd )
	{	LengthInd    = 2 * LengthInd;
		ExtendBuffer(LengthInd, NumberInd, Ind);
	}
	CppADUnknownError( NumberInd < LengthInd );
	Ind[NumberInd++] = ind0;

}

template <class Base>
inline void TapeRec<Base>::PutInd(size_t ind0, size_t ind1)
{ 
	CppADUnknownError( LengthInd > 0 );
	CppADUnknownError( NumberInd <= LengthInd );
	if( NumberInd + 1 >= LengthInd )
	{	LengthInd    = 2 * LengthInd;
		ExtendBuffer(LengthInd, NumberInd, Ind);
	}
	CppADUnknownError( NumberInd + 1 < LengthInd );
	Ind[NumberInd++] = ind0;
	Ind[NumberInd++] = ind1;

}


template <class Base>
inline void TapeRec<Base>::PutInd(size_t ind0, size_t ind1, size_t ind2)
{ 
	CppADUnknownError( LengthInd > 0 );
	CppADUnknownError( NumberInd <= LengthInd );
	if( NumberInd + 2 >= LengthInd )
	{	LengthInd    = 2 * LengthInd;
		ExtendBuffer(LengthInd, NumberInd, Ind);
	}
	CppADUnknownError( NumberInd + 2 < LengthInd );
	Ind[NumberInd++] = ind0;
	Ind[NumberInd++] = ind1;
	Ind[NumberInd++] = ind2;

}

template <class Base>
inline void TapeRec<Base>::PutInd(
	size_t ind0, 
	size_t ind1, 
	size_t ind2, 
	size_t ind3, 
	size_t ind4, 
	size_t ind5
)
{ 
	CppADUnknownError( LengthInd > 0 );
	CppADUnknownError( NumberInd <= LengthInd );
	if( NumberInd + 6 >= LengthInd )
	{	LengthInd    = 6 + 2 * LengthInd;
		ExtendBuffer(LengthInd, NumberInd, Ind);
	}
	CppADUnknownError( NumberInd + 6 < LengthInd );
	Ind[NumberInd++] = ind0;
	Ind[NumberInd++] = ind1;
	Ind[NumberInd++] = ind2;
	Ind[NumberInd++] = ind3;
	Ind[NumberInd++] = ind4;
	Ind[NumberInd++] = ind5;
}

template <class Base>
inline size_t TapeRec<Base>::PutTxt(const char *text)
{	size_t i;

	// determine length of the text including terminating '\0'
	size_t n;
	for(n = 0; text[n] != '\0'; n++)
		CppADUnknownError( n < 1000 ); // should check limit in PrintFor
	n++;

	CppADUnknownError( LengthTxt > 0 );
	CppADUnknownError( NumberTxt <= LengthTxt );

	if( NumberTxt + n >= LengthTxt )
	{	LengthTxt    = 2 * LengthTxt + n;
		ExtendBuffer(LengthTxt, NumberTxt, Txt);
	}
	CppADUnknownError( NumberTxt + n < LengthTxt );

	// copy text including terminating '\0'
	for(i = 0; i < n; i++)
		Txt[NumberTxt++] = text[i];
	CppADUnknownError( text[i-1] == '\0' );

	return NumberTxt - n;
}

} // END CppAD namespace

# endif
