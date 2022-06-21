// Child table for section speed_cppad_jit
document.write('\
<select onchange="speed_cppad_jit_child(this)">\
<option>speed_cppad_jit-&gt;</option>\
<option>cppad_jit_det_minor.cpp</option>\
<option>cppad_jit_det_lu.cpp</option>\
<option>cppad_jit_mat_mul.cpp</option>\
<option>cppad_jit_ode.cpp</option>\
<option>cppad_jit_poly.cpp</option>\
<option>cppad_jit_sparse_hessian.cpp</option>\
<option>cppad_jit_sparse_jacobian.cpp</option>\
</select>\
');
function speed_cppad_jit_child(item)
{	var child_list = [
		'cppad_jit_det_minor.cpp.htm',
		'cppad_jit_det_lu.cpp.htm',
		'cppad_jit_mat_mul.cpp.htm',
		'cppad_jit_ode.cpp.htm',
		'cppad_jit_poly.cpp.htm',
		'cppad_jit_sparse_hessian.cpp.htm',
		'cppad_jit_sparse_jacobian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
