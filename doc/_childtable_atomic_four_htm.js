// Child table for section atomic_four
document.write('\
<select onchange="atomic_four_child(this)">\
<option>atomic_four-&gt;</option>\
<option>atomic_four_define</option>\
<option>atomic_four_example</option>\
</select>\
');
function atomic_four_child(item)
{	var child_list = [
		'atomic_four_define.htm',
		'atomic_four_example.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
