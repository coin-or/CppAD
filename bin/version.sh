# ! /bin/bash -e
# This file was written by Bradley M. Bell and is used by multiple packages.
# This version has the same license as the other files in this package.
# -----------------------------------------------------------------------------
echo_eval() {
     echo $*
     eval $*
}
# -----------------------------------------------------------------------------
# package
if ! grep '^PROJECT([a-zA-Z_]*)$' CMakeLists.txt > /dev/null
then
	echo 'Cannot determine package for this directory.'
	exit 1
else
	package=`grep '^PROJECT([a-zA-Z_]*)$' CMakeLists.txt |
		sed -e 's|PROJECT(||' -e 's|)||'`
fi
# -----------------------------------------------------------------------------
case $package in
	#
	cppad)
	file_list='doc.omh configure.ac'
	;;
	#
	cppad_swig)
	file_list='doc.omh'
	;;
	#
	cppad_mixed)
	file_list='doc.omh'
	;;
	#
	dismod_at)
	file_list='doc.omh bin/setup.py.in'
	;;
	#
	*)
	echo "version.sh: does not yet reconize the package $package"
	echo "See $HOME/$package.git/bin/version.sh for how to fix this."
	exit 1
	;;
esac
# -----------------------------------------------------------------------------
ok='yes'
if [ "$1" != 'get' ] \
&& [ "$1" != 'set' ] \
&& [ "$1" != 'date' ] \
&& [ "$1" != 'copy' ] \
&& [ "$1" != 'check' ]
then
	ok='no'
fi
if [ "$1" == 'set' ]
then
	if [ "$2" == '' ]
	then
		ok='no'
	fi
	if [ "$3" != '' ]
	then
		ok='no'
	fi
else
	if [ "$2" != '' ]
	then
		ok='no'
	fi
fi
if [ "$ok" != 'yes' ]
then
	echo "version.sh $*"
	echo 'usage: version.sh (get|date|copy|check)'
	echo '       version.sh set version'
	echo 'get:   returns date in CMakeLists.txt'
	echo 'set:   sets version in CMakeLists.txt'
	echo 'date:  sets version in CMakeLists.txt to current date'
	echo 'copy:  copies version in CMakeLists.txt to other files'
	echo 'check: checks that version number has been copied'
	exit 1
fi
cmd="$1"
# -----------------------------------------------------------------------------
# determine version number
if [ ! -f CMakeLists.txt ]
then
	echo 'version.sh: cannot find ./CMakeLists.txt'
	exit 1
fi
cat << EOF > version.$$
/^SET *( *${package}_version *"[0-9]\{8\}[0-9.]*" *)/! b skip
s|^SET *( *${package}_version *"\\([0-9]\{8\}[0-9.]*\\)" *)|\1|
p
: skip
EOF
# version number in CMakeLists.txt
version=`sed -n -f version.$$ CMakeLists.txt`
rm version.$$
if ! (echo $version | grep '[0-9]\{8\}') > /dev/null
then
	echo "version.sh: Cannot find ${package}_verison number in CMakeLists.txt"
	exit 1
fi
if [ "$cmd" == 'set' ]
then
	# version number on command line
	version="$2"
fi
if [ "$cmd" == 'date' ]
then
	# version number corresponding to current date
	version=`date +%Y%m%d`
fi
# -----------------------------------------------------------------------------
if [ "$cmd" == 'get' ]
then
	echo "$version"
	exit 0
fi
# -----------------------------------------------------------------------------
# cases where are are setting the version
if [ "$cmd" == 'set' ] || [ "$cmd" == 'date' ]
then
cat << EOF > version.$$
s|^SET *( *${package}_version *"[0-9.]*" *)|SET(${package}_version "$version")|
EOF
	sed  -i.old CMakeLists.txt -f version.$$
	rm version.$$
	if diff CMakeLists.txt.old CMakeLists.txt
	then
		echo 'No change to CMakeLists.txt'
	fi
	rm CMakeLists.txt.old
	#
	echo 'version.sh set: OK'
	exit 0
fi
# -----------------------------------------------------------------------------
change='no'
for file in $file_list
do
sed < $file > $file.copy \
-e "s|$package-[0-9]\\{8\\}[0-9.]*|${package}-$version|" \
-e "s|version *= *'[0-9]\\{8\\}[0-9.]*'|version = '$version'|" \
-e "s|AC_INIT(\[cppad\], \[[0-9]\\{8\\}[0-9.]*\]|AC_INIT([cppad], [$version]|"
	if [ -x "$file" ]
	then
		chmod +x $file.copy
	fi
	if ! diff $file $file.copy > /dev/null
	then
		echo '-------------------------------------------------------------'
		echo "diff $file"
		if diff $file $file.copy
		then
			echo 'version.sh: program error'
			exit 1
		else
			if [ "$cmd" == 'copy' ]
			then
				mv $file.copy $file
				change='yes'
			fi
			if [ "$cmd" == 'check' ]
			then
				ok='no'
				rm $file.copy
			fi
		fi
	else
		rm $file.copy
	fi
done
if [ "$ok" != 'yes' ]
then
	echo 'version.sh check: Found differences.'
	exit 1
fi
if [ "$change" == 'yes' ]  && [ "$package" == 'cppad' ]
then
	# update files that depend on version number in configure.ac
	bin/autotools.sh automake
fi
# ----------------------------------------------------------------------------
if [ "$cmd" != 'copy' ] && [ "$cmd" != 'check' ]
then
	echo 'version.sh: program error'
	exit 1
fi
echo "version.sh $cmd: OK"
exit 0
