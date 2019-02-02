// Child table for section uniform_01
document.write('\
<select onchange="uniform_01_child(this)">\
<option>uniform_01-&gt;</option>\
<option>uniform_01.hpp</option>\
</select>\
');
function uniform_01_child(item)
{	var child_list = [
		'uniform_01.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
