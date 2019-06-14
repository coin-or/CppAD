/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */
# include <cppad/local/json/parser.hpp>

// next_index
void CppAD::local::json::parser::next_index(void)
{   CPPAD_ASSERT_UNKNOWN( index_ < graph_.size() );
    if( graph_[index_] == '\r' )
    {   ++line_number_;
        char_number_ = 0;
    }
    ++index_;
    ++char_number_;
}

// skip_white_space
void CppAD::local::json::parser::skip_white_space(void)
{  while( index_ < graph_.size() && isspace( graph_[index_] ) )
        next_index();
}

// constructor
CppAD::local::json::parser::parser(const std::string& graph)
:
graph_(graph),
index_(0),
line_number_(1),
char_number_(1),
token_("")
{   skip_white_space();
    if( index_ < graph_.size() )
        token_ = graph_[index_];
    return;
}


// token
const std::string& CppAD::local::json::parser::token(void) const
{   return token_; }

// line_number
size_t CppAD::local::json::parser::line_number(void) const
{   return line_number_; }

// char_number
size_t CppAD::local::json::parser::char_number(void) const
{   return char_number_; }


// next_char
bool CppAD::local::json::parser::next_char(void)
{   if( index_ < graph_.size() )
        next_index();
    skip_white_space();
    if( index_ < graph_.size() )
    {   token_.resize(1);
        token_[0] = graph_[index_];
        return true;
    }
    return false;
}

// next_string
bool CppAD::local::json::parser::next_string(void)
{   // advance to next non white space character
    bool ok = next_char();
    if( ! ok )
        return false;
    //
    // check for "
    skip_white_space();
    if( graph_[index_] != '"' )
        return false;
    //
    token_.resize(0);
    next_index();
    while( index_ < graph_.size() && graph_[index_] != '"' )
    {   token_.push_back( graph_[index_] );
        next_index();
    }
    //
    // check for "
    if( graph_[index_] != '"' )
        return false;
    //
    return true;
}

// next_non_neg_int
bool CppAD::local::json::parser::next_non_neg_int(void)
{   // advance to next non white space character
    bool ok = next_char();
    if( ! ok )
        return false;
    ok = std::isdigit( graph_[index_] );
    if( ! ok )
        return false;
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
    return true;
}

// next_float
bool CppAD::local::json::parser::next_float(void)
{   // advance to next non white space character
    bool ok = next_char();
    if( ! ok )
        return false;
    char ch = graph_[index_];
    ok  = std::isdigit(ch);
    ok |= (ch == '.') | (ch == '+') | (ch == '-');
    ok |= (ch == 'e') | (ch == 'E');
    if( ! ok )
        return false;
    //
    token_.resize(0);
    while( ok )
    {   token_.push_back( graph_[index_] );
        ok = index_ + 1 < graph_.size();
        if( ok )
        {   ch  = graph_[index_ + 1];
            ok  = isdigit(ch);
            ok |= (ch == '.') | (ch == '+') | (ch == '-');
            ok |= (ch == 'e') | (ch == 'E');
        }
        if( ok )
            next_index();
    }
    return true;
}
