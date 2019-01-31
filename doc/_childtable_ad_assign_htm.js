// Child table for section ad_assign
document.write('\
<select onchange="ad_assign_child(this)">\
<option>ad_assign-&gt;</option>\
<option>ad_assign.cpp</option>\
</select>\
');
function ad_assign_child(item)
{	var child_list = [
		'ad_assign.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
