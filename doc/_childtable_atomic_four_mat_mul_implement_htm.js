// Child table for section atomic_four_mat_mul_implement
document.write('\
<select onchange="atomic_four_mat_mul_implement_child(this)">\
<option>atomic_four_mat_mul_implement-&gt;</option>\
<option>atomic_four_mat_mul.hpp</option>\
<option>atomic_four_mat_mul_set.hpp</option>\
<option>atomic_four_mat_mul_get.hpp</option>\
<option>atomic_four_mat_mul_base_mat_mul.hpp</option>\
<option>atomic_four_mat_mul_for_type.hpp</option>\
<option>atomic_four_mat_mul_forward.hpp</option>\
<option>atomic_four_mat_mul_reverse.hpp</option>\
<option>atomic_four_mat_mul_jac_sparsity.hpp</option>\
<option>atomic_four_mat_mul_hes_sparsity.hpp</option>\
<option>atomic_four_mat_mul_rev_depend.hpp</option>\
</select>\
');
function atomic_four_mat_mul_implement_child(item)
{	var child_list = [
		'atomic_four_mat_mul.hpp.htm',
		'atomic_four_mat_mul_set.hpp.htm',
		'atomic_four_mat_mul_get.hpp.htm',
		'atomic_four_mat_mul_base_mat_mul.hpp.htm',
		'atomic_four_mat_mul_for_type.hpp.htm',
		'atomic_four_mat_mul_forward.hpp.htm',
		'atomic_four_mat_mul_reverse.hpp.htm',
		'atomic_four_mat_mul_jac_sparsity.hpp.htm',
		'atomic_four_mat_mul_hes_sparsity.hpp.htm',
		'atomic_four_mat_mul_rev_depend.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
