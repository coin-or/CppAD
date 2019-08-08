// Child table for section atomic_two_eigen_mat_inv.cpp
document.write('\
<select onchange="atomic_two_eigen_mat_inv.cpp_child(this)">\
<option>atomic_two_eigen_mat_inv.cpp-&gt;</option>\
<option>atomic_two_eigen_mat_inv.hpp</option>\
</select>\
');
function atomic_two_eigen_mat_inv.cpp_child(item)
{	var child_list = [
		'atomic_two_eigen_mat_inv.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
