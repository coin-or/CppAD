// Child table for section qp_interior
document.write('\
<select onchange="qp_interior_child(this)">\
<option>qp_interior-&gt;</option>\
<option>qp_interior.cpp</option>\
<option>qp_interior.hpp</option>\
</select>\
');
function qp_interior_child(item)
{	var child_list = [
		'qp_interior.cpp.htm',
		'qp_interior.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
