// Child table for section log
document.write('\
<select onchange="log_child(this)">\
<option>log-&gt;</option>\
<option>log.cpp</option>\
</select>\
');
function log_child(item)
{	var child_list = [
		'log.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
