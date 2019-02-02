// Child table for section speed_utility
document.write('\
<select onchange="speed_utility_child(this)">\
<option>speed_utility-&gt;</option>\
<option>det_by_lu</option>\
<option>det_of_minor</option>\
<option>det_by_minor</option>\
<option>det_33</option>\
<option>det_grad_33</option>\
<option>mat_sum_sq</option>\
<option>ode_evaluate</option>\
<option>sparse_jac_fun</option>\
<option>sparse_hes_fun</option>\
<option>uniform_01</option>\
</select>\
');
function speed_utility_child(item)
{	var child_list = [
		'det_by_lu.htm',
		'det_of_minor.htm',
		'det_by_minor.htm',
		'det_33.htm',
		'det_grad_33.htm',
		'mat_sum_sq.htm',
		'ode_evaluate.htm',
		'sparse_jac_fun.htm',
		'sparse_hes_fun.htm',
		'uniform_01.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
