// Child table for section sparse_jacobian
document.write('\
<select onchange="sparse_jacobian_child(this)">\
<option>sparse_jacobian-&gt;</option>\
<option>sparse_jacobian.cpp</option>\
</select>\
');
function sparse_jacobian_child(item)
{	var child_list = [
		'sparse_jacobian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
