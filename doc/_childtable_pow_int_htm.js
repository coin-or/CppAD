// Child table for section pow_int
document.write('\
<select onchange="pow_int_child(this)">\
<option>pow_int-&gt;</option>\
<option>pow_int.cpp</option>\
</select>\
');
function pow_int_child(item)
{	var child_list = [
		'pow_int.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
