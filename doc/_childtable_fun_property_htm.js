// Child table for section fun_property
document.write('\
<select onchange="fun_property_child(this)">\
<option>fun_property-&gt;</option>\
<option>fun_property.cpp</option>\
</select>\
');
function fun_property_child(item)
{	var child_list = [
		'fun_property.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
