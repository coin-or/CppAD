// Child table for section log1p
document.write('\
<select onchange="log1p_child(this)">\
<option>log1p-&gt;</option>\
<option>log1p.cpp</option>\
</select>\
');
function log1p_child(item)
{	var child_list = [
		'log1p.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
