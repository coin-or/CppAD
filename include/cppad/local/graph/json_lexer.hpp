# ifndef CPPAD_LOCAL_GRAPH_JSON_LEXER_HPP
# define CPPAD_LOCAL_GRAPH_JSON_LEXER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <string>
# include <cppad/core/cppad_assert.hpp>

// BEGIN_NAMESPACE_CPPAD_LOCAL_GRAPH
namespace CppAD { namespace local { namespace graph {

// ===========================================================================
class json_lexer {
// ===========================================================================

/*
-------------------------------------------------------------------------------
{xrst_begin json_lexer_member_data dev}

json lexer: Private Data
########################

Member Variables
****************

graph\_
=======
The :ref:`json_ad_graph-name` .

index\_
=======
is the index in the graph for the current character.
If a token is returned, this corresponds to the last character
it the token.

line_number\_
=============
line number in the graph for the current character

char_number\_
=============
character number in the graph for the current character

token\_
=======
used to return tokens.

function_name\_
===============
is the function name for this graph.
This is initialized as empty,
should be set as soon as it is parsed,
and is used for error reporting.

token
*****
returns current value of ``token_`` .

line_number
***********
returns current value of ``line_number_``
(which corresponds to last character in the token).

char_number
***********
returns current value of ``char_number_`` .
(which corresponds to last character in the token).

set_function_name
*****************
sets the value of ``function_name_`` .

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
private:
   const std::string& json_;
   size_t             index_;
   size_t             line_number_;
   size_t             char_number_;
   std::string        token_;
   std::string        function_name_;
public:
   const std::string& token(void)       const;
   size_t             line_number(void) const;
   size_t             char_number(void) const;
   void               set_function_name(const std::string& function_name);
/* {xrst_code}
{xrst_spell_on}

{xrst_end json_lexer_member_data}
-------------------------------------------------------------------------------
{xrst_begin json_lexer_report_error dev}

json lexer: Report an Error
###########################

Syntax
******
| *json_lexer* . ``report_error`` ( *expected* , *found* )

json_lexer
**********
is a ``local::graph::json_lexer`` object.

expected
********
is the token that is expected.

found
*****
is the token or text that was found.

Report
******
The current CppAD :ref:`ErrorHandler-name` is used to report
an error parsing this Json AD graph.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void report_error(const std::string& expected, const std::string& found);
/* {xrst_code}
{xrst_spell_on}

{xrst_end json_lexer_report_error}
-------------------------------------------------------------------------------
{xrst_begin json_lexer_next_index dev}

json lexer: Advance Index by One
################################

Syntax
******

   *json_lexer* . ``next_index`` ()

json_lexer
**********
is a ``local::graph::json_lexer`` object.

index\_
*******
The input value of ``index_`` is increased by one.
It is an error to call this routine when the input value
of ``index_`` is greater than or equal ``json_.size()`` .

line_number\_
*************
If the previous character, before  the call, was a new line,
``line_number_`` is increased by one.

char_number\_
*************
If the previous character, before the call, was a new line,
``char_number`` is set to one.
Otherwise, ``char_number_`` is increased by one.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
private:
   void next_index(void);
/* {xrst_code}
{xrst_spell_on}

{xrst_end json_lexer_next_index}
-------------------------------------------------------------------------------
{xrst_begin json_lexer_skip_white_space dev}

json lexer: Skip White Space That Separates Tokens
##################################################

Syntax
******

   *json_lexer* . ``skip_white_space`` ()

json_lexer
**********
is a json lexer object.

Discussion
**********
This member functions is used to increase ``index_`` until either
a non-white space character is found or ``index_`` is equal
to ``json_.size()`` .

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
private:
   void skip_white_space(void);
/* {xrst_code}
{xrst_spell_on}

{xrst_end json_lexer_skip_white_space}
-------------------------------------------------------------------------------
{xrst_begin json_lexer_constructor dev}

json lexer: Constructor
#######################

Syntax
******

   ``local::graph::lexer`` *json_lexer* ( *json* )

json
****
The argument *json* is an :ref:`json_ad_graph-name`
and it is assumed that *json* does not change
for as long as *json_lexer* exists.

Initialization
**************
The current token, index, line number, and character number
are set to the first non white space character in ``json_`` .
If this is not a left brace character ``'{'`` ,
the error is reported and the constructor does not return.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   json_lexer(const std::string& json);
/* {xrst_code}
{xrst_spell_on}

{xrst_end json_lexer_constructor}
-------------------------------------------------------------------------------
{xrst_begin json_lexer_check_next_char dev}
{xrst_spell
   ch
}

Get and Check Next Single Character Token
#########################################

Syntax
******

   *json_lexer* . ``check_next_char`` ( *ch* )

index\_
*******
The search for the character starts
at one greater than the input value for ``index_`` and skips white space.

ch
**
Is a non white space
single character token that is expected.
If this character is not found,
the error is reported and this function does not return.
In the special case where *ch* is ``'\0'`` ,
any non-white space character will be accepted
(but there must be such a character).

token\_
*******
If this routine returns, ``token_`` has size one
and contains the character that is found.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void check_next_char(char ch);
/* {xrst_code}
{xrst_spell_on}

{xrst_end json_lexer_check_next_char}
-------------------------------------------------------------------------------
{xrst_begin json_lexer_check_next_string dev}

Get and Check Next Single Character Token
#########################################

Syntax
******

   *json_lexer* . ``check_next_string`` ( *expected* )

index\_
*******
The search for the string starts
at one greater than the input value for ``index_`` and skips white space.

expected
********
Is the value (not including double quotes) for the string that is expected.
If this string is not found, the error is reported
and this function does not return.
In the special case where *expected* is empty,
any string will be accepted.

token\_
*******
If this routine returns,
*token_* is the string that was found.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void check_next_string(const std::string& expected);
/* {xrst_code}
{xrst_spell_on}

{xrst_end json_lexer_check_next_string}
-------------------------------------------------------------------------------
{xrst_begin json_lexer_next_non_neg_int dev}

Get Next Non-Negative Integer
#############################

Syntax
******

| |tab| *json_lexer* . ``next_non_neg_int`` ()
| |tab| *value* = *json_lexer* . ``token2size_t`` ()

index\_
*******
The search for the non-negative integer starts
at one greater than the input value for ``index_`` and skips white space.

token\_
*******
is set to the non-negative integer.
If the next token is not a non-negative integer,
the error is reported and this function does not return.

value
*****
If the current token is a non-negative integer,
*value* is the corresponding value.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void next_non_neg_int(void);
   size_t token2size_t(void) const;

/* {xrst_code}
{xrst_spell_on}

{xrst_end json_lexer_next_non_neg_int}
-------------------------------------------------------------------------------
{xrst_begin json_lexer_next_float dev}

Get Next Floating Point Number
##############################

Syntax
******

| |tab| *ok* = *json_lexer* . ``next_float`` ()
| |tab| *value* = *json_lexer* . ``token2double`` ()

index\_
*******
The search for the floating point number starts
at one greater than the input value for ``index_`` and skips white space.

token\_
*******
is set to the floating point number.
If the next token is not a floating point number,
the error is reported and this function does not return.

value
*****
If the current token is a floating point number,
*value* is the corresponding value.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void next_float(void);
   double token2double(void) const;

/* {xrst_code}
{xrst_spell_on}

{xrst_end json_lexer_next_float}
*/

// ==========================================================================
}; // end class lexer
// ==========================================================================


} } } // END_NAMESPACE_CPPAD_LOCAL_GRAPH


# endif
