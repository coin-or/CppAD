// Child table for section acos
document.write('\
<select onchange="acos_child(this)">\
<option>acos-&gt;</option>\
<option>acos.cpp</option>\
</select>\
');
function acos_child(item)
{	var child_list = [
		'acos.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
