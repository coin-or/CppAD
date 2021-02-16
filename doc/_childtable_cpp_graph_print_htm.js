// Child table for section cpp_graph_print
document.write('\
<select onchange="cpp_graph_print_child(this)">\
<option>cpp_graph_print-&gt;</option>\
<option>print_graph.cpp</option>\
</select>\
');
function cpp_graph_print_child(item)
{	var child_list = [
		'print_graph.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
