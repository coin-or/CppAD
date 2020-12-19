// Child table for section speed_main
document.write('\
<select onchange="speed_main_child(this)">\
<option>speed_main-&gt;</option>\
<option>link_routines</option>\
</select>\
');
function speed_main_child(item)
{	var child_list = [
		'link_routines.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
