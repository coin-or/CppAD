// Child table for section atomic_four_lin_ode
document.write('\
<select onchange="atomic_four_lin_ode_child(this)">\
<option>atomic_four_lin_ode-&gt;</option>\
<option>atomic_four_lin_ode_implement</option>\
<option>atomic_four_lin_ode_forward.cpp</option>\
<option>atomic_four_lin_ode_reverse.cpp</option>\
<option>atomic_four_lin_ode_sparsity.cpp</option>\
<option>atomic_four_lin_ode_rev_depend.cpp</option>\
</select>\
');
function atomic_four_lin_ode_child(item)
{	var child_list = [
		'atomic_four_lin_ode_implement.htm',
		'atomic_four_lin_ode_forward.cpp.htm',
		'atomic_four_lin_ode_reverse.cpp.htm',
		'atomic_four_lin_ode_sparsity.cpp.htm',
		'atomic_four_lin_ode_rev_depend.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
