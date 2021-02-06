// Child table for section reversetheory
document.write('\
<select onchange="reversetheory_child(this)">\
<option>reversetheory-&gt;</option>\
<option>exp_reverse</option>\
<option>log_reverse</option>\
<option>sqrt_reverse</option>\
<option>sin_cos_reverse</option>\
<option>atan_reverse</option>\
<option>asin_reverse</option>\
<option>acos_reverse</option>\
<option>tan_reverse</option>\
<option>erf_reverse</option>\
<option>pow_reverse</option>\
</select>\
');
function reversetheory_child(item)
{	var child_list = [
		'exp_reverse.htm',
		'log_reverse.htm',
		'sqrt_reverse.htm',
		'sin_cos_reverse.htm',
		'atan_reverse.htm',
		'asin_reverse.htm',
		'acos_reverse.htm',
		'tan_reverse.htm',
		'erf_reverse.htm',
		'pow_reverse.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
