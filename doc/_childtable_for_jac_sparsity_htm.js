// Child table for section for_jac_sparsity
document.write('\
<select onchange="for_jac_sparsity_child(this)">\
<option>for_jac_sparsity-&gt;</option>\
<option>for_jac_sparsity.cpp</option>\
</select>\
');
function for_jac_sparsity_child(item)
{	var child_list = [
		'for_jac_sparsity.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
