// Child table for section seq_property
document.write('\
<select onchange="seq_property_child(this)">\
<option>seq_property-&gt;</option>\
<option>seq_property.cpp</option>\
</select>\
');
function seq_property_child(item)
{	var child_list = [
		'seq_property.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
