// Child table for section con_dyn_var
document.write('\
<select onchange="con_dyn_var_child(this)">\
<option>con_dyn_var-&gt;</option>\
<option>con_dyn_var.cpp</option>\
</select>\
');
function con_dyn_var_child(item)
{	var child_list = [
		'con_dyn_var.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
