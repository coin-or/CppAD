// Child table for section atomic_three_forward
document.write('\
<select onchange="atomic_three_forward_child(this)">\
<option>atomic_three_forward-&gt;</option>\
<option>atomic_three_forward.cpp</option>\
<option>atomic_three_dynamic.cpp</option>\
</select>\
');
function atomic_three_forward_child(item)
{	var child_list = [
		'atomic_three_forward.cpp.htm',
		'atomic_three_dynamic.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
