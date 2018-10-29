# -----------------------------------------------------------------------------
# Plan for batch edit of all files:
# 1. Do not update copyright date when do this edit
# 2. Remove all svn Id commands and delete check_svn_dist.sh.
# 3. "included in the COPYING file" -> "included in the epl-v10.txt file"
# 4. Remove all omhelp index commands.
# 5. Change: $code $srcfile ...$$ $$ -> $srcfile ... $$
# 6. Convert tabs to spaces.
# 7. Change CPPAD_TESTVECTOR to a template type (see Eigen entry in wishlist).
# 8. Change : B^n to \B{R}^n and other simuilar uses of B as a space.
# 9. Change Vector<T> -> <T>Vector for T = Base, Size, Bool (T != Set)
# 10. Change http://www.coin-or.org/CppAD to new download server and figure
#     a way to distribute gpl version of repository.
# 11. Change UserOp -> AfunOp, Usr[ar][vp]Op -> Afun[ar][vp]Op
# 12. Remove all doxygen \a commands (not used consistently)
# 13. Remove CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
# 14. Remove all inlines for functions that depend on template parameters.
# 15. Change 'ind[0-9] -> arg[0-9] when used as operator arguments.
# 16. Remove all lines that have 'SHORT COPYRIGHT' on them.
# 17. Remove 'It returns true if it succeeds and false otherwise.'
# 18. Change template <class *> -> template <typename *>.
# 19. Create check_sort.sh and use it to sort all alphabetical lists.
# 20. Convert to EPL2 and remove the scripts that convert to GPL.
#     this includes removing gpl_license.sh.
# 21. Move test_more -> test.
# 22. Move cppad -> include/cppad.
# 23. Convert all $xref commands -> $cref commands.
# 24. Move whats_new/whats_new_yy.omh -> whats_new/yyyy.omh
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
#	omh/appendix/theory
# '
# sed command that maps old file and or directory names to new file names
# move_sed='
#	s|omh/appendix/theory|omh/theory|
# '
# list of files that get edited by the extra_sed command
# extra_files='
#	omh/theory/theory.omh
# '
# sed command that is applied to the extra files
# (after the other sed commands in this file)
# extra_sed='
#	s|^.contents%|&\n\tomh/theory/introduction.omh%|
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
