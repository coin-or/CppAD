// Child table for section speed_fadbad
document.write('\
<select onchange="speed_fadbad_child(this)">\
<option>speed_fadbad-&gt;</option>\
<option>fadbad_det_minor.cpp</option>\
<option>fadbad_det_lu.cpp</option>\
<option>fadbad_mat_mul.cpp</option>\
<option>fadbad_ode.cpp</option>\
<option>fadbad_poly.cpp</option>\
<option>fadbad_sparse_hessian.cpp</option>\
<option>fadbad_sparse_jacobian.cpp</option>\
</select>\
');
function speed_fadbad_child(item)
{	var child_list = [
		'fadbad_det_minor.cpp.htm',
		'fadbad_det_lu.cpp.htm',
		'fadbad_mat_mul.cpp.htm',
		'fadbad_ode.cpp.htm',
		'fadbad_poly.cpp.htm',
		'fadbad_sparse_hessian.cpp.htm',
		'fadbad_sparse_jacobian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
