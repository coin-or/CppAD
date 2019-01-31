// Child table for section speed_main
document.write('\
<select onchange="speed_main_child(this)">\
<option>speed_main-&gt;</option>\
<option>link_det_lu</option>\
<option>link_det_minor</option>\
<option>link_mat_mul</option>\
<option>link_ode</option>\
<option>link_poly</option>\
<option>link_sparse_hessian</option>\
<option>link_sparse_jacobian</option>\
<option>microsoft_timer</option>\
</select>\
');
function speed_main_child(item)
{	var child_list = [
		'link_det_lu.htm',
		'link_det_minor.htm',
		'link_mat_mul.htm',
		'link_ode.htm',
		'link_poly.htm',
		'link_sparse_hessian.htm',
		'link_sparse_jacobian.htm',
		'microsoft_timer.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
