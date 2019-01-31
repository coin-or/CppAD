// Child table for section atomic_three_hes_sparsity
document.write('\
<select onchange="atomic_three_hes_sparsity_child(this)">\
<option>atomic_three_hes_sparsity-&gt;</option>\
<option>atomic_three_hes_sparsity.cpp</option>\
</select>\
');
function atomic_three_hes_sparsity_child(item)
{	var child_list = [
		'atomic_three_hes_sparsity.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
