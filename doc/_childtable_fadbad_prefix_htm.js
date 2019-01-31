// Child table for section fadbad_prefix
document.write('\
<select onchange="fadbad_prefix_child(this)">\
<option>fadbad_prefix-&gt;</option>\
<option>get_fadbad.sh</option>\
</select>\
');
function fadbad_prefix_child(item)
{	var child_list = [
		'get_fadbad.sh.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
