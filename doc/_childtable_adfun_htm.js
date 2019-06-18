// Child table for section adfun
document.write('\
<select onchange="adfun_child(this)">\
<option>adfun-&gt;</option>\
<option>record_adfun</option>\
<option>other_adfun</option>\
<option>drivers</option>\
<option>Forward</option>\
<option>Reverse</option>\
<option>sparsity_pattern</option>\
<option>sparse_derivative</option>\
<option>optimize</option>\
<option>abs_normal</option>\
<option>FunCheck</option>\
<option>check_for_nan</option>\
</select>\
');
function adfun_child(item)
{	var child_list = [
		'record_adfun.htm',
		'other_adfun.htm',
		'drivers.htm',
		'forward.htm',
		'reverse.htm',
		'sparsity_pattern.htm',
		'sparse_derivative.htm',
		'optimize.htm',
		'abs_normal.htm',
		'funcheck.htm',
		'check_for_nan.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
