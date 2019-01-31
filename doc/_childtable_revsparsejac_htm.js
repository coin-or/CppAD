// Child table for section revsparsejac
document.write('\
<select onchange="revsparsejac_child(this)">\
<option>revsparsejac-&gt;</option>\
<option>rev_sparse_jac.cpp</option>\
</select>\
');
function revsparsejac_child(item)
{	var child_list = [
		'rev_sparse_jac.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
