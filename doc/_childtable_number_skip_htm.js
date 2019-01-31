// Child table for section number_skip
document.write('\
<select onchange="number_skip_child(this)">\
<option>number_skip-&gt;</option>\
<option>number_skip.cpp</option>\
</select>\
');
function number_skip_child(item)
{	var child_list = [
		'number_skip.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
