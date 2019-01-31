// Child table for section lusolve
document.write('\
<select onchange="lusolve_child(this)">\
<option>lusolve-&gt;</option>\
<option>lu_solve.cpp</option>\
<option>lu_solve.hpp</option>\
</select>\
');
function lusolve_child(item)
{	var child_list = [
		'lu_solve.cpp.htm',
		'lu_solve.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
