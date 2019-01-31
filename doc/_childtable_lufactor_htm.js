// Child table for section lufactor
document.write('\
<select onchange="lufactor_child(this)">\
<option>lufactor-&gt;</option>\
<option>lu_factor.cpp</option>\
<option>lu_factor.hpp</option>\
</select>\
');
function lufactor_child(item)
{	var child_list = [
		'lu_factor.cpp.htm',
		'lu_factor.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
