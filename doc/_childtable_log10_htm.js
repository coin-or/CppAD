// Child table for section log10
document.write('\
<select onchange="log10_child(this)">\
<option>log10-&gt;</option>\
<option>log10.cpp</option>\
</select>\
');
function log10_child(item)
{	var child_list = [
		'log10.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
