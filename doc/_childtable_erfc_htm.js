// Child table for section erfc
document.write('\
<select onchange="erfc_child(this)">\
<option>erfc-&gt;</option>\
<option>erfc.cpp</option>\
</select>\
');
function erfc_child(item)
{	var child_list = [
		'erfc.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
