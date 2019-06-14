# ifndef CPPAD_LOCAL_JSON_PARSER_HPP
# define CPPAD_LOCAL_JSON_PARSER_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */

# include <string>
# include <cppad/core/cppad_assert.hpp>

// BEGIN_NAMESPACE_CPPAD_LOCAL_JSON
namespace CppAD { namespace local { namespace json {

// ===========================================================================
class parser {
// ===========================================================================

/*
-------------------------------------------------------------------------------
$begin json_parser_member_data$$

$section json parset: Private Data$$

$head graph_$$
The $cref json_ad_graph$$.

$head index_$$
is the index in the graph for the current character.
If a token is returned, this corresponds to the last character
it the token.

$head line_number_$$
line number in the graph for the current character

$head char_number_$$
character number in the graph for the current character

$head token_$$
used to return tokens.

$head token$$
returns current value of $code token_$$.

$head line_number$$
returns current value of $code line_number_$$
(which corresponds to last character in the token).

$head char_number$$
returns current value of $code char_number_$$.
(which corresponds to last character in the token).

$head Source Code$$
$srcode% */
private:
    const std::string& graph_;
    size_t             index_;
    size_t             line_number_;
    size_t             char_number_;
    std::string        token_;
public:
    const std::string& token(void)       const;
    size_t             line_number(void) const;
    size_t             char_number(void) const;
/* $$
$enb
-------------------------------------------------------------------------------
$begin json_parser_next_index$$

$section json parser: Advance Index by One$$

$head Syntax$$
$codei%
    %json_parser%.next_index()
%$$

$head json_parser$$
is a $code local::json::parser$$ object.

$head index_$$
The input value of $code index_$$ is increased by one.
It is an error to call this routine when the input value
of $code index_$$ is greater than or equal $code graph_.size()$$.

$head line_number_$$
If the previous character, before  the call, was a new line,
$code line_number_$$ is increased by one.

$head char_number_$$
If the previous character, before the call, was a new line,
$code char_number$$ is set to one.
Othwerise, $code char_number_$$ is increased by one.

$head Prototype$$
$srccode */
private:
    void next_index(void);
/* $$
$enb
-------------------------------------------------------------------------------
$begin json_parser_skip_white_space$$

$section json parser: Skip White Space That Separates Tokens$$

$head Syntax$$
$codei%
    %json_parser%.skip_white_space()
%$$

$head json_parser$$
is a json parser object.

$head Discussion$$
The $cref json_parser_next_index$$ rountine is used to increase
$code index_$$ until either
a non-white space character is found or $code index_$$ is equal
to $code graph_.size()$$.

$head Prototype$$
$srccode */
private:
    void skip_white_space(void);
/* $$
$end
-------------------------------------------------------------------------------
$begin json_parser_constructor$$

$section json parser: Constructor$$

$head Syntax$$
$codei%
    local::json::parser %json_parser%(%graph%)
%$$

$head graph$$
The argument $icode graph$$ is an $cref json_ad_graph$$
and it is assumed that $icode graph$$ does not change
for as long as $icode json_parser$$ exists.

$head Initialization$$
The current token, index, line number, and character number
are set to the first non white space character in $code graph_$$.
If all of $code graph_$$ is white space, the cuurrent token is set to
$code ""$$.

$head Prototype$$
$srccode */
public:
    parser(const std::string& graph);
/* $$
$end
-------------------------------------------------------------------------------
$begin json parser_next_char$$

$section Get Next Single Character$$

$head Syntax$$
$codei%
    %ok% = %json_parser%.next_char()
%$$

$head index_$$
The search for the character starts
at one greater than the input value for $code index_$$ and skips white space.

$head ok$$
is true if a non white space character is found.

$head token_$$
If $icode ok$$ is true,
$icode token_$$ is set to the character that is found.

$head Prototype$$
$srccode */
public:
    bool next_char(void);
/* $$
$end
-------------------------------------------------------------------------------
$begin json parser_next_string$$

$section Get Next String$$

$head Syntax$$
$codei%
    %ok% = %json_parser%.next_char()
%$$

$head index_$$
The search for the string starts
at one greater than the input value for $code index_$$ and skips white space.

$head ok$$
is true if a string is the next token.

$head token_$$
If $icode ok$$ is true,
$icode token_$$ is set to the string excluding the
starting and ending double quote characters.
It is OK for the token to be empty.

$head Prototype$$
$srccode */
public:
    bool next_string(void);
/* $$
$end
-------------------------------------------------------------------------------
$begin json parser_next_non_neg_int$$

$section Get Next Non-Negative Integer$$

$head Syntax$$
$codei%
    %ok% = %json_parser%.next_non_neg_int()
%$$

$head index_$$
The search for the non-negative integer starts
at one greater than the input value for $code index_$$ and skips white space.

$head ok$$
is true if a non-negative integer is the next token.

$head token_$$
If $icode ok$$ is true,
$icode token_$$ is set to the non-negative integer.
The token will never be empty.

$head Prototype$$
$srccode */
public:
    bool next_non_neg_int(void);

/* $$
-------------------------------------------------------------------------------
$begin json parser_next_float$$

$section Get Next Floating Point Number$$

$head Syntax$$
$codei%
    %ok% = %json_parser%.next_float()
%$$

$head index_$$
The search for the floating point number starts
at one greater than the input value for $code index_$$ and skips white space.

$head ok$$
is true if a floating point number is the next token.

$head token_$$
If $icode ok$$ is true,
$icode token_$$ is set to the floating point number.
The token will never be empty.

$head Prototype$$
$srccode */
public:
    bool next_float(void);

/* $$
$end
*/

// ==========================================================================
}; // end class parser
// ==========================================================================


} } } // END_NAMESPACE_CPPAD_LOCAL_JSON


# endif
