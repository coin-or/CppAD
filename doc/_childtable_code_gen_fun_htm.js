// Child table for section code_gen_fun
document.write('\
<select onchange="code_gen_fun_child(this)">\
<option>code_gen_fun-&gt;</option>\
<option>code_gen_fun_function.cpp</option>\
<option>code_gen_fun_file.cpp</option>\
<option>code_gen_fun_jacobian.cpp</option>\
<option>code_gen_fun_jac_as_fun.cpp</option>\
<option>code_gen_fun_sparse_jacobian.cpp</option>\
<option>code_gen_fun_sparse_jac_as_fun.cpp</option>\
</select>\
');
function code_gen_fun_child(item)
{	var child_list = [
		'code_gen_fun_function.cpp.htm',
		'code_gen_fun_file.cpp.htm',
		'code_gen_fun_jacobian.cpp.htm',
		'code_gen_fun_jac_as_fun.cpp.htm',
		'code_gen_fun_sparse_jacobian.cpp.htm',
		'code_gen_fun_sparse_jac_as_fun.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
