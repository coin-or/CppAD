// Child table for section abs_eval
document.write('\
<select onchange="abs_eval_child(this)">\
<option>abs_eval-&gt;</option>\
<option>abs_eval.cpp</option>\
<option>abs_eval.hpp</option>\
</select>\
');
function abs_eval_child(item)
{	var child_list = [
		'abs_eval.cpp.htm',
		'abs_eval.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
