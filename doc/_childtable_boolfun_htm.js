// Child table for section boolfun
document.write('\
<select onchange="boolfun_child(this)">\
<option>boolfun-&gt;</option>\
<option>bool_fun.cpp</option>\
</select>\
');
function boolfun_child(item)
{	var child_list = [
		'bool_fun.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
