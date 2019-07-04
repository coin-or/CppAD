// Child table for section to_json
document.write('\
<select onchange="to_json_child(this)">\
<option>to_json-&gt;</option>\
<option>to_json.cpp</option>\
</select>\
');
function to_json_child(item)
{	var child_list = [
		'to_json.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
