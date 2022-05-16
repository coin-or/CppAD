// Child table for section atomic_four_lin_ode_implement
document.write('\
<select onchange="atomic_four_lin_ode_implement_child(this)">\
<option>atomic_four_lin_ode_implement-&gt;</option>\
<option>atomic_four_lin_ode.hpp</option>\
<option>atomic_four_lin_ode_set.hpp</option>\
<option>atomic_four_lin_ode_get.hpp</option>\
<option>atomic_four_lin_ode_base_lin_ode.hpp</option>\
<option>atomic_four_lin_ode_for_type.hpp</option>\
<option>atomic_four_lin_ode_forward.hpp</option>\
<option>atomic_four_lin_ode_reverse.hpp</option>\
<option>atomic_four_lin_ode_jac_sparsity.hpp</option>\
<option>atomic_four_lin_ode_hes_sparsity.hpp</option>\
</select>\
');
function atomic_four_lin_ode_implement_child(item)
{	var child_list = [
		'atomic_four_lin_ode.hpp.htm',
		'atomic_four_lin_ode_set.hpp.htm',
		'atomic_four_lin_ode_get.hpp.htm',
		'atomic_four_lin_ode_base_lin_ode.hpp.htm',
		'atomic_four_lin_ode_for_type.hpp.htm',
		'atomic_four_lin_ode_forward.hpp.htm',
		'atomic_four_lin_ode_reverse.hpp.htm',
		'atomic_four_lin_ode_jac_sparsity.hpp.htm',
		'atomic_four_lin_ode_hes_sparsity.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
