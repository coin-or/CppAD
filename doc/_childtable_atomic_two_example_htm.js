// Child table for section atomic_two_example
document.write('\
<select onchange="atomic_two_example_child(this)">\
<option>atomic_two_example-&gt;</option>\
<option>atomic_two_eigen_mat_mul.cpp</option>\
<option>atomic_two_eigen_mat_inv.cpp</option>\
<option>atomic_two_eigen_cholesky.cpp</option>\
</select>\
');
function atomic_two_example_child(item)
{	var child_list = [
		'atomic_two_eigen_mat_mul.cpp.htm',
		'atomic_two_eigen_mat_inv.cpp.htm',
		'atomic_two_eigen_cholesky.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
