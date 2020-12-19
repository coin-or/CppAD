// Child table for section compiled_fun
document.write('\
<select onchange="compiled_fun_child(this)">\
<option>compiled_fun-&gt;</option>\
<option>compiled_fun_function.cpp</option>\
<option>compiled_fun_file.cpp</option>\
<option>compiled_fun_jacobian.cpp</option>\
<option>compiled_fun_jac_as_fun.cpp</option>\
<option>compiled_fun_sparse_jacobian.cpp</option>\
<option>compiled_fun_sparse_jac_as_fun.cpp</option>\
</select>\
');
function compiled_fun_child(item)
{	var child_list = [
		'compiled_fun_function.cpp.htm',
		'compiled_fun_file.cpp.htm',
		'compiled_fun_jacobian.cpp.htm',
		'compiled_fun_jac_as_fun.cpp.htm',
		'compiled_fun_sparse_jacobian.cpp.htm',
		'compiled_fun_sparse_jac_as_fun.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
