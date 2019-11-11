// Child table for section cpp_ad_graph
document.write('\
<select onchange="cpp_ad_graph_child(this)">\
<option>cpp_ad_graph-&gt;</option>\
<option>graph_op_enum</option>\
<option>cpp_graph</option>\
<option>from_graph</option>\
<option>to_graph</option>\
</select>\
');
function cpp_ad_graph_child(item)
{	var child_list = [
		'graph_op_enum.htm',
		'cpp_graph.htm',
		'from_graph.htm',
		'to_graph.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
