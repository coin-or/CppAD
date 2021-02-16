// Child table for section function_name
document.write('\
<select onchange="function_name_child(this)">\
<option>function_name-&gt;</option>\
<option>function_name.cpp</option>\
</select>\
');
function function_name_child(item)
{	var child_list = [
		'function_name.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
