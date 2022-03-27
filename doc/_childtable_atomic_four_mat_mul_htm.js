// Child table for section atomic_four_mat_mul
document.write('\
<select onchange="atomic_four_mat_mul_child(this)">\
<option>atomic_four_mat_mul-&gt;</option>\
<option>atomic_four_mat_mul_implement</option>\
<option>atomic_four_mat_mul_forward.cpp</option>\
<option>atomic_four_mat_mul_reverse.cpp</option>\
<option>atomic_four_mat_mul_sparsity.cpp</option>\
<option>atomic_four_mat_mul_rev_depend.cpp</option>\
<option>atomic_four_mat_mul_identical_zero.cpp</option>\
</select>\
');
function atomic_four_mat_mul_child(item)
{	var child_list = [
		'atomic_four_mat_mul_implement.htm',
		'atomic_four_mat_mul_forward.cpp.htm',
		'atomic_four_mat_mul_reverse.cpp.htm',
		'atomic_four_mat_mul_sparsity.cpp.htm',
		'atomic_four_mat_mul_rev_depend.cpp.htm',
		'atomic_four_mat_mul_identical_zero.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
