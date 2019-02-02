// Child table for section jacobian
document.write('\
<select onchange="jacobian_child(this)">\
<option>jacobian-&gt;</option>\
<option>jacobian.cpp</option>\
</select>\
');
function jacobian_child(item)
{	var child_list = [
		'jacobian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
