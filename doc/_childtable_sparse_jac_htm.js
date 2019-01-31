// Child table for section sparse_jac
document.write('\
<select onchange="sparse_jac_child(this)">\
<option>sparse_jac-&gt;</option>\
<option>sparse_jac_for.cpp</option>\
<option>sparse_jac_rev.cpp</option>\
</select>\
');
function sparse_jac_child(item)
{	var child_list = [
		'sparse_jac_for.cpp.htm',
		'sparse_jac_rev.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
