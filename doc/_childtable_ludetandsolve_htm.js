// Child table for section ludetandsolve
document.write('\
<select onchange="ludetandsolve_child(this)">\
<option>ludetandsolve-&gt;</option>\
<option>LuSolve</option>\
<option>LuFactor</option>\
<option>LuInvert</option>\
</select>\
');
function ludetandsolve_child(item)
{	var child_list = [
		'lusolve.htm',
		'lufactor.htm',
		'luinvert.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
