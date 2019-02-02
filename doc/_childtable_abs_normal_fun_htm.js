// Child table for section abs_normal_fun
document.write('\
<select onchange="abs_normal_fun_child(this)">\
<option>abs_normal_fun-&gt;</option>\
<option>abs_get_started.cpp</option>\
</select>\
');
function abs_normal_fun_child(item)
{	var child_list = [
		'abs_get_started.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
