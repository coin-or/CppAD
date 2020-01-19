// Child table for section colpack_prefix
document.write('\
<select onchange="colpack_prefix_child(this)">\
<option>colpack_prefix-&gt;</option>\
<option>colpack_jac.cpp</option>\
<option>colpack_jacobian.cpp</option>\
<option>colpack_hes.cpp</option>\
<option>colpack_hessian.cpp</option>\
</select>\
');
function colpack_prefix_child(item)
{	var child_list = [
		'colpack_jac.cpp.htm',
		'colpack_jacobian.cpp.htm',
		'colpack_hes.cpp.htm',
		'colpack_hessian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
