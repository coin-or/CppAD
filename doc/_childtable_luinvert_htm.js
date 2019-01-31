// Child table for section luinvert
document.write('\
<select onchange="luinvert_child(this)">\
<option>luinvert-&gt;</option>\
<option>lu_invert.cpp</option>\
<option>lu_invert.hpp</option>\
</select>\
');
function luinvert_child(item)
{	var child_list = [
		'lu_invert.cpp.htm',
		'lu_invert.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
