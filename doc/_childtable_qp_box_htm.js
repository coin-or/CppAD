// Child table for section qp_box
document.write('\
<select onchange="qp_box_child(this)">\
<option>qp_box-&gt;</option>\
<option>qp_box.cpp</option>\
<option>qp_box.hpp</option>\
</select>\
');
function qp_box_child(item)
{	var child_list = [
		'qp_box.cpp.htm',
		'qp_box.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
