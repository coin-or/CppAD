// Child table for section binary_math
document.write('\
<select onchange="binary_math_child(this)">\
<option>binary_math-&gt;</option>\
<option>atan2</option>\
<option>pow</option>\
<option>azmul</option>\
</select>\
');
function binary_math_child(item)
{	var child_list = [
		'atan2.htm',
		'pow.htm',
		'azmul.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
