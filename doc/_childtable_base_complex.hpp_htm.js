// Child table for section base_complex.hpp
document.write('\
<select onchange="base_complex.hpp_child(this)">\
<option>base_complex.hpp-&gt;</option>\
<option>complex_poly.cpp</option>\
</select>\
');
function base_complex.hpp_child(item)
{	var child_list = [
		'complex_poly.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
