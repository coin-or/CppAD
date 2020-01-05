# Plan for batch Edit:
# 1. Remove tab from copyright message.
# 2. Fix following spelling errors:
#    comparision->comparison
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
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
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
s|num_vec_ind_rec|num_var_vecad_ind_rec|g
s|num_vecad_vec_rec|num_var_vecad_rec|g
s|AddVec(|add_var_vecad(|g
s|PutVecInd(|put_var_vecad_ind(|g
s|vecad_ind_vec_|all_var_vecad_ind_|g
s|num_load_op_rec_  |num_var_load_rec_ |g
s|num_load_op_rec|num_var_load_rec|g
s|// load_op_$|// load_op2var_|g
s|    f\.load_op_)|f.load_op2var_)|g
s|\([^a-zA-Z0-9_]\)load_op_     |\1load_op2var_ |g
s|\([^a-zA-Z0-9_]\)load_op_\([^a-zA-Z0-9_]\)|\1load_op2var_\2|g
