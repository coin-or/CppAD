// Child table for section nearequal
document.write('\
<select onchange="nearequal_child(this)">\
<option>nearequal-&gt;</option>\
<option>near_equal.cpp</option>\
</select>\
');
function nearequal_child(item)
{	var child_list = [
		'near_equal.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
