// Child table for section odegearcontrol
document.write('\
<select onchange="odegearcontrol_child(this)">\
<option>odegearcontrol-&gt;</option>\
<option>ode_gear_control.cpp</option>\
</select>\
');
function odegearcontrol_child(item)
{	var child_list = [
		'ode_gear_control.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
