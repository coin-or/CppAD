// Child table for section to_csrc
document.write('\
<select onchange="to_csrc_child(this)">\
<option>to_csrc-&gt;</option>\
<option>example_jit</option>\
</select>\
');
function to_csrc_child(item)
{	var child_list = [
		'example_jit.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
