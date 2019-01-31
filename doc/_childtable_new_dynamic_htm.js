// Child table for section new_dynamic
document.write('\
<select onchange="new_dynamic_child(this)">\
<option>new_dynamic-&gt;</option>\
<option>new_dynamic.cpp</option>\
</select>\
');
function new_dynamic_child(item)
{	var child_list = [
		'new_dynamic.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
