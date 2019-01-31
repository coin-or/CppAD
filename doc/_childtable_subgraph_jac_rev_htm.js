// Child table for section subgraph_jac_rev
document.write('\
<select onchange="subgraph_jac_rev_child(this)">\
<option>subgraph_jac_rev-&gt;</option>\
<option>subgraph_jac_rev.cpp</option>\
<option>subgraph_hes2jac.cpp</option>\
</select>\
');
function subgraph_jac_rev_child(item)
{	var child_list = [
		'subgraph_jac_rev.cpp.htm',
		'subgraph_hes2jac.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
