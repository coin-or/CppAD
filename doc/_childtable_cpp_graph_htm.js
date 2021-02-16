// Child table for section cpp_graph
document.write('\
<select onchange="cpp_graph_child(this)">\
<option>cpp_graph-&gt;</option>\
<option>cpp_graph_ctor</option>\
<option>cpp_graph_scalar</option>\
<option>cpp_graph_vector</option>\
<option>cpp_graph_print</option>\
</select>\
');
function cpp_graph_child(item)
{	var child_list = [
		'cpp_graph_ctor.htm',
		'cpp_graph_scalar.htm',
		'cpp_graph_vector.htm',
		'cpp_graph_print.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
