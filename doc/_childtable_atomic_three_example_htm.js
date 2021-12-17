// Child table for section atomic_three_example
document.write('\
<select onchange="atomic_three_example_child(this)">\
<option>atomic_three_example-&gt;</option>\
<option>atomic_three_get_started.cpp</option>\
<option>atomic_three_norm_sq.cpp</option>\
<option>atomic_three_tangent.cpp</option>\
<option>atomic_three_base2ad.cpp</option>\
<option>atomic_three_reciprocal.cpp</option>\
<option>atomic_three_mat_mul.cpp</option>\
<option>atomic_three_vector_op.cpp</option>\
</select>\
');
function atomic_three_example_child(item)
{	var child_list = [
		'atomic_three_get_started.cpp.htm',
		'atomic_three_norm_sq.cpp.htm',
		'atomic_three_tangent.cpp.htm',
		'atomic_three_base2ad.cpp.htm',
		'atomic_three_reciprocal.cpp.htm',
		'atomic_three_mat_mul.cpp.htm',
		'atomic_three_vector_op.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
