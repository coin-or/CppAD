// Child table for section speed_xpackage
document.write('\
<select onchange="speed_xpackage_child(this)">\
<option>speed_xpackage-&gt;</option>\
<option>xpackage_det_minor.cpp</option>\
<option>xpackage_det_lu.cpp</option>\
<option>xpackage_mat_mul.cpp</option>\
<option>xpackage_ode.cpp</option>\
<option>xpackage_poly.cpp</option>\
<option>xpackage_sparse_hessian.cpp</option>\
<option>xpackage_sparse_jacobian.cpp</option>\
</select>\
');
function speed_xpackage_child(item)
{	var child_list = [
		'xpackage_det_minor.cpp.htm',
		'xpackage_det_lu.cpp.htm',
		'xpackage_mat_mul.cpp.htm',
		'xpackage_ode.cpp.htm',
		'xpackage_poly.cpp.htm',
		'xpackage_sparse_hessian.cpp.htm',
		'xpackage_sparse_jacobian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
