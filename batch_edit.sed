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
#   include/cppad/local/graph/operator.hpp
#   include/cppad/core/graph/json_op_define.omh
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
#   s|operator.hpp|cpp_graph_op.hpp|
#   s|json_op_define.omh|json_graph_op.omh|
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
s|/graph/operator.hpp|/graph/cpp_graph_op.hpp|
s|_GRAPH_OPERATOR_HPP|_GRAPH_CPP_GRAPH_OP_HPP|
s|/graph/json_op_define.omh|/graph/json_graph_op.omh|
#
s|$begin json_op_define|$begin json_graph_op|
s|$cref json_op_define|$cref json_graph_op|
s|/json_op_define/|/json_graph_op/|g
#
s|$begin graph_operator|$begin cpp_graph_operator|
s|$cref graph_operator|$cref cpp_graph_operator|
s|/graph_operator/|/cpp_graph_operator/|g
