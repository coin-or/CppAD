// Child table for section poly
document.write('\
<select onchange="poly_child(this)">\
<option>poly-&gt;</option>\
<option>poly.cpp</option>\
<option>poly.hpp</option>\
</select>\
');
function poly_child(item)
{	var child_list = [
		'poly.cpp.htm',
		'poly.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
