// Child table for section sparse_derivative
document.write('\
<select onchange="sparse_derivative_child(this)">\
<option>sparse_derivative-&gt;</option>\
<option>sparse_jac</option>\
<option>sparse_jacobian</option>\
<option>sparse_hes</option>\
<option>sparse_hessian</option>\
<option>subgraph_jac_rev</option>\
</select>\
');
function sparse_derivative_child(item)
{	var child_list = [
		'sparse_jac.htm',
		'sparse_jacobian.htm',
		'sparse_hes.htm',
		'sparse_hessian.htm',
		'subgraph_jac_rev.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
