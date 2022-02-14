// Child table for section atomic_vector
document.write('\
<select onchange="atomic_vector_child(this)">\
<option>atomic_vector-&gt;</option>\
<option>atomic_vector_implement</option>\
<option>atomic_four_vector_add.cpp</option>\
<option>atomic_four_vector_div.cpp</option>\
<option>atomic_four_vector_hes_sparsity.cpp</option>\
<option>atomic_four_vector_jac_sparsity.cpp</option>\
<option>atomic_four_vector_mul.cpp</option>\
<option>atomic_four_vector_neg.cpp</option>\
<option>atomic_four_vector_rev_depend.cpp</option>\
<option>atomic_four_vector_sub.cpp</option>\
</select>\
');
function atomic_vector_child(item)
{	var child_list = [
		'atomic_vector_implement.htm',
		'atomic_four_vector_add.cpp.htm',
		'atomic_four_vector_div.cpp.htm',
		'atomic_four_vector_hes_sparsity.cpp.htm',
		'atomic_four_vector_jac_sparsity.cpp.htm',
		'atomic_four_vector_mul.cpp.htm',
		'atomic_four_vector_neg.cpp.htm',
		'atomic_four_vector_rev_depend.cpp.htm',
		'atomic_four_vector_sub.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
