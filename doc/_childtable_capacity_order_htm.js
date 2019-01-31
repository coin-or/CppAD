// Child table for section capacity_order
document.write('\
<select onchange="capacity_order_child(this)">\
<option>capacity_order-&gt;</option>\
<option>capacity_order.cpp</option>\
</select>\
');
function capacity_order_child(item)
{	var child_list = [
		'capacity_order.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
