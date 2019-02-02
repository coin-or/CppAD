// Child table for section value
document.write('\
<select onchange="value_child(this)">\
<option>value-&gt;</option>\
<option>value.cpp</option>\
</select>\
');
function value_child(item)
{	var child_list = [
		'value.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
