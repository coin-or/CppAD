// Child table for section atomic_four_example
document.write('\
<select onchange="atomic_four_example_child(this)">\
<option>atomic_four_example-&gt;</option>\
<option>atomic_four_get_started.cpp</option>\
<option>atomic_four_norm_sq.cpp</option>\
<option>atomic_vector</option>\
<option>atomic_mat_mul</option>\
</select>\
');
function atomic_four_example_child(item)
{	var child_list = [
		'atomic_four_get_started.cpp.htm',
		'atomic_four_norm_sq.cpp.htm',
		'atomic_vector.htm',
		'atomic_mat_mul.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
