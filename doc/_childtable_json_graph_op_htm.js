// Child table for section json_graph_op
document.write('\
<select onchange="json_graph_op_child(this)">\
<option>json_graph_op-&gt;</option>\
<option>json_unary_op.cpp</option>\
<option>json_add_op.cpp</option>\
<option>json_azmul_op.cpp</option>\
<option>json_discrete_op.cpp</option>\
<option>json_div_op.cpp</option>\
<option>json_mul_op.cpp</option>\
<option>json_pow_op.cpp</option>\
<option>json_sub_op.cpp</option>\
<option>json_sum_op.cpp</option>\
<option>json_comp_op.cpp</option>\
<option>json_cexp_op.cpp</option>\
<option>json_atom_op.cpp</option>\
</select>\
');
function json_graph_op_child(item)
{	var child_list = [
		'json_unary_op.cpp.htm',
		'json_add_op.cpp.htm',
		'json_azmul_op.cpp.htm',
		'json_discrete_op.cpp.htm',
		'json_div_op.cpp.htm',
		'json_mul_op.cpp.htm',
		'json_pow_op.cpp.htm',
		'json_sub_op.cpp.htm',
		'json_sum_op.cpp.htm',
		'json_comp_op.cpp.htm',
		'json_cexp_op.cpp.htm',
		'json_atom_op.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
