// Child table for section drivers
document.write('\
<select onchange="drivers_child(this)">\
<option>drivers-&gt;</option>\
<option>Jacobian</option>\
<option>Hessian</option>\
<option>ForOne</option>\
<option>RevOne</option>\
<option>ForTwo</option>\
<option>RevTwo</option>\
</select>\
');
function drivers_child(item)
{	var child_list = [
		'jacobian.htm',
		'hessian.htm',
		'forone.htm',
		'revone.htm',
		'fortwo.htm',
		'revtwo.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
