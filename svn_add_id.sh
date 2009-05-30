#! /bin/bash
# $Id$
# ----------------------------------------------------------------------------
# Each of the cases in ext_list and file_list must be handled by the
#	case "$ext" in
# statment in the script below.
#
# List of extensions that are handled by svn_add_id.sh.
# Spaces must surround each entry.
ext_list=" .sh .py .bat .c .h .cpp .hpp  .omh " 
#
# List of specific file names that are handled by svn_add_id.sh
# Spaces must surround each entry.
# No special grep pattern matching characters, except for '.', can be used.
file_list=" makefile.am configure.ac "
#
# ----------------------------------------------------------------------------
command="$0"
directory="$1"
dry_run="$2"
if [ ! -d "$1" ] || ( [ "$2" != "" ] && [ "$2" != "--dry-run" ] )
then
	echo "\"$directory\" is not a directory"
	echo
	echo "usage: svn_add_id.sh directory --dry-run"
	echo
	echo "This command adds the Subversion keyword comamnd \$Id"
	echo "to files in and below the specified directory."
	echo
	echo "If a file already has a Subversion \$Id comamnd, it is"
	echo "not modified."
	echo
	echo "If --dry-run is specified, proposed changes are listed,"
	echo "but no action is taken."
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
	if [ -d "$directory/$name" ]
	then
		# This is a directory so run svn_add_id.sh on it
		if ! $command "$directory/$name" "$dry_run"
		then
			echo "Aborted $command $directory/$name $dry_run"
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
			if grep '\$Id.*\$' $directory/$name > /dev/null
			then
				# $Id command is already in this file
				# so do not process it
				ext=""
			fi
		fi
		pattern=`echo "$name" | sed -e 's/\./[.]/g' -e 's/.*/ & /'`
		if echo "$file_list" | grep "$pattern" > /dev/null
		then
			if grep '\$Id.*\$' $directory/$name > /dev/null
			then
				# $Id command is already in this file
				# so do not process it
				ext=""
			fi
			ext="$name"
		fi
	fi
	if [ "$ext" != "" ]
	then
		# Separate by cases in $ext_list or $file_list
		case "$ext" in
			# Shell or python script file. Put Id command after 
			# interpertor specification; e.g., ! /bin/bash
			.sh | .py )
			sed < $directory/$name > svn_add_id.$$ \
				-e '1,1s/^[^!]*$/# $Id$\n&/' \
				-e '1,1s/^# *!.*$/& \n# $Id$/'
			;;

			# Dos batch file
		       .bat )
			echo "rem \$Id\$" > svn_add_id.$$
			cat "$directory/$name" >> svn_add_id.$$
			;;

			# C and C++ source code
		       .c | .h | .cpp | .hpp )
			echo "/* \$Id\$ */" > svn_add_id.$$
			cat "$directory/$name" >> svn_add_id.$$
			;;

			# OMhelp files
			.omh )
			echo "\$Id\$" > svn_add_id.$$
			cat "$directory/$name" >> svn_add_id.$$
			;;

			# Automake input files
			makefile.am )
			sed < $directory/$name > svn_add_id.$$ \
				-e '1,1s/^.*$/# $Id$\n&/' 
			;;

			# Autoconf input files
			configure.ac )
			sed < $directory/$name > svn_add_id.$$ \
				-e '1,1s/^.*$/dnl $Id$\n&/' 
			;;

		esac
		# Must have matched one of the cases above because $ext is
		# in $ext_list of $file_list
		echo "$directory/$name"
		if diff $directory/$name svn_add_id.$$
		then
			echo "Cannot add \$Id command to $directory/$name"
			exit 1
		fi
		if [ "$dry_run" == "" ]
		then
			if ! mv svn_add_id.$$ $directory/$name
			then
				echo "Cannot modify $directory/$name"
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
done
if [ -e svn_add_id.$$ ]
then
	echo "svn_add_id: file svn_add_id.$$ should have been deleted"
	exit 1
fi
exit 0
