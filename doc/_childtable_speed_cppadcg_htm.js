// Child table for section speed_cppadcg
document.write('\
<select onchange="speed_cppadcg_child(this)">\
<option>speed_cppadcg-&gt;</option>\
<option>cppadcg_det_minor.cpp</option>\
<option>cppadcg_det_lu.cpp</option>\
<option>cppadcg_mat_mul.cpp</option>\
<option>cppadcg_ode.cpp</option>\
<option>cppadcg_poly.cpp</option>\
<option>cppadcg_sparse_hessian.cpp</option>\
<option>cppadcg_sparse_jacobian.cpp</option>\
</select>\
');
function speed_cppadcg_child(item)
{	var child_list = [
		'cppadcg_det_minor.cpp.htm',
		'cppadcg_det_lu.cpp.htm',
		'cppadcg_mat_mul.cpp.htm',
		'cppadcg_ode.cpp.htm',
		'cppadcg_poly.cpp.htm',
		'cppadcg_sparse_hessian.cpp.htm',
		'cppadcg_sparse_jacobian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
