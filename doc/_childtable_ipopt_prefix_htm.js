// Child table for section ipopt_prefix
document.write('\
<select onchange="ipopt_prefix_child(this)">\
<option>ipopt_prefix-&gt;</option>\
<option>get_ipopt.sh</option>\
</select>\
');
function ipopt_prefix_child(item)
{	var child_list = [
		'get_ipopt.sh.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
