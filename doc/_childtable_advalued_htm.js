// Child table for section advalued
document.write('\
<select onchange="advalued_child(this)">\
<option>advalued-&gt;</option>\
<option>Arithmetic</option>\
<option>unary_standard_math</option>\
<option>binary_math</option>\
<option>CondExp</option>\
<option>Discrete</option>\
<option>numeric_limits</option>\
<option>atomic</option>\
</select>\
');
function advalued_child(item)
{	var child_list = [
		'arithmetic.htm',
		'unary_standard_math.htm',
		'binary_math.htm',
		'condexp.htm',
		'discrete.htm',
		'numeric_limits.htm',
		'atomic.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
