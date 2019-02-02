// Child table for section min_nso_quad
document.write('\
<select onchange="min_nso_quad_child(this)">\
<option>min_nso_quad-&gt;</option>\
<option>min_nso_quad.cpp</option>\
<option>min_nso_quad.hpp</option>\
</select>\
');
function min_nso_quad_child(item)
{	var child_list = [
		'min_nso_quad.cpp.htm',
		'min_nso_quad.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
