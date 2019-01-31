// Child table for section for_hes_sparsity
document.write('\
<select onchange="for_hes_sparsity_child(this)">\
<option>for_hes_sparsity-&gt;</option>\
<option>for_hes_sparsity.cpp</option>\
</select>\
');
function for_hes_sparsity_child(item)
{	var child_list = [
		'for_hes_sparsity.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
