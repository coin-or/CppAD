// Child table for section from_graph
document.write('\
<select onchange="from_graph_child(this)">\
<option>from_graph-&gt;</option>\
<option>switch_var_dyn.cpp</option>\
</select>\
');
function from_graph_child(item)
{	var child_list = [
		'switch_var_dyn.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
