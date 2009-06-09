#! /bin/bash
# $Id$
# ----------------------------------------------------------------------------
# Each of the cases in ext_list and file_list must be handled by the
#	case "$ext" in
# statment in the script below.
#
# List of extensions that are handled by svn_add_id.sh
ext_list=" .sh .py .bat .c .h .cpp .hpp  .omh  .l .y " 
#
# List of specific file names that are handled by svn_add_id.sh
# (Assume no special grep pattern matching characters except for .) 
file_list=" makefile.am configure.ac "
#
# ----------------------------------------------------------------------------
command="$0"
directory="$1"
dry_run="$2"
if [ ! -d "$1" ] || ( [ "$2" != "" ] && [ "$2" != "--not-dry-run" ] )
then
	echo "\"$directory\" is not a directory"
	echo
	echo "usage: svn_add_id.sh directory [--not-dry-run]"
	echo
	echo "This command adds the Subversion keyword comamnd \$Id"
	echo "to files in and below the specified directory."
	echo
	echo "It also executes the command"
	echo "	svn propset svn:keywords Id set on name"
	echo "where name is the name of the file that was modified." 
	echo
	echo "If a file already has a Subversion \$Id comamnd, it is"
	echo "not modified, but its properity will still be set."
	echo
	echo "If --not-dry-run is missing, the proposed changes are listed,"
	echo "but no action is taken (neither file edits or property setting)."
	echo
	echo "The shell variables ext_list and file_list determine which files"
	echo "are checked; see the top of file $0" 
	exit 1
fi 
#
# list of files and subdirectories of the specified directory
dir_list=`ls "$directory"`
for name in $dir_list
do
	id_already="no"
	full_name="$directory/$name"
	if [ -d "$full_name" ]
	then
		# This is a directory so run svn_add_id.sh on it
		if ! $command "$full_name" "$dry_run"
		then
			echo "Aborted $command $full_name $dry_run"
			exit 1
		fi
		# do not process this file
		ext=""
	else
		ext=`echo $name | sed -e 's/.*\([.][^.]*\)/\1/' -e 's/[^.]*//'`
		pattern=`echo "$ext" | sed -e 's/\.\(.*\)/ [.]\1 /'`
		if ! echo "$ext_list" | grep "$pattern" > /dev/null
		then
			# Not in extension list so do not process this file
			ext=""
		else
			if grep '\$Id.*\$' $full_name > /dev/null
			then
				# $Id command is already in this file
				# so do not process it
				id_already="yes"
			fi
		fi
		pattern=`echo "$name" | sed -e 's/\./[.]/g' -e 's/.*/ & /'`
		if echo "$file_list" | grep "$pattern" > /dev/null
		then
			if grep '\$Id.*\$' $full_name > /dev/null
			then
				# $Id command is already in this file
				# so do not process it
				id_already="yes"
			fi
			ext="$name"
		fi
	fi
	if [ "$ext" != "" ] && [ "$id_already" == "no" ]
	then
		# Separate by cases in $ext_list or $file_list
		case "$ext" in
			# Shell or python script file. Put Id command after 
			# interpertor specification; e.g., ! /bin/bash
			.sh | .py )
			sed < $full_name > svn_add_id.$$ \
				-e '1,1s/^[^!]*$/# \$Id\$\n&/' \
				-e '1,1s/^# *!.*$/& \n# \$Id\$/'
			;;

			# Dos batch file
		       .bat )
			echo "rem \$Id\$" > svn_add_id.$$
			cat "$full_name" >> svn_add_id.$$
			;;

			# C and C++ source code
		       .c | .h | .cpp | .hpp )
			echo "/* \$Id\$ */" > svn_add_id.$$
			cat "$full_name" >> svn_add_id.$$
			;;

			# .l and .y bison and flex source code
		       .l | .y  )
			echo "/* \$Id\$ */" > svn_add_id.$$
			cat "$full_name" >> svn_add_id.$$
			;;

			# OMhelp files
			.omh )
			echo "\$Id\$" > svn_add_id.$$
			cat "$full_name" >> svn_add_id.$$
			;;

			# Automake input files
			makefile.am )
			sed < $full_name > svn_add_id.$$ \
				-e '1,1s/^.*$/# \$Id\$\n&/' 
			;;

			# Autoconf input files
			configure.ac )
			sed < $full_name > svn_add_id.$$ \
				-e '1,1s/^.*$/dnl \$Id\$\n&/' 
			;;

		esac
		# Must have matched one of the cases above because $ext is
		# in $ext_list of $file_list
		echo "$full_name"
		if diff $full_name svn_add_id.$$
		then
			echo "Cannot add \$Id command to $full_name"
			exit 1
		fi
		if [ "$dry_run" == "--not-dry-run" ]
		then
			if ! mv svn_add_id.$$ $full_name
			then
				echo "Cannot modify $full_name"
				exit 1
			fi
		else
			if ! rm svn_add_id.$$
			then
				echo "Cannot remove svn_add_id.$$"
				exit 1
			fi
		fi
	fi
	if [ "$ext" != "" ]
	then
		echo "svn propset svn:keywords Id $full_name"
		if [ "$dry_run" == "--not-dry-run" ]
		then
			if ! svn propset svn:keywords Id $full_name
			then
				echo "Cannot set Id keyword on for $full_name"
				exit 1
			fi
		fi
	fi
done
if [ -e svn_add_id.$$ ]
then
	echo "svn_add_id: file svn_add_id.$$ should have been deleted"
	exit 1
fi
exit 0
