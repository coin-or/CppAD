// Child table for section speed_double
document.write('\
<select onchange="speed_double_child(this)">\
<option>speed_double-&gt;</option>\
<option>double_det_minor.cpp</option>\
<option>double_det_lu.cpp</option>\
<option>double_mat_mul.cpp</option>\
<option>double_ode.cpp</option>\
<option>double_poly.cpp</option>\
<option>double_sparse_hessian.cpp</option>\
<option>double_sparse_jacobian.cpp</option>\
</select>\
');
function speed_double_child(item)
{	var child_list = [
		'double_det_minor.cpp.htm',
		'double_det_lu.cpp.htm',
		'double_mat_mul.cpp.htm',
		'double_ode.cpp.htm',
		'double_poly.cpp.htm',
		'double_sparse_hessian.cpp.htm',
		'double_sparse_jacobian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
