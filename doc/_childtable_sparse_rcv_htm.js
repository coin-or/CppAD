// Child table for section sparse_rcv
document.write('\
<select onchange="sparse_rcv_child(this)">\
<option>sparse_rcv-&gt;</option>\
<option>sparse_rcv.cpp</option>\
</select>\
');
function sparse_rcv_child(item)
{	var child_list = [
		'sparse_rcv.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
