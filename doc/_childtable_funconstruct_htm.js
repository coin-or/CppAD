// Child table for section funconstruct
document.write('\
<select onchange="funconstruct_child(this)">\
<option>funconstruct-&gt;</option>\
<option>fun_assign.cpp</option>\
</select>\
');
function funconstruct_child(item)
{	var child_list = [
		'fun_assign.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
