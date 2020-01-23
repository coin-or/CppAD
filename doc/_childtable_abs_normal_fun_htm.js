// Child table for section abs_normal_fun
document.write('\
<select onchange="abs_normal_fun_child(this)">\
<option>abs_normal_fun-&gt;</option>\
<option>example_abs_normal</option>\
</select>\
');
function abs_normal_fun_child(item)
{	var child_list = [
		'example_abs_normal.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
