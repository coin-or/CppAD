// Child table for section numeric_limits
document.write('\
<select onchange="numeric_limits_child(this)">\
<option>numeric_limits-&gt;</option>\
<option>num_limits.cpp</option>\
</select>\
');
function numeric_limits_child(item)
{	var child_list = [
		'num_limits.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
