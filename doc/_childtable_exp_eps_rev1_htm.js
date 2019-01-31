// Child table for section exp_eps_rev1
document.write('\
<select onchange="exp_eps_rev1_child(this)">\
<option>exp_eps_rev1-&gt;</option>\
<option>exp_eps_rev1.cpp</option>\
</select>\
');
function exp_eps_rev1_child(item)
{	var child_list = [
		'exp_eps_rev1.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
