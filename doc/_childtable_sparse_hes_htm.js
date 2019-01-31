// Child table for section sparse_hes
document.write('\
<select onchange="sparse_hes_child(this)">\
<option>sparse_hes-&gt;</option>\
<option>sparse_hes.cpp</option>\
</select>\
');
function sparse_hes_child(item)
{	var child_list = [
		'sparse_hes.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
