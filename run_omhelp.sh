# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
if [ "$1" != "doc" ] && [ "$1" != "dev" ]
then
	echo "run_omhelp.sh target: where target is doc or dev"
	exit
fi
if [ "$1" = dev ]
then
	rm -r -f dev
	#
	echo "Building developer documentation"
	mkdir dev
	cd    dev
	if ! omhelp ../dev.omh -noframe -xml -debug > ../omhelp_dev.log
	then
		grep "^OMhelp Error:" ../omhelp_dev.log
		echo "OMhelp could not build developer documentation."
		echo "See the complete error message in omhelp_dev.log"
		exit 1
	fi
	if grep "^OMhelp Warning:" omhelp_dev.log
	then
		echo "See the complete warning messages in omhelp_dev.log."
		exit 1
	fi
	omhelp ../dev.omh -noframe -debug
	cd ..
fi
if [ "$1" = doc ]
then
	#
	rm -r -f doc
	echo "Building user documentation"
	#
	mkdir doc
	cd    doc
	if ! omhelp ../doc.omh > ../omhelp_doc.log \
		-l http://www.coin-or.org/CppAD/ \
		-noframe \
		-xml \
		-debug
	then
		grep "^OMhelp Error:" ../omhelp_doc.log
		echo "OMhelp could not build user documentation."
		echo "See the complete error message in omhelp_doc.log."
		exit 1
	fi
	if grep "^OMhelp Warning:" omhelp_doc.log
	then
		echo "See the complete warning messages in omhelp_doc.log."
		exit 1
	fi
	omhelp ../doc.omh  \
                -l http://www.coin-or.org/CppAD/ \
		-noframe \
		-debug
	cd ..
fi
exit 0
