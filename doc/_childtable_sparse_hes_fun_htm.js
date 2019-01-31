// Child table for section sparse_hes_fun
document.write('\
<select onchange="sparse_hes_fun_child(this)">\
<option>sparse_hes_fun-&gt;</option>\
<option>sparse_hes_fun.cpp</option>\
<option>sparse_hes_fun.hpp</option>\
</select>\
');
function sparse_hes_fun_child(item)
{	var child_list = [
		'sparse_hes_fun.cpp.htm',
		'sparse_hes_fun.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
