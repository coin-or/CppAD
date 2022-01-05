// Child table for section atomic_vector.hpp
document.write('\
<select onchange="atomic_vector__46__hpp_child(this)">\
<option>atomic_vector.hpp-&gt;</option>\
<option>atomic_vector_forward_op.cpp</option>\
<option>atomic_vector_reverse_op.cpp</option>\
<option>atomic_vector_add_op.cpp</option>\
<option>atomic_vector_sub_op.cpp</option>\
<option>atomic_vector_mul_op.cpp</option>\
<option>atomic_vector_div_op.cpp</option>\
<option>atomic_vector_neg_op.cpp</option>\
</select>\
');
function atomic_vector__46__hpp_child(item)
{	var child_list = [
		'atomic_vector_forward_op.cpp.htm',
		'atomic_vector_reverse_op.cpp.htm',
		'atomic_vector_add_op.cpp.htm',
		'atomic_vector_sub_op.cpp.htm',
		'atomic_vector_mul_op.cpp.htm',
		'atomic_vector_div_op.cpp.htm',
		'atomic_vector_neg_op.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
