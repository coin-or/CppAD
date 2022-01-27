// Child table for section atomic_four_forward
document.write('\
<select onchange="atomic_four_forward_child(this)">\
<option>atomic_four_forward-&gt;</option>\
<option>atomic_four_forward.cpp</option>\
<option>atomic_four_dynamic.cpp</option>\
</select>\
');
function atomic_four_forward_child(item)
{	var child_list = [
		'atomic_four_forward.cpp.htm',
		'atomic_four_dynamic.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
