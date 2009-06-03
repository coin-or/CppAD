#! /bin/bash
#
list="
	op_code.hpp
	player.hpp
"
for name in $list
do
	if grep -i "$name.*warning" doxygen.log
	then
		echo "Unexpected doxygen error or warning for $name."
		exit 1
	fi
done
