// Child table for section ode_evaluate
document.write('\
<select onchange="ode_evaluate_child(this)">\
<option>ode_evaluate-&gt;</option>\
<option>ode_evaluate.cpp</option>\
<option>ode_evaluate.hpp</option>\
</select>\
');
function ode_evaluate_child(item)
{	var child_list = [
		'ode_evaluate.cpp.htm',
		'ode_evaluate.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
