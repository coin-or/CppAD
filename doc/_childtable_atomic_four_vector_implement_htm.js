// Child table for section atomic_four_vector_implement
document.write('\
<select onchange="atomic_four_vector_implement_child(this)">\
<option>atomic_four_vector_implement-&gt;</option>\
<option>atomic_four_vector.hpp</option>\
<option>atomic_four_vector_forward_op.hpp</option>\
<option>atomic_four_vector_reverse_op.hpp</option>\
<option>atomic_four_vector_jac_sparsity.hpp</option>\
<option>atomic_four_vector_hes_sparsity.hpp</option>\
<option>atomic_four_vector_for_type.hpp</option>\
<option>atomic_four_vector_rev_depend.hpp</option>\
<option>atomic_four_vector_add_op.hpp</option>\
<option>atomic_four_vector_sub_op.hpp</option>\
<option>atomic_four_vector_mul_op.hpp</option>\
<option>atomic_four_vector_div_op.hpp</option>\
<option>atomic_four_vector_neg_op.hpp</option>\
</select>\
');
function atomic_four_vector_implement_child(item)
{	var child_list = [
		'atomic_four_vector.hpp.htm',
		'atomic_four_vector_forward_op.hpp.htm',
		'atomic_four_vector_reverse_op.hpp.htm',
		'atomic_four_vector_jac_sparsity.hpp.htm',
		'atomic_four_vector_hes_sparsity.hpp.htm',
		'atomic_four_vector_for_type.hpp.htm',
		'atomic_four_vector_rev_depend.hpp.htm',
		'atomic_four_vector_add_op.hpp.htm',
		'atomic_four_vector_sub_op.hpp.htm',
		'atomic_four_vector_mul_op.hpp.htm',
		'atomic_four_vector_div_op.hpp.htm',
		'atomic_four_vector_neg_op.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
