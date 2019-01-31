// Child table for section sinh
document.write('\
<select onchange="sinh_child(this)">\
<option>sinh-&gt;</option>\
<option>sinh.cpp</option>\
</select>\
');
function sinh_child(item)
{	var child_list = [
		'sinh.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
