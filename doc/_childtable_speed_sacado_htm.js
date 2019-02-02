// Child table for section speed_sacado
document.write('\
<select onchange="speed_sacado_child(this)">\
<option>speed_sacado-&gt;</option>\
<option>sacado_det_minor.cpp</option>\
<option>sacado_det_lu.cpp</option>\
<option>sacado_mat_mul.cpp</option>\
<option>sacado_ode.cpp</option>\
<option>sacado_poly.cpp</option>\
<option>sacado_sparse_hessian.cpp</option>\
<option>sacado_sparse_jacobian.cpp</option>\
</select>\
');
function speed_sacado_child(item)
{	var child_list = [
		'sacado_det_minor.cpp.htm',
		'sacado_det_lu.cpp.htm',
		'sacado_mat_mul.cpp.htm',
		'sacado_ode.cpp.htm',
		'sacado_poly.cpp.htm',
		'sacado_sparse_hessian.cpp.htm',
		'sacado_sparse_jacobian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
