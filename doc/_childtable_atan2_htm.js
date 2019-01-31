// Child table for section atan2
document.write('\
<select onchange="atan2_child(this)">\
<option>atan2-&gt;</option>\
<option>atan2.cpp</option>\
</select>\
');
function atan2_child(item)
{	var child_list = [
		'atan2.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
