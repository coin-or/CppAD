# ----------------------------------------------------------------------------
# None of the lists below can have white space or a dollar sign in an entry.
#
# list of directories that are added to the repository by batch_edit.sh
# new_directories='
#   include/cppad/local/sparse
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
#   include/cppad/local/sparse_binary_op.hpp
#   include/cppad/local/sparse_internal.hpp
#   include/cppad/local/sparse_list.hpp
#   include/cppad/local/sparse_pack.hpp
#   include/cppad/local/sparse_sizevec.hpp
#   include/cppad/local/sparse_unary_op.hpp
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
#   s|sparse_|sparse/|
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
s|cppad/local/sparse_binary_op.hpp|cppad/local/sparse/binary_op.hpp|
s|cppad/local/sparse_internal.hpp|cppad/local/sparse/internal.hpp|
s|cppad/local/sparse_list.hpp|cppad/local/sparse/list.hpp|
s|cppad/local/sparse_pack.hpp|cppad/local/sparse/pack.hpp|
s|cppad/local/sparse_sizevec.hpp|cppad/local/sparse/sizevec.hpp|
s|cppad/local/sparse_unary_op.hpp|cppad/local/sparse/unary_op.hpp|
