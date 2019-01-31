// Child table for section rombergmul
document.write('\
<select onchange="rombergmul_child(this)">\
<option>rombergmul-&gt;</option>\
<option>Rombergmul.cpp</option>\
</select>\
');
function rombergmul_child(item)
{	var child_list = [
		'rombergmul.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
