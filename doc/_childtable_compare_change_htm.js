// Child table for section compare_change
document.write('\
<select onchange="compare_change_child(this)">\
<option>compare_change-&gt;</option>\
<option>compare_change.cpp</option>\
</select>\
');
function compare_change_child(item)
{	var child_list = [
		'compare_change.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
