// Child table for section to_string
document.write('\
<select onchange="to_string_child(this)">\
<option>to_string-&gt;</option>\
<option>to_string.cpp</option>\
</select>\
');
function to_string_child(item)
{	var child_list = [
		'to_string.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
