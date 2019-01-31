// Child table for section numerictype
document.write('\
<select onchange="numerictype_child(this)">\
<option>numerictype-&gt;</option>\
<option>numeric_type.cpp</option>\
</select>\
');
function numerictype_child(item)
{	var child_list = [
		'numeric_type.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
