// Child table for section forwardtheory
document.write('\
<select onchange="forwardtheory_child(this)">\
<option>forwardtheory-&gt;</option>\
<option>exp_forward</option>\
<option>log_forward</option>\
<option>sqrt_forward</option>\
<option>sin_cos_forward</option>\
<option>atan_forward</option>\
<option>asin_forward</option>\
<option>acos_forward</option>\
<option>tan_forward</option>\
<option>erf_forward</option>\
<option>pow_forward</option>\
</select>\
');
function forwardtheory_child(item)
{	var child_list = [
		'exp_forward.htm',
		'log_forward.htm',
		'sqrt_forward.htm',
		'sin_cos_forward.htm',
		'atan_forward.htm',
		'asin_forward.htm',
		'acos_forward.htm',
		'tan_forward.htm',
		'erf_forward.htm',
		'pow_forward.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
