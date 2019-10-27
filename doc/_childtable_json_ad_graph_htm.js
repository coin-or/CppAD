// Child table for section json_ad_graph
document.write('\
<select onchange="json_ad_graph_child(this)">\
<option>json_ad_graph-&gt;</option>\
<option>json_graph_op</option>\
<option>from_json</option>\
<option>to_json</option>\
<option>json_sparse.cpp</option>\
</select>\
');
function json_ad_graph_child(item)
{	var child_list = [
		'json_graph_op.htm',
		'from_json.htm',
		'to_json.htm',
		'json_sparse.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
