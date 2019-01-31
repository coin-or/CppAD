// Child table for section sparse_jac_fun
document.write('\
<select onchange="sparse_jac_fun_child(this)">\
<option>sparse_jac_fun-&gt;</option>\
<option>sparse_jac_fun.cpp</option>\
<option>sparse_jac_fun.hpp</option>\
</select>\
');
function sparse_jac_fun_child(item)
{	var child_list = [
		'sparse_jac_fun.cpp.htm',
		'sparse_jac_fun.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
