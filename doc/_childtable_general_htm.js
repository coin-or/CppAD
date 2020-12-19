// Child table for section general
document.write('\
<select onchange="general_child(this)">\
<option>general-&gt;</option>\
<option>ad_fun.cpp</option>\
<option>ad_in_c.cpp</option>\
<option>conj_grad.cpp</option>\
<option>cppad_eigen.hpp</option>\
<option>hes_minor_det.cpp</option>\
<option>hes_lu_det.cpp</option>\
<option>interface2c.cpp</option>\
<option>jac_minor_det.cpp</option>\
<option>jac_lu_det.cpp</option>\
<option>mul_level</option>\
<option>ode_stiff.cpp</option>\
<option>mul_level_ode.cpp</option>\
<option>mul_level_adolc_ode.cpp</option>\
<option>stack_machine.cpp</option>\
<option>compiled_fun</option>\
</select>\
');
function general_child(item)
{	var child_list = [
		'ad_fun.cpp.htm',
		'ad_in_c.cpp.htm',
		'conj_grad.cpp.htm',
		'cppad_eigen.hpp.htm',
		'hes_minor_det.cpp.htm',
		'hes_lu_det.cpp.htm',
		'interface2c.cpp.htm',
		'jac_minor_det.cpp.htm',
		'jac_lu_det.cpp.htm',
		'mul_level.htm',
		'ode_stiff.cpp.htm',
		'mul_level_ode.cpp.htm',
		'mul_level_adolc_ode.cpp.htm',
		'stack_machine.cpp.htm',
		'compiled_fun.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
