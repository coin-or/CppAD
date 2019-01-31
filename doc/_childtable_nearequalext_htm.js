// Child table for section nearequalext
document.write('\
<select onchange="nearequalext_child(this)">\
<option>nearequalext-&gt;</option>\
<option>near_equal_ext.cpp</option>\
</select>\
');
function nearequalext_child(item)
{	var child_list = [
		'near_equal_ext.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
