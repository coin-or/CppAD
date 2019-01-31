// Child table for section odeerrcontrol
document.write('\
<select onchange="odeerrcontrol_child(this)">\
<option>odeerrcontrol-&gt;</option>\
<option>ode_err_control.cpp</option>\
<option>ode_err_maxabs.cpp</option>\
</select>\
');
function odeerrcontrol_child(item)
{	var child_list = [
		'ode_err_control.cpp.htm',
		'ode_err_maxabs.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
