// Child table for section sparsity_pattern
document.write('\
<select onchange="sparsity_pattern_child(this)">\
<option>sparsity_pattern-&gt;</option>\
<option>for_jac_sparsity</option>\
<option>rev_jac_sparsity</option>\
<option>for_hes_sparsity</option>\
<option>rev_hes_sparsity</option>\
<option>subgraph_sparsity</option>\
<option>dependency.cpp</option>\
<option>rc_sparsity.cpp</option>\
<option>ForSparseJac</option>\
<option>RevSparseJac</option>\
<option>RevSparseHes</option>\
<option>ForSparseHes</option>\
</select>\
');
function sparsity_pattern_child(item)
{	var child_list = [
		'for_jac_sparsity.htm',
		'rev_jac_sparsity.htm',
		'for_hes_sparsity.htm',
		'rev_hes_sparsity.htm',
		'subgraph_sparsity.htm',
		'dependency.cpp.htm',
		'rc_sparsity.cpp.htm',
		'forsparsejac.htm',
		'revsparsejac.htm',
		'revsparsehes.htm',
		'forsparsehes.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
