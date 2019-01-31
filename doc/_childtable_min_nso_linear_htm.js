// Child table for section min_nso_linear
document.write('\
<select onchange="min_nso_linear_child(this)">\
<option>min_nso_linear-&gt;</option>\
<option>min_nso_linear.cpp</option>\
<option>min_nso_linear.hpp</option>\
</select>\
');
function min_nso_linear_child(item)
{	var child_list = [
		'min_nso_linear.cpp.htm',
		'min_nso_linear.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
