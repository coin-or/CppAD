// Child table for section subgraph_sparsity
document.write('\
<select onchange="subgraph_sparsity_child(this)">\
<option>subgraph_sparsity-&gt;</option>\
<option>subgraph_sparsity.cpp</option>\
</select>\
');
function subgraph_sparsity_child(item)
{	var child_list = [
		'subgraph_sparsity.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
