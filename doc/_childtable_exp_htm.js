// Child table for section exp
document.write('\
<select onchange="exp_child(this)">\
<option>exp-&gt;</option>\
<option>exp.cpp</option>\
</select>\
');
function exp_child(item)
{	var child_list = [
		'exp.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
