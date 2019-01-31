// Child table for section rev_jac_sparsity
document.write('\
<select onchange="rev_jac_sparsity_child(this)">\
<option>rev_jac_sparsity-&gt;</option>\
<option>rev_jac_sparsity.cpp</option>\
</select>\
');
function rev_jac_sparsity_child(item)
{	var child_list = [
		'rev_jac_sparsity.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
