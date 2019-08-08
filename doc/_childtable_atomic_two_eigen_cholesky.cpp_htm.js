// Child table for section atomic_two_eigen_cholesky.cpp
document.write('\
<select onchange="atomic_two_eigen_cholesky.cpp_child(this)">\
<option>atomic_two_eigen_cholesky.cpp-&gt;</option>\
<option>cholesky_theory</option>\
<option>atomic_two_eigen_cholesky.hpp</option>\
</select>\
');
function atomic_two_eigen_cholesky.cpp_child(item)
{	var child_list = [
		'cholesky_theory.htm',
		'atomic_two_eigen_cholesky.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
