// Child table for section sacado_prefix
document.write('\
<select onchange="sacado_prefix_child(this)">\
<option>sacado_prefix-&gt;</option>\
<option>get_sacado.sh</option>\
</select>\
');
function sacado_prefix_child(item)
{	var child_list = [
		'get_sacado.sh.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
