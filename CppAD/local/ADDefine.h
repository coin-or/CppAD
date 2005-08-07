# ifndef CppADADDefineIncluded
# define CppADADDefineIncluded
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
$begin ADDefine$$
$spell
	Microsoft
	VecADelem
	Cpp
	inline
	namespace
	std
	const
	Op
$$
$aindex head$$

$mindex macro definition$$
$section Macros Used by CppAD Implementation$$ 

$head CppADNull$$
The preprocessor symbol $code CppADNull$$ is used for a null pointer. 
If it is not yet defined,
it is defined when $code CppAD/local/ADDefine.h$$ is included is included.

$head CppADStandardMathFun$$
The macro call
$syntax%
	CppADStandardMathFun(%Name%)
%$$
defines an inline function in the
current namespace that is a link to the corresponding
standard mathematical functions
$syntax%
	float           %Name%(float x)
	double          %Name%(double x)
	complex<float>  %Name%(std::complex<float> x)
	complex<double> %Name%(std::complex<double> x)
%$$
Note that you should not place a semi-colon directly after the use
of this macro.

$head CppADStandardMathBinaryFun$$
The macro call
$syntax%
	CppADStandardMathBinaryFun(%Name%)
%$$
defines an inline function in the
current namespace that is a link to the corresponding
standard mathematical functions
$syntax%
	float           %Name%(float x, float y)
	double          %Name%(double x, double y)
	complex<float>  %Name%(std::complex<float> x, std::complex<float> y)
	complex<double> %Name%(std::complex<double> x, std::complex<double> y)
%$$
Note that you should not place a semi-colon directly after the use
of this macro.

$head CppADFoldBinaryOperator$$
The syntax
$syntax%
	CppADFoldBinaryOperator(%Type%, %Op%)
%$$
defines the operators
$syntax%
	%left% %Op% %right%
%$$
which has the result type $italic Type$$ and the 
following argument types:
$syntax%
	%left%                    %right%
	-----------------         -----------------
	AD<%Base%>                VecADelem<%Base%>
	VecADelem<%Base%>         AD<%Base%>
	VecADelem<%Base%>         VecADelem<%Base%>
	%Base%                    AD<%Base%>
	%Base%                    VecADelem<%Base%>
	AD<%Base%>                %Base%
	VecADelem<%Base%>         %Base%
	int                       AD<%Base%>
	int                       VecADelem<%Base%>
	AD<%Base%>                int
	VecADelem<%Base%>         int
%$$
In the case where the left operand has type $syntax%AD<%Base%>%$$,
it defines a member function.
All of the arguments are $code const$$ and call by reference,
except for the $code int$$ case which is call by value.
In all cases, it converts the operands to $syntax%AD<%Base%>%$$ and then
uses the definition of the same operation for that case. 
Hence it assumes a definition
for the case where both operands are $syntax%AD<%Base%>%$$ objects.


$end
*/

# ifndef CppADNull
# define CppADNull     0
# endif

# ifdef _MSC_VER

# define CppADStandardMathFun(Name)                                       \
                                                                          \
	inline float Name(float x)                                        \
	{	return ::Name(x); }                                       \
                                                                          \
	inline std::complex<float> Name(std::complex<float> x)            \
	{	return std::Name(x); }                                    \
                                                                          \
	inline double Name(double x)                                      \
	{	return ::Name(x); }                                       \
                                                                          \
	inline std::complex<double> Name(std::complex<double> x)          \
	{	return std::Name(x); }

# else

# define CppADStandardMathFun(Name)                                       \
                                                                          \
	inline float Name(float x)                                        \
	{	return std::Name(x); }                                    \
                                                                          \
	inline std::complex<float> Name(std::complex<float> x)            \
	{	return std::Name(x); }                                    \
                                                                          \
	inline double Name(double x)                                      \
	{	return std::Name(x); }                                    \
                                                                          \
	inline std::complex<double> Name(std::complex<double> x)          \
	{	return std::Name(x); }

# endif

# ifdef _MSC_VER

# define CppADStandardMathBinaryFun(Name)                                 \
                                                                          \
	inline float Name(float x, float y)                               \
	{	return ::Name(x, y); }                                    \
                                                                          \
	inline std::complex<float> Name(                                  \
		std::complex<float> x, std::complex<float> y)             \
	{	return std::Name(x, y); }                                 \
                                                                          \
	inline double Name(double x, double y)                            \
	{	return ::Name(x, y); }                                    \
                                                                          \
	inline std::complex<double> Name(                                 \
		std::complex<double> x, std::complex<double> y)           \
	{	return std::Name(x, y); }

# else

# define CppADStandardMathBinaryFun(Name)                                 \
                                                                          \
	inline float Name(float x, float y)                               \
	{	return std::Name(x, y); }                                 \
                                                                          \
	inline std::complex<float> Name(                                  \
		std::complex<float> x, std::complex<float> y)             \
	{	return std::Name(x, y); }                                 \
                                                                          \
	inline double Name(double x, double y)                            \
	{	return std::Name(x, y); }                                 \
                                                                          \
	inline std::complex<double> Name(                                 \
		std::complex<double> x, std::complex<double> y)           \
	{	return std::Name(x, y); }

# endif

# define CppADFoldBinaryOperator(Type, Op)                             \
/* ----------------------------------------------------------------*/  \
/* Operations with VecADelem<Base> and AD<Base> only*/                 \
template <class Base>                                                  \
                                                                       \
inline Type AD<Base>::operator Op (const VecADelem<Base> &right) const \
{	return *this Op right.ADBase(); }                              \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const VecADelem<Base> &left, const VecADelem<Base> &right)    \
{	return left.ADBase() Op right.ADBase(); }                      \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const VecADelem<Base> &left, const AD<Base> &right)           \
{	return left.ADBase() Op right; }                               \
/* ----------------------------------------------------------------*/  \
/* Operations Base */                                                  \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const Base &left, const AD<Base> &right)                      \
{	return AD<Base>(left) Op right; }                              \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const Base &left, const VecADelem<Base> &right)               \
{	return AD<Base>(left) Op right.ADBase(); }                     \
                                                                       \
template <class Base>                                                  \
inline Type AD<Base>::operator Op (const Base &right)  const           \
{	return *this Op AD<Base>(right); }                             \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const VecADelem<Base> &left, const Base &right)               \
{	return left.ADBase() Op AD<Base>(right); }                     \
                                                                       \
/* ----------------------------------------------------------------*/  \
/* Operations int */                                                   \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(int left, const AD<Base> &right)                              \
{	return AD<Base>(left) Op right; }                              \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(int left, const VecADelem<Base> &right)                       \
{	return AD<Base>(left) Op right.ADBase(); }                     \
                                                                       \
template <class Base>                                                  \
inline Type AD<Base>::operator Op (int right) const                    \
{	return *this Op AD<Base>(right); }                             \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const VecADelem<Base> &left, int right)                       \
{	return left.ADBase() Op AD<Base>(right); }

# endif
