// Child table for section forsparsehes
document.write('\
<select onchange="forsparsehes_child(this)">\
<option>forsparsehes-&gt;</option>\
<option>for_sparse_hes.cpp</option>\
</select>\
');
function forsparsehes_child(item)
{	var child_list = [
		'for_sparse_hes.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
