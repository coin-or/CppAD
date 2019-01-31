// Child table for section sparse_rc
document.write('\
<select onchange="sparse_rc_child(this)">\
<option>sparse_rc-&gt;</option>\
<option>sparse_rc.cpp</option>\
</select>\
');
function sparse_rc_child(item)
{	var child_list = [
		'sparse_rc.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
