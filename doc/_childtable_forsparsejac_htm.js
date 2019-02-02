// Child table for section forsparsejac
document.write('\
<select onchange="forsparsejac_child(this)">\
<option>forsparsejac-&gt;</option>\
<option>for_sparse_jac.cpp</option>\
</select>\
');
function forsparsejac_child(item)
{	var child_list = [
		'for_sparse_jac.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
