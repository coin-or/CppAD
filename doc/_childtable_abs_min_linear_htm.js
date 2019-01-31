// Child table for section abs_min_linear
document.write('\
<select onchange="abs_min_linear_child(this)">\
<option>abs_min_linear-&gt;</option>\
<option>abs_min_linear.cpp</option>\
<option>abs_min_linear.hpp</option>\
</select>\
');
function abs_min_linear_child(item)
{	var child_list = [
		'abs_min_linear.cpp.htm',
		'abs_min_linear.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
