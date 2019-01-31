// Child table for section simplex_method
document.write('\
<select onchange="simplex_method_child(this)">\
<option>simplex_method-&gt;</option>\
<option>simplex_method.cpp</option>\
<option>simplex_method.hpp</option>\
</select>\
');
function simplex_method_child(item)
{	var child_list = [
		'simplex_method.cpp.htm',
		'simplex_method.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
