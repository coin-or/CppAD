
#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/fix_comp_op.sh" ]
then
	echo "bin/fix_comp_op.sh: must be executed from its parent directory"
	exit 1
fi
old_hash='b4c0e5'
new_hash='611e98'
# -----------------------------------------------------------------------------
# cd /home/bradbell/repo/cppad.git
# -----------------------------------------------------------------------------
# get newer version of cppad
git checkout --quiet $new_hash
git reset --quiet --hard
#
# configure
echo "bin/run_cmake.sh >& /dev/null"
bin/run_cmake.sh >& /dev/null
# -----------------------------------------------------------------------------
# run speed test with no changes
cd build/speed/cppad
#
# build speed test
echo "make check_speed_cppad > /dev/null"
make check_speed_cppad > /dev/null
#
# run speed test
echo "new_has with no changes"
./speed_cppad det_lu 125
#
cd ../../..
# -----------------------------------------------------------------------------
dir='cppad/local'
git show $old_hash:$dir/compare.hpp > $dir/compare_old.hpp
git show $old_hash:$dir/comp_op.hpp > $dir/comp_op_old.hpp
# -----------------------------------------------------------------------------
file='compare_old.hpp'
sed -i $dir/$file \
	-e 's|COMPARE_INCLUDED|COMPARE_OLD_INCLUDED|'
#
file='comp_op_old.hpp'
sed -i $dir/$file \
	-e 's|COMP_OP_INCLUDED|COMP_OP_OLD_INCLUDED|'
#
file='op.hpp'
sed -i $dir/$file \
	-e '/comp_op.hpp/s|$|\n# include <cppad/local/comp_op_old.hpp>|'
# -----------------------------------------------------------------------------
file='bool_valued.hpp'
sed -i $dir/$file \
	-e 's|cppad/local/compare.hpp|cppad/local/compare_old.hpp|'
# -----------------------------------------------------------------------------
cat << EOF > junk.sed
/RecordCondExp/! b skip
: loop
N
/\\n\\t);$/! b loop
s|$|\\
\\
	// record a comparison operator\\
	void RecordCompare(\\
		enum CompareOp  cop      ,\\
		bool            result   ,\\
		const AD<Base> \\&left     ,\\
		const AD<Base> \\&right\\
	);\\
|
: skip
EOF
file='ad_tape.hpp'
echo_eval sed -i $dir/$file -f junk.sed
# -----------------------------------------------------------------------------
cat << EOF > junk.sed
/case CExpOp:/! b skip
: loop
N
/\\n\\t\\t\\tbreak;$/! b loop
s|$|\\
			// ---------------------------------------------------\\
\\
			case ComOp:\\
			CPPAD_ASSERT_NARG_NRES(op, 4, 0);\\
			CPPAD_ASSERT_UNKNOWN( arg[1] > 1 );\\
			break;|
: skip
EOF
list='
	for_jac_sweep.hpp
	rev_jac_sweep.hpp
	rev_hes_sweep.hpp
	reverse_sweep.hpp
'
for file in $list
do
	echo_eval sed -i $dir/$file -f junk.sed
done
# -----------------------------------------------------------------------------
cat << EOF > junk.sed
/case CExpOp:/! b skip
: loop
N
/\\n\\t\\t\\tbreak;$/! b loop
s|$|\\
			// ---------------------------------------------------\\
\\
			case ComOp:\\
			if( compare_change_count )\\
			{	forward_comp_op_0(\\
					compare_change_number, arg, num_par, parameter, J, taylor\\
				);\\
				if( compare_change_count == compare_change_number )\\
					compare_change_op_index = i_op;\\
			}\\
			break;|
: skip
EOF
file='forward0sweep.hpp'
echo_eval sed -i $dir/$file -f junk.sed
# -----------------------------------------------------------------------------
cat << EOF > junk.sed
/case CExpOp:/! b skip
: loop
N
/\\n\\t\\t\\tbreak;$/! b loop
s|$|\\
			// ---------------------------------------------------\\
\\
			case ComOp:\\
			if( ( p == 0 ) \\& ( compare_change_count ) )\\
			{	forward_comp_op_0(\\
					compare_change_number, arg, num_par, parameter, J, taylor\\
				);\\
				if( compare_change_count == compare_change_number )\\
					compare_change_op_index = i_op;\\
			}\\
			break;|
: skip
EOF
file='forward1sweep.hpp'
echo_eval sed -i $dir/$file -f junk.sed
# -----------------------------------------------------------------------------
cat << EOF > junk.sed
/case CExpOp:/! b skip
: loop
N
/\\n\\t\\t\\tbreak;$/! b loop
s|$|\\
			// ---------------------------------------------------\\
\\
			case ComOp:\\
			CPPAD_ASSERT_UNKNOWN(q > 0 );\\
			break;|
: skip
EOF
file='forward2sweep.hpp'
echo_eval sed -i $dir/$file -f junk.sed
# -----------------------------------------------------------------------------
cat << EOF > junk.sed
/^\\tCosOp,/! b one
s|^|\\tComOp,    // Compare(cop, result, left, right)\\n|
#
: one
/^\\t\\t1, \/\/ CosOp/! b two
s|^|\\t\\t4, // ComOp\\n|
#
: two
/^\\t\\t2, \/\/ CosOp/! b three
s|^|\\t\\t0, // ComOp\\n|
#
: three
/^\\t\\t"Cos"    ,/! b four
s|^|\\t\\t"Com"    ,\\n|
#
: four
/5 arguments, no result/! b six
s|^|\\t\\t// 4 arguments, no result\\
		case ComOp:\\
		if( arg[1] \\& 2 )\\
		{	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) <= result );\\
		}\\
		if( arg[1] \\& 4 )\\
		{	CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) <= result );\\
		}\\
		break;\\
		// -------------------------------------------------------------------\\
\\
|
: six
EOF
file='op_code.hpp'
echo_eval sed -i $dir/$file -f junk.sed
# -----------------------------------------------------------------------------
cat << EOF > junk.sed
/Compare operators never get removed/! b one
s|$|\\n\t\\t\\tcase ComOp:\\
			if( arg[1] \\& 2 )\\
				tape[arg[2]] .connect_type = yes_connected;\\
			if( arg[1] \\& 4 )\\
				tape[arg[3]] .connect_type = yes_connected;\\
			break;\\
|
: one
/see wish_list\\/Optimize\\/CompareChange entry/! b two
s|$|\\n\\t\\t\\tcase ComOp:|
: two
/---------------------------------------------------\$/N
/---\\n\\t\\t\\t\/\/ Conditional expression operators/! b three
s|^|\\t\\t\\t// -------------------------------------------------------------\\
			// Comparison operators, 4 arguments no result\\
			case ComOp:\\
			new_arg[0] = arg[0];\\
			new_arg[1] = arg[1];\\
			if( arg[1] \\& 2 )\\
			{	new_arg[2] = tape[arg[2]].new_var;\\
				CPPAD_ASSERT_UNKNOWN( size_t(new_arg[2]) < num_var );\\
			}\\
			else\\
			{	new_arg[2] = rec->PutPar( play->GetPar( arg[2] ) );\\
			}\\
			if( arg[1] \\& 4 )\\
			{	new_arg[3] = tape[arg[3]].new_var;\\
				CPPAD_ASSERT_UNKNOWN( size_t(new_arg[3]) < num_var );\\
			}\\
			else\\
			{	new_arg[3] = rec->PutPar( play->GetPar( arg[3] ) );\\
			}\\
			rec->PutArg(new_arg[0], new_arg[1], new_arg[2], new_arg[3]);\\
			rec->PutOp(op);\\
			break;\\
|
: three
EOF
file='optimize.hpp'
echo_eval sed -i $dir/$file -f junk.sed
# -----------------------------------------------------------------------------
# run speed test with changes
cd build/speed/cppad
#
# build speed test
echo "make check_speed_cppad > /dev/null"
make check_speed_cppad > /dev/null
#
# run speed test
echo "new_has with no changes"
./speed_cppad det_lu 125
#
cd ../../..
# -----------------------------------------------------------------------------
echo 'fix_comp_op: OK'
exit 0
