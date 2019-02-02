// Child table for section speed_adolc
document.write('\
<select onchange="speed_adolc_child(this)">\
<option>speed_adolc-&gt;</option>\
<option>adolc_det_minor.cpp</option>\
<option>adolc_det_lu.cpp</option>\
<option>adolc_mat_mul.cpp</option>\
<option>adolc_ode.cpp</option>\
<option>adolc_poly.cpp</option>\
<option>adolc_sparse_hessian.cpp</option>\
<option>adolc_sparse_jacobian.cpp</option>\
<option>adolc_alloc_mat</option>\
</select>\
');
function speed_adolc_child(item)
{	var child_list = [
		'adolc_det_minor.cpp.htm',
		'adolc_det_lu.cpp.htm',
		'adolc_mat_mul.cpp.htm',
		'adolc_ode.cpp.htm',
		'adolc_poly.cpp.htm',
		'adolc_sparse_hessian.cpp.htm',
		'adolc_sparse_jacobian.cpp.htm',
		'adolc_alloc_mat.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
