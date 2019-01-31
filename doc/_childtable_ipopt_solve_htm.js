// Child table for section ipopt_solve
document.write('\
<select onchange="ipopt_solve_child(this)">\
<option>ipopt_solve-&gt;</option>\
<option>ipopt_solve_get_started.cpp</option>\
<option>ipopt_solve_retape.cpp</option>\
<option>ipopt_solve_ode_inverse.cpp</option>\
</select>\
');
function ipopt_solve_child(item)
{	var child_list = [
		'ipopt_solve_get_started.cpp.htm',
		'ipopt_solve_retape.cpp.htm',
		'ipopt_solve_ode_inverse.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
