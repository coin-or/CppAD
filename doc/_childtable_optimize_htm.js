// Child table for section optimize
document.write('\
<select onchange="optimize_child(this)">\
<option>optimize-&gt;</option>\
<option>optimize_twice.cpp</option>\
<option>optimize_forward_active.cpp</option>\
<option>optimize_reverse_active.cpp</option>\
<option>optimize_compare_op.cpp</option>\
<option>optimize_print_for.cpp</option>\
<option>optimize_conditional_skip.cpp</option>\
<option>optimize_nest_conditional.cpp</option>\
<option>optimize_cumulative_sum.cpp</option>\
</select>\
');
function optimize_child(item)
{	var child_list = [
		'optimize_twice.cpp.htm',
		'optimize_forward_active.cpp.htm',
		'optimize_reverse_active.cpp.htm',
		'optimize_compare_op.cpp.htm',
		'optimize_print_for.cpp.htm',
		'optimize_conditional_skip.cpp.htm',
		'optimize_nest_conditional.cpp.htm',
		'optimize_cumulative_sum.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
