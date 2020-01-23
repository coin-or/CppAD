// Child table for section example_abs_normal
document.write('\
<select onchange="example_abs_normal_child(this)">\
<option>example_abs_normal-&gt;</option>\
<option>abs_get_started.cpp</option>\
<option>abs_print_mat</option>\
<option>abs_eval</option>\
<option>simplex_method</option>\
<option>lp_box</option>\
<option>abs_min_linear</option>\
<option>min_nso_linear</option>\
<option>qp_interior</option>\
<option>qp_box</option>\
<option>abs_min_quad</option>\
<option>min_nso_quad</option>\
</select>\
');
function example_abs_normal_child(item)
{	var child_list = [
		'abs_get_started.cpp.htm',
		'abs_print_mat.htm',
		'abs_eval.htm',
		'simplex_method.htm',
		'lp_box.htm',
		'abs_min_linear.htm',
		'min_nso_linear.htm',
		'qp_interior.htm',
		'qp_box.htm',
		'abs_min_quad.htm',
		'min_nso_quad.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
