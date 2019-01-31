// Child table for section subgraph_reverse
document.write('\
<select onchange="subgraph_reverse_child(this)">\
<option>subgraph_reverse-&gt;</option>\
<option>subgraph_reverse.cpp</option>\
</select>\
');
function subgraph_reverse_child(item)
{	var child_list = [
		'subgraph_reverse.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
