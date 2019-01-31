// Child table for section forward_order
document.write('\
<select onchange="forward_order_child(this)">\
<option>forward_order-&gt;</option>\
<option>forward.cpp</option>\
<option>forward_order.cpp</option>\
</select>\
');
function forward_order_child(item)
{	var child_list = [
		'forward.cpp.htm',
		'forward_order.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
