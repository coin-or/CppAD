# ----------------------------------------------------------------------------
# None of the lists below can have white space or a dollar sign in an entry.
#
# list of directories that are added to the repository by batch_edit.sh
# new_directories='
# '
# list of files that are deleted by batch_edit.sh
# delete_files='
# '
# List of files that are not edited by the sed commands in this file
# (with the possible exception of the extra_seds commands).
# The files in bin/devel.sh ignore_files are automatically in this list.
# ignore_files='
# '
# list of files and or directories that are moved to new names
# move_paths='
#   cppad_lib/json_parser.cpp
#   test_more/general/local/json_parser.cpp
#   include/cppad/local/json/parser.hpp
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
#   s|json_parser|json_lexer|
#   s|json/parser|json/lexer|
# '
# list of files that get edited by the extra_seds command
# extra_files='
#   cppad_lib/json_lexer.cpp
#   include/cppad/local/json/lexer.hpp
#   test_more/general/local/json_lexer.cpp
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# extra_seds='
#   s|parser|lexer|g
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
s|json_prser,|json_lexer, |
s|JSON_PARSER_HPP|JSON_LEXER_HPP|
s|json\([/_ ]\)parser|json\1lexer|
