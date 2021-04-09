/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cmath>
/*
$begin cppad_link_fabs$$
$spell
    cppad
    fabs
$$

$section Link to Absolute Value Function$$

$srccode%cpp% */
extern "C" double cppad_link_fabs(double x)
{   return std::fabs(x); }
/* %$$
$end

------------------------------------------------------------------------------
$begin cppad_link_sign$$
$spell
    cppad
$$

$section Link to sign Function$$

$srccode%cpp% */
extern "C" double cppad_link_sign(double x)
{   return double(x > 0.0) - double(x < 0.0); }
/* %$$
$end

------------------------------------------------------------------------------
$begin cppad_link_double2ascii$$
$spell
    cppad
    len
$$

$section Link to Conversion From double To Ascii$$

$head Syntax$$
$icode%n_char% = cppad_link_double2ascii(%len_ascii%, %ascii%, %value%)
%$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN_DOUBLE2ASCII%// END_DOUBLE2ASCII%1
%$$

$head len_ascii$$
This is the length of the $icode ascii$$ vector
and must be greater than or equal zero.

$head ascii$$
The input value of this vector does not matter.
Upon return it contains the ascii characters corresponding
$icode value$$.
Note that this does not include a terminating $code '\0'$$ character
at the end.

$head value$$
This is the value that is converted to ascii.

$head n_char$$
This is the number of characters that are placed in $icode ascii$$.
If more than $icode len_ascii$$ characters are required to properly
display $icode value$$, $icode len_ascii$$ characters are placed in
$icode ascii$$, each with the value $code x$$, and $icode n_char$$
is equal to $icode len_ascii$$.

$head IO Library$$
This routine works without linking in any I/O library routines.

$end
*/
extern "C" int cppad_link_special(int len_ascii, char* ascii, const char* value)
{   int len_value = 0;
    while( value[len_value] != '\0' )
        ++len_value;
    if(len_ascii < len_value )
    {   for(int i = 0; i < len_ascii; ++i)
            ascii[i] = 'x';
        return len_ascii;
    }
    for(int i = 0; i < len_value; ++i)
        ascii[i] = value[i];
    return len_value;
}
// BEGIN_DOUBLE2ASCII
extern "C" int cppad_link_double2ascii(int len_ascii, char* ascii, double value)
// END_DOUBLE2ASCII
{   // mantissa has one extra digit for rounding off
    int n_mantissa = 6;
    int mantissa[7];
    //
    int n_exponent = 3;
    int exponent[3];
    //
    // size of buffere is greater thant or equal
    // 2 + n_mantissa + 2 + n_exponent
    char buffer[2 + 6 + 2 + 3];
    //
    if( len_ascii <= 0 )
        return 0;
    //
    // +0.0
    const char* zero = "+0.0";
    if( value == 0.0 )
        return cppad_link_special(len_ascii, ascii, zero);
    //
    // nan
    const char* nan = "nan";
    if( value != value )
        return cppad_link_special(len_ascii, ascii, nan);
    //
    // +inf
    const char* plus_inf  = "+inf";
    if( value == 1.0 / 0.0 )
        return cppad_link_special(len_ascii, ascii, plus_inf);
    //
    // -inf
    const char* minus_inf = "-inf";
    if( value == - 1.0 / 0.0 )
        return cppad_link_special(len_ascii, ascii, minus_inf);
    //
    // ---------------------------------------------------------------------
    // normalize the value
    // ---------------------------------------------------------------------
    int value_negative = value < 0.0;
    if( value_negative )
        value = - value;
    //
    int pow10 = 0;
    while( value < 1.0 )
    {   --pow10;
        value *= 10.0;
        if( pow10 <= -1000 )
            return cppad_link_special(len_ascii, ascii, zero);
    }
    while(value >= 10.0 )
    {   ++pow10;
        value /= 10.0;
        if( 1000 <= pow10 )
        {   if( value_negative )
                return cppad_link_special(len_ascii, ascii, plus_inf);
            else
                return cppad_link_special(len_ascii, ascii, minus_inf);
        }
    }
    // assert( 1.0 < value && value < 10.0 );
    int pow10_negative = pow10 < 0;
    if( pow10_negative )
        pow10 = - pow10;
    // -------------------------------------------------------------------
    // mantissa
    // -------------------------------------------------------------------
    //
    // compute n_mantissa + 1 digits
    for(int i = 0; i <= n_mantissa; ++i)
    {   mantissa[i] = (int) value;
        // assert( mantissa[i] < 10 );
        value -= (double) mantissa[i];
        value *= 10.0;
    }
    //
    // round off to n_mantissa
    if( mantissa[n_mantissa] >= 5 )
    {   int i        = n_mantissa - 1;
        mantissa[i] += 1;
        while( i > 0 && mantissa[i] == 10 )
        {   mantissa[i-1] += 1;
            mantissa[i]    = 0;
            --i;
        }
        if( mantissa[0] == 10 )
        {   for(i = 1; i < n_mantissa; ++i)
                mantissa[i+1] = mantissa[i];
            ++pow10;
            mantissa[0] = '1';
            mantissa[1] = '0';
        }
    }
    //
    // remove trailing zeros in mantissa
    while( n_mantissa > 2 && mantissa[n_mantissa - 1] == 0 )
        --n_mantissa;
    // -------------------------------------------------------------------
    // exponent
    // -------------------------------------------------------------------
    //
    // compute exponents in reverse order
    for(int i = 0; i < n_exponent; ++i)
    {   exponent[i] = pow10 % 10;
        pow10      /= 10;
    }
    //
    // remove leading zeros in exponent
    while( n_exponent > 0 && exponent[n_exponent - 1] == 0 )
            --n_exponent;
    // ---------------------------------------------------------------------
    // convert digits to ascii in buffer
    // ---------------------------------------------------------------------
    int n_char = 0;
    if( value_negative )
        buffer[0] = '-';
    else
        buffer[0] = '+';
    buffer[1] = static_cast<char>('0' + mantissa[0]);
    buffer[2] = '.';
    for(int i = 1; i < n_mantissa; ++i)
        buffer[2 + i] = static_cast<char>('0' + mantissa[i]);
    if( n_exponent == 0 )
    {   buffer[2 + n_mantissa] = '\0';
        n_char = 2 + n_mantissa;
    }
    else
    {   buffer[2 + n_mantissa] = 'e';
        if( pow10_negative )
            buffer[2 + n_mantissa + 1] = '-';
        else
            buffer[2 + n_mantissa + 1] = '+';
        //
        // convert digits to ascii
        for(int i = 0; i < n_exponent; ++i)
            buffer[2 + n_mantissa + 2 + i] =
                static_cast<char>('0' + exponent[n_exponent - i - 1]);
        //
        n_char = 2 + n_mantissa + 2 + n_exponent;
    }
    // ---------------------------------------------------------------------
    // copy to ascii and return
    // ---------------------------------------------------------------------
    if( n_char > len_ascii )
    {   n_char = len_ascii;
        for(int i = 0; i < len_ascii; ++i)
            ascii[i] = 'x';
    }
    else
    {   for(int i = 0; i < n_char; ++i)
            ascii[i] = buffer[i];
    }
    //
    return n_char;
}
/*
-------------------------------------------------------------------------------
$begin cppad_link_print$$
$spell
    cppad
    len
    notpos
$$

$section Function That Acts Like AD Graph Print Operator$$

$head Syntax$$
$icode%n_out% = cppad_link_print(
    %len_msg%, %n_in%, %msg%, %notpos%, %before%, %value%, %after%
)%$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN_PRINT%// END_PRINT%1
%$$

$head n_in$$
This is the number of characters that are already in $icode msg$$.
It must hold that $icode%n_in% <= %len_msg%$$.

$head len_msg$$
This is the length of the vector $icode msg$$; i.e.,
the number of character $icode msg$$ can hold.

$head msg$$
The contents of $icode msg$$ with index less than $icode n_in$$
are not modified.
The characters placed in $icode msg$$ start with index $icode n_in$$
and end with index $icode%n_out% - 1%$$.
No $code '\0$$ character are placed in $icode msg$$.

$head notpos$$
If this value is positive, no characters are placed in $icode msg$$
and $icode n_out$$ is equal to $icode n_in$$.

$head before$$
This is a $code '\0'$$ terminated string containing the characters to be
put first in $icode msg$$.
If the characters in $icode before$$ do not fit,
they are truncated and $icode%n_out% = %len_msg%$$.

$head value$$
This value is converted to ascii and the corresponding characters are
placed in $icode msg$$ following $icode before$$.
If the ascii characters corresponding to $icode value$$ do not fit,
the character $code x$$ used to signify this fact.

$head after$$.
This is a $code '\0'$$ terminated string containing the characters
to be put in $icode msg$$ following $icode value$$.
If the characters in $icode before$$ do not fit,
they are truncated and $icode%n_out% = %len_msg%$$.


$end
*/
// BEGIN_PRINT
extern "C" int cppad_link_print(int n_in, int len_msg,
char* msg, double notpos, const char* before, double value, const char* after
)
// END_PRINT
{   //
    // initialize n_out
    int n_out = n_in;
    //
    if( 0.0 < notpos || n_in == len_msg )
        return n_out;
    //
    // copy before
    int i = 0;
    while( before[i] != '\0' )
    {   msg[n_out++] = before[i++];
        if( n_out == len_msg )
            return n_out;
    }
    //
    // copy characters corresponding to value
    int   len_ascii = len_msg - n_out;
    char* ascii     = msg + n_out;
    int   n_char    = cppad_link_double2ascii(len_ascii, ascii, value);
    n_out            += n_char;
    if( n_out == len_msg )
        return n_out;
    //
    // copy after
    i = 0;
    while( after[i] != '\0' )
    {   msg[n_out++] = after[i++];
        if( n_out == len_msg )
            return n_out;
    }
    //
    return n_out;
}
