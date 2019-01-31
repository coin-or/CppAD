// Child table for section forward
document.write('\
<select onchange="forward_child(this)">\
<option>forward-&gt;</option>\
<option>new_dynamic</option>\
<option>forward_zero</option>\
<option>forward_one</option>\
<option>forward_two</option>\
<option>forward_order</option>\
<option>forward_dir</option>\
<option>size_order</option>\
<option>compare_change</option>\
<option>capacity_order</option>\
<option>number_skip</option>\
</select>\
');
function forward_child(item)
{	var child_list = [
		'new_dynamic.htm',
		'forward_zero.htm',
		'forward_one.htm',
		'forward_two.htm',
		'forward_order.htm',
		'forward_dir.htm',
		'size_order.htm',
		'compare_change.htm',
		'capacity_order.htm',
		'number_skip.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
