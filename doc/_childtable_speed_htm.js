// Child table for section speed
document.write('\
<select onchange="speed_child(this)">\
<option>speed-&gt;</option>\
<option>speed_main</option>\
<option>speed_utility</option>\
<option>speed_double</option>\
<option>speed_adolc</option>\
<option>speed_cppad</option>\
<option>speed_fadbad</option>\
<option>speed_cppadcg</option>\
<option>speed_sacado</option>\
<option>speed_xpackage</option>\
</select>\
');
function speed_child(item)
{	var child_list = [
		'speed_main.htm',
		'speed_utility.htm',
		'speed_double.htm',
		'speed_adolc.htm',
		'speed_cppad.htm',
		'speed_fadbad.htm',
		'speed_cppadcg.htm',
		'speed_sacado.htm',
		'speed_xpackage.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
