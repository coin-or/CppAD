// Child table for section forone
document.write('\
<select onchange="forone_child(this)">\
<option>forone-&gt;</option>\
<option>for_one.cpp</option>\
</select>\
');
function forone_child(item)
{	var child_list = [
		'for_one.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
