/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */
# include <cppad/local/json/lexer.hpp>
# include <cppad/local/json/operator.hpp>
# include <cppad/utility/error_handler.hpp>
# include <cppad/utility/to_string.hpp>
# include <cppad/utility/thread_alloc.hpp>


// BEGIN_CPPAD_LOCAL_JSON_NAMESPACE
namespace CppAD { namespace local { namespace json {

// report_error
void lexer::report_error(
    const std::string& expected ,
    const std::string& found    )
{   std::string msg = "Error occured while parsing Json AD graph.\n";
    msg += "Expected a " + expected + " token but found " + found;
    msg +="\nDetected at character number " + to_string(char_number_);
    msg +=" in line number " + to_string(line_number_);
    msg += " of the graph.\n";
    msg += "See https://coin-or.github.io/CppAD/doc/json_ad_graph.htm.";
    //
    // use this source code as point of detection
    bool known       = true;
    int  line        = __LINE__;
    const char* file = __FILE__;
    const char* exp  = "false";
    //
    // CppAD error handler
    ErrorHandler::Call(known, line, file, exp, msg.c_str());
}

// next_index
void lexer::next_index(void)
{   CPPAD_ASSERT_UNKNOWN( index_ < graph_.size() );
    if( graph_[index_] == '\n' )
    {   ++line_number_;
        char_number_ = 0;
    }
    ++index_;
    ++char_number_;
}

// skip_white_space
void lexer::skip_white_space(void)
{  while( index_ < graph_.size() && isspace( graph_[index_] ) )
        next_index();
}

// constructor
lexer::lexer(const std::string& graph)
:
graph_(graph),
index_(0),
line_number_(1),
char_number_(1),
token_("")
{   // make sure op_name2enum has been initialized
    if( op_name2enum.size() == 0 )
    {   CPPAD_ASSERT_KNOWN( ! thread_alloc::in_parallel() ,
            "First call to json graph lexer called in parallel mode"
        );
        set_op_name2enum();
    }

    skip_white_space();
    if( index_ < graph_.size() )
        token_ = graph_[index_];
    if( token_ != "{" )
    {   std::string expected = "'{'";
        std::string found    = "'";
        if( index_ < graph_.size() )
            found += graph_[index_];
        found += "'";
        report_error(expected, found);
    }
    return;
}


// token
const std::string& lexer::token(void) const
{   return token_; }

// line_number
size_t lexer::line_number(void) const
{   return line_number_; }

// char_number
size_t lexer::char_number(void) const
{   return char_number_; }

// token2size_t
size_t lexer::token2size_t(void) const
{   return size_t( std::atoi( token_.c_str() ) ); }

// token2double
double lexer::token2double(void) const
{   return std::atof( token_.c_str() ); }

// check_next_char
void lexer::check_next_char(char ch)
{   // advance to next character
    if( index_ < graph_.size() )
        next_index();
    skip_white_space();
    //
    bool ok = false;
    if( index_ < graph_.size() )
    {   token_.resize(1);
        token_[0] = graph_[index_];
        ok = token_[0] == ch;
    }
    if( ! ok )
    {   std::string expected;
        expected += "'";
        expected += ch;
        expected += "'";
        //
        std::string found = "'";
        found += graph_[index_];;
        found += "'";
        report_error(expected, found);
    }
}

// check_next_string
void lexer::check_next_string(const std::string& expected)
{   // advance to next character
    bool found_first_quote = index_ < graph_.size();
    if( found_first_quote )
    {   next_index();
        skip_white_space();
        found_first_quote = index_ < graph_.size();
    }
    // check for "
    if( found_first_quote )
        found_first_quote = graph_[index_] == '"';
    //
    // set value of token
    token_.resize(0);
    if( found_first_quote )
    {   next_index();
        while( index_ < graph_.size() && graph_[index_] != '"' )
        {   token_.push_back( graph_[index_] );
            next_index();
        }
    }
    // check for "
    bool found_second_quote = false;
    if( found_first_quote && index_ < graph_.size() )
        found_second_quote = graph_[index_] == '"';
    //
    bool ok = found_first_quote & found_second_quote;
    if( ok & (expected != "" ) )
        ok = expected == token_;
    if( ! ok )
    {   std::string expected_token;
        if( expected == "" )
            expected_token = "string";
        else
        {   expected_token = '"';
            expected_token += expected;
            expected_token += '"';
        }
        //
        std::string found;
        if( ! found_first_quote )
        {   found = "'";
            if( index_ < graph_.size() )
                found += graph_[index_];
            found += "'";
        }
        else
        {   found += '"';
            found += token_;
            if( found_second_quote )
                found += '"';
        }
        report_error(expected_token, found);
    }
}

// next_non_neg_int
void lexer::next_non_neg_int(void)
{   // advance to next character
    bool ok = index_ < graph_.size();
    if( ok )
    {   next_index();
        skip_white_space();
        ok = index_ < graph_.size();
    }
    if( ok )
        ok = std::isdigit( graph_[index_] );
    if( ! ok )
    {   std::string expected_token = "non-negative integer";
        std::string found = "'";
        if( index_ < graph_.size() )
            found += graph_[index_];
        found += "'";
        report_error(expected_token, found);
    }
    //
    token_.resize(0);
    while( ok )
    {   token_.push_back( graph_[index_] );
        ok = index_ + 1 < graph_.size();
        if( ok )
            ok = isdigit( graph_[index_ + 1] );
        if( ok )
            next_index();
    }
}

// next_float
void lexer::next_float(void)
{   // advance to next character
    bool ok = index_ < graph_.size();
    if( ok )
    {   next_index();
        skip_white_space();
        ok = index_ < graph_.size();
    }
    if( ok )
    {   char ch = graph_[index_];
        ok = std::isdigit(ch);
        ok |= (ch == '.') | (ch == '+') | (ch == '-');
        ok |= (ch == 'e') | (ch == 'E');
    }
    if( ! ok )
    {   std::string expected_token = "floating point number";
        std::string found = "'";
        if( index_ < graph_.size() )
            found += graph_[index_];
        found += "'";
        report_error(expected_token, found);
    }
    //
    token_.resize(0);
    while( ok )
    {   token_.push_back( graph_[index_] );
        ok = index_ + 1 < graph_.size();
        if( ok )
        {   char ch  = graph_[index_ + 1];
            ok  = isdigit(ch);
            ok |= (ch == '.') | (ch == '+') | (ch == '-');
            ok |= (ch == 'e') | (ch == 'E');
        }
        if( ok )
            next_index();
    }
    return;
}

} } } // END_CPPAD_LOCAL_JSON_NAMESPACE
