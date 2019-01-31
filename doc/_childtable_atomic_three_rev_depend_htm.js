// Child table for section atomic_three_rev_depend
document.write('\
<select onchange="atomic_three_rev_depend_child(this)">\
<option>atomic_three_rev_depend-&gt;</option>\
<option>atomic_three_rev_depend.cpp</option>\
</select>\
');
function atomic_three_rev_depend_child(item)
{	var child_list = [
		'atomic_three_rev_depend.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
