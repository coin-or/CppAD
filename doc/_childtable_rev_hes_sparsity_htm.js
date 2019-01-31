// Child table for section rev_hes_sparsity
document.write('\
<select onchange="rev_hes_sparsity_child(this)">\
<option>rev_hes_sparsity-&gt;</option>\
<option>rev_hes_sparsity.cpp</option>\
</select>\
');
function rev_hes_sparsity_child(item)
{	var child_list = [
		'rev_hes_sparsity.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
