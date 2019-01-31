// Child table for section exp_eps
document.write('\
<select onchange="exp_eps_child(this)">\
<option>exp_eps-&gt;</option>\
<option>exp_eps.hpp</option>\
<option>exp_eps.cpp</option>\
<option>exp_eps_for0</option>\
<option>exp_eps_for1</option>\
<option>exp_eps_rev1</option>\
<option>exp_eps_for2</option>\
<option>exp_eps_rev2</option>\
<option>exp_eps_cppad</option>\
</select>\
');
function exp_eps_child(item)
{	var child_list = [
		'exp_eps.hpp.htm',
		'exp_eps.cpp.htm',
		'exp_eps_for0.htm',
		'exp_eps_for1.htm',
		'exp_eps_rev1.htm',
		'exp_eps_for2.htm',
		'exp_eps_rev2.htm',
		'exp_eps_cppad.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
