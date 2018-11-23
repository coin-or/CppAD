# -----------------------------------------------------------------------------
# Plan for batch edit of all files:
# 1. Do not update copyright date when do this edit
# 6. Convert tabs to spaces.
# 7. Change CPPAD_TESTVECTOR to a template type (see Eigen entry in wishlist).
# 8. Change : B^n to \B{R}^n and other simuilar uses of B as a space.
# 9. Change Vector<T> -> <T>Vector for T = Base, Size, Bool (T != Set)
# 11. Change UserOp -> AfunOp, Usr[ar][vp]Op -> Afun[ar][vp]Op
# 12. Remove all doxygen \a commands (not used consistently)
# 13. Remove CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
# 14. Remove all inlines for functions that depend on template parameters.
# 15. Change 'ind[0-9] -> arg[0-9] when used as operator arguments.
# 17. Remove 'It returns true if it succeeds and false otherwise.'
# 18. Change template <class *> -> template <typename *>.
# 19. Create check_sort.sh and use it to sort all alphabetical lists.
# ----------------------------------------------------------------------------
# list of directories that are added to the repository by batch_edit.sh
# new_directories='
# '
# list of files that are deleted by batch_edit.sh
# delete_files='
# '
# list of files that are not edited by the sed commands in this file
# (with the possible exception of the extra_sed commands)
# ignore_files='
# '
# list of files and or directories that are moved to new names
# move_paths='
#	include/cppad/local/play/user_op_info.hpp
#	include/cppad/local/user_state.hpp
# '
# sed command that maps old file and or directory names to new file names
# move_sed='
#	s|user_|atom_|
# '
# list of files that get edited by the extra_sed command
# extra_files='
# '
# sed command that is applied to the extra files
# (after the other sed commands in this file)
# extra_sed='
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
s|user_state\.hpp|atom_state.hpp|g
s|USER_STATE_HPP|ATOM_STATE_HPP|
s|user_op_info|atom_op_info|
s|USER_OP_INFO|ATOM_OP_INFO|
#
s|enum_user_state|enum_atom_state|g
s|start_user|start_atom|g
s|arg_user|arg_atom|g
s|ret_user|ret_atom|g
s|end_user|end_atom|g
#
s|user_usrrp|atom_funrp|g
s|user_atom\([^a-z_]\)|atom_fun\1|g
#
s|user_r_pack|atomc_r_pack|g
s|user_s_pack|atomc_a_pack|g
s|user_r_bool|atomc_r_bool|g
s|user_s_bool|atomc_a_bool|g
s|user_r_set|atomc_r_set|g
s|user_s_set|atomc_a_set|g
#
s|user_state|atom_state|g
s|user_old|atom_old|g
#
s|user_\(i[xy][^a-z_]\)|atom_\1|g
s|user_\(i[xy]\)$|atom_\1|
#
s|user_\([mnijx][^a-z_]\)|atom_\1|g
s|user_\([mnijx]\)$|atom_\1|
