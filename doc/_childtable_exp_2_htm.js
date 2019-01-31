// Child table for section exp_2
document.write('\
<select onchange="exp_2_child(this)">\
<option>exp_2-&gt;</option>\
<option>exp_2.hpp</option>\
<option>exp_2.cpp</option>\
<option>exp_2_for0</option>\
<option>exp_2_for1</option>\
<option>exp_2_rev1</option>\
<option>exp_2_for2</option>\
<option>exp_2_rev2</option>\
<option>exp_2_cppad</option>\
</select>\
');
function exp_2_child(item)
{	var child_list = [
		'exp_2.hpp.htm',
		'exp_2.cpp.htm',
		'exp_2_for0.htm',
		'exp_2_for1.htm',
		'exp_2_rev1.htm',
		'exp_2_for2.htm',
		'exp_2_rev2.htm',
		'exp_2_cppad.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
