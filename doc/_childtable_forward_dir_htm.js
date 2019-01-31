// Child table for section forward_dir
document.write('\
<select onchange="forward_dir_child(this)">\
<option>forward_dir-&gt;</option>\
<option>forward_dir.cpp</option>\
</select>\
');
function forward_dir_child(item)
{	var child_list = [
		'forward_dir.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
