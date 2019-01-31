// Child table for section sparse_hessian
document.write('\
<select onchange="sparse_hessian_child(this)">\
<option>sparse_hessian-&gt;</option>\
<option>sparse_hessian.cpp</option>\
<option>sub_sparse_hes.cpp</option>\
<option>sparse_sub_hes.cpp</option>\
</select>\
');
function sparse_hessian_child(item)
{	var child_list = [
		'sparse_hessian.cpp.htm',
		'sub_sparse_hes.cpp.htm',
		'sparse_sub_hes.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
