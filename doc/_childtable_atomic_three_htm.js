// Child table for section atomic_three
document.write('\
<select onchange="atomic_three_child(this)">\
<option>atomic_three-&gt;</option>\
<option>atomic_three_define</option>\
<option>atomic_three_example</option>\
</select>\
');
function atomic_three_child(item)
{	var child_list = [
		'atomic_three_define.htm',
		'atomic_three_example.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
