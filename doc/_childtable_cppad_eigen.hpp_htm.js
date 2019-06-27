// Child table for section cppad_eigen.hpp
document.write('\
<select onchange="cppad_eigen__46__hpp_child(this)">\
<option>cppad_eigen.hpp-&gt;</option>\
<option>eigen_plugin.hpp</option>\
<option>eigen_array.cpp</option>\
<option>eigen_det.cpp</option>\
</select>\
');
function cppad_eigen__46__hpp_child(item)
{	var child_list = [
		'eigen_plugin.hpp.htm',
		'eigen_array.cpp.htm',
		'eigen_det.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
