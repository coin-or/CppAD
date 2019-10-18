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
#   include/cppad/local/json/lexer.hpp
#   include/cppad/local/json/parser.hpp
#   include/cppad/local/json/writer.hpp
#   include/cppad/local/json/dev_json.omh
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
#   s|/json/lexer.hpp|/json/json_lexer.hpp|
#   s|/json/parser.hpp|/json/json_parser.hpp|
#   s|/json/writer.hpp|/json/json_writer.hpp|
#   s|/json/dev_json.omh|/json/dev_graph.omh|
# '
# list of files that get edited by the extra_seds command
# extra_files='
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# extra_seds='
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
s|/json/lexer.hpp|/json/json_lexer.hpp|g
s|_JSON_LEXER_HPP|_JSON_JSON_LEXER_HPP|g
#
s|/json/parser.hpp|/json/json_parser.hpp|g
s|_JSON_PARSER_HPP|_JSON_JSON_PARSER_HPP|g
#
s|/json/writer.hpp|/json/json_writer.hpp|g
s|_JSON_WRITER_HPP|_JSON_JSON_WRITER_HPP|g
#
s|/json/dev_json.omh|/json/dev_graph.omh|g

