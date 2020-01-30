// Child table for section cppadcg_sparse_jacobian.cpp
document.write('\
<select onchange="cppadcg_sparse_jacobian__46__cpp_child(this)">\
<option>cppadcg_sparse_jacobian.cpp-&gt;</option>\
<option>cppadcg_sparse_jacobian.c</option>\
<option>cppadcg_sparse_jacobian_cg.cpp</option>\
</select>\
');
function cppadcg_sparse_jacobian__46__cpp_child(item)
{	var child_list = [
		'cppadcg_sparse_jacobian.c.htm',
		'cppadcg_sparse_jacobian_cg.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
