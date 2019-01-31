// Child table for section odegear
document.write('\
<select onchange="odegear_child(this)">\
<option>odegear-&gt;</option>\
<option>ode_gear.cpp</option>\
</select>\
');
function odegear_child(item)
{	var child_list = [
		'ode_gear.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
