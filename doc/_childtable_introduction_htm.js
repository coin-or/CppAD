// Child table for section introduction
document.write('\
<select onchange="introduction_child(this)">\
<option>introduction-&gt;</option>\
<option>exp_2</option>\
<option>exp_eps</option>\
<option>exp_apx.cpp</option>\
</select>\
');
function introduction_child(item)
{	var child_list = [
		'exp_2.htm',
		'exp_eps.htm',
		'exp_apx.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
