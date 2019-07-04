// Child table for section from_json
document.write('\
<select onchange="from_json_child(this)">\
<option>from_json-&gt;</option>\
<option>from_json.cpp</option>\
</select>\
');
function from_json_child(item)
{	var child_list = [
		'from_json.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
