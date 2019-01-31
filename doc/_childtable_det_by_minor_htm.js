// Child table for section det_by_minor
document.write('\
<select onchange="det_by_minor_child(this)">\
<option>det_by_minor-&gt;</option>\
<option>det_by_minor.cpp</option>\
<option>det_by_minor.hpp</option>\
</select>\
');
function det_by_minor_child(item)
{	var child_list = [
		'det_by_minor.cpp.htm',
		'det_by_minor.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
