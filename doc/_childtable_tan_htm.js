// Child table for section tan
document.write('\
<select onchange="tan_child(this)">\
<option>tan-&gt;</option>\
<option>tan.cpp</option>\
</select>\
');
function tan_child(item)
{	var child_list = [
		'tan.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
