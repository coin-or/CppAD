#! /bin/bash
#
list="
	NumIndTable\[
	NumInd(
	NumVarTable\[
	NumVar(
	printOpField(
	printOp(
"
for name in $list
do
	grep "$name" doxygen.log
done
