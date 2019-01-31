// Child table for section taylor_ode
document.write('\
<select onchange="taylor_ode_child(this)">\
<option>taylor_ode-&gt;</option>\
<option>taylor_ode.cpp</option>\
</select>\
');
function taylor_ode_child(item)
{	var child_list = [
		'taylor_ode.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
