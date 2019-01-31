// Child table for section acosh
document.write('\
<select onchange="acosh_child(this)">\
<option>acosh-&gt;</option>\
<option>acosh.cpp</option>\
</select>\
');
function acosh_child(item)
{	var child_list = [
		'acosh.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
