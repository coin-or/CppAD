// Child table for section adolc_prefix
document.write('\
<select onchange="adolc_prefix_child(this)">\
<option>adolc_prefix-&gt;</option>\
<option>get_adolc.sh</option>\
</select>\
');
function adolc_prefix_child(item)
{	var child_list = [
		'get_adolc.sh.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
