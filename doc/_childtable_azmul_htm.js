// Child table for section azmul
document.write('\
<select onchange="azmul_child(this)">\
<option>azmul-&gt;</option>\
<option>azmul.cpp</option>\
</select>\
');
function azmul_child(item)
{	var child_list = [
		'azmul.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
