// Child table for section fortwo
document.write('\
<select onchange="fortwo_child(this)">\
<option>fortwo-&gt;</option>\
<option>for_two.cpp</option>\
</select>\
');
function fortwo_child(item)
{	var child_list = [
		'for_two.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
