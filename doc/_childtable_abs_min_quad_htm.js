// Child table for section abs_min_quad
document.write('\
<select onchange="abs_min_quad_child(this)">\
<option>abs_min_quad-&gt;</option>\
<option>abs_min_quad.cpp</option>\
<option>abs_min_quad.hpp</option>\
</select>\
');
function abs_min_quad_child(item)
{	var child_list = [
		'abs_min_quad.cpp.htm',
		'abs_min_quad.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
