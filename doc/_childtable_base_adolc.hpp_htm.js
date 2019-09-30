// Child table for section base_adolc.hpp
document.write('\
<select onchange="base_adolc__46__hpp_child(this)">\
<option>base_adolc.hpp-&gt;</option>\
<option>mul_level_adolc.cpp</option>\
</select>\
');
function base_adolc__46__hpp_child(item)
{	var child_list = [
		'mul_level_adolc.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
