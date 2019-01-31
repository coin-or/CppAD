// Child table for section mat_sum_sq
document.write('\
<select onchange="mat_sum_sq_child(this)">\
<option>mat_sum_sq-&gt;</option>\
<option>mat_sum_sq.cpp</option>\
<option>mat_sum_sq.hpp</option>\
</select>\
');
function mat_sum_sq_child(item)
{	var child_list = [
		'mat_sum_sq.cpp.htm',
		'mat_sum_sq.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
