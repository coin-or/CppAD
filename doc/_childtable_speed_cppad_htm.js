// Child table for section speed_cppad
document.write('\
<select onchange="speed_cppad_child(this)">\
<option>speed_cppad-&gt;</option>\
<option>cppad_det_minor.cpp</option>\
<option>cppad_det_lu.cpp</option>\
<option>cppad_mat_mul.cpp</option>\
<option>cppad_ode.cpp</option>\
<option>cppad_poly.cpp</option>\
<option>cppad_sparse_hessian.cpp</option>\
<option>cppad_sparse_jacobian.cpp</option>\
</select>\
');
function speed_cppad_child(item)
{	var child_list = [
		'cppad_det_minor.cpp.htm',
		'cppad_det_lu.cpp.htm',
		'cppad_mat_mul.cpp.htm',
		'cppad_ode.cpp.htm',
		'cppad_poly.cpp.htm',
		'cppad_sparse_hessian.cpp.htm',
		'cppad_sparse_jacobian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
