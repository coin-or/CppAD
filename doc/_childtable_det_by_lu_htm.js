// Child table for section det_by_lu
document.write('\
<select onchange="det_by_lu_child(this)">\
<option>det_by_lu-&gt;</option>\
<option>det_by_lu.cpp</option>\
<option>det_by_lu.hpp</option>\
</select>\
');
function det_by_lu_child(item)
{	var child_list = [
		'det_by_lu.cpp.htm',
		'det_by_lu.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
