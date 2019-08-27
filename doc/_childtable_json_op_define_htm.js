// Child table for section json_op_define
document.write('\
<select onchange="json_op_define_child(this)">\
<option>json_op_define-&gt;</option>\
<option>json_abs_op.cpp</option>\
<option>json_acos_op.cpp</option>\
<option>json_asin_op.cpp</option>\
<option>json_atan_op.cpp</option>\
<option>json_cosh_op.cpp</option>\
<option>json_cos_op.cpp</option>\
<option>json_exp_op.cpp</option>\
<option>json_log_op.cpp</option>\
<option>json_sign_op.cpp</option>\
<option>json_sinh_op.cpp</option>\
<option>json_sin_op.cpp</option>\
<option>json_sqrt_op.cpp</option>\
<option>json_tanh_op.cpp</option>\
<option>json_tan_op.cpp</option>\
<option>json_acosh_op.cpp</option>\
<option>json_asinh_op.cpp</option>\
<option>json_atanh_op.cpp</option>\
<option>json_erf_op.cpp</option>\
<option>json_erfc_op.cpp</option>\
<option>json_expm1_op.cpp</option>\
<option>json_log1p_op.cpp</option>\
<option>json_add_op.cpp</option>\
<option>json_atom_op.cpp</option>\
<option>json_div_op.cpp</option>\
<option>json_mul_op.cpp</option>\
<option>json_sub_op.cpp</option>\
<option>json_sum_op.cpp</option>\
</select>\
');
function json_op_define_child(item)
{	var child_list = [
		'json_abs_op.cpp.htm',
		'json_acos_op.cpp.htm',
		'json_asin_op.cpp.htm',
		'json_atan_op.cpp.htm',
		'json_cosh_op.cpp.htm',
		'json_cos_op.cpp.htm',
		'json_exp_op.cpp.htm',
		'json_log_op.cpp.htm',
		'json_sign_op.cpp.htm',
		'json_sinh_op.cpp.htm',
		'json_sin_op.cpp.htm',
		'json_sqrt_op.cpp.htm',
		'json_tanh_op.cpp.htm',
		'json_tan_op.cpp.htm',
		'json_acosh_op.cpp.htm',
		'json_asinh_op.cpp.htm',
		'json_atanh_op.cpp.htm',
		'json_erf_op.cpp.htm',
		'json_erfc_op.cpp.htm',
		'json_expm1_op.cpp.htm',
		'json_log1p_op.cpp.htm',
		'json_add_op.cpp.htm',
		'json_atom_op.cpp.htm',
		'json_div_op.cpp.htm',
		'json_mul_op.cpp.htm',
		'json_sub_op.cpp.htm',
		'json_sum_op.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
