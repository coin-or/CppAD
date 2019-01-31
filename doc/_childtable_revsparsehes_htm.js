// Child table for section revsparsehes
document.write('\
<select onchange="revsparsehes_child(this)">\
<option>revsparsehes-&gt;</option>\
<option>rev_sparse_hes.cpp</option>\
<option>sparsity_sub.cpp</option>\
</select>\
');
function revsparsehes_child(item)
{	var child_list = [
		'rev_sparse_hes.cpp.htm',
		'sparsity_sub.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
