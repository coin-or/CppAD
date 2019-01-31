// Child table for section det_of_minor
document.write('\
<select onchange="det_of_minor_child(this)">\
<option>det_of_minor-&gt;</option>\
<option>det_of_minor.cpp</option>\
<option>det_of_minor.hpp</option>\
</select>\
');
function det_of_minor_child(item)
{	var child_list = [
		'det_of_minor.cpp.htm',
		'det_of_minor.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
