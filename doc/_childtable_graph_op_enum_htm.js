// Child table for section graph_op_enum
document.write('\
<select onchange="graph_op_enum_child(this)">\
<option>graph_op_enum-&gt;</option>\
<option>graph_azmul_op.cpp</option>\
<option>graph_add_op.cpp</option>\
<option>graph_div_op.cpp</option>\
<option>graph_mul_op.cpp</option>\
<option>graph_pow_op.cpp</option>\
<option>graph_sub_op.cpp</option>\
<option>graph_unary_op.cpp</option>\
<option>graph_sum_op.cpp</option>\
<option>graph_comp_op.cpp</option>\
<option>graph_cexp_op.cpp</option>\
<option>graph_atom_op.cpp</option>\
</select>\
');
function graph_op_enum_child(item)
{	var child_list = [
		'graph_azmul_op.cpp.htm',
		'graph_add_op.cpp.htm',
		'graph_div_op.cpp.htm',
		'graph_mul_op.cpp.htm',
		'graph_pow_op.cpp.htm',
		'graph_sub_op.cpp.htm',
		'graph_unary_op.cpp.htm',
		'graph_sum_op.cpp.htm',
		'graph_comp_op.cpp.htm',
		'graph_cexp_op.cpp.htm',
		'graph_atom_op.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
