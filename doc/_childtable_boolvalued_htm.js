// Child table for section boolvalued
document.write('\
<select onchange="boolvalued_child(this)">\
<option>boolvalued-&gt;</option>\
<option>Compare</option>\
<option>NearEqualExt</option>\
<option>BoolFun</option>\
<option>con_dyn_var</option>\
<option>EqualOpSeq</option>\
</select>\
');
function boolvalued_child(item)
{	var child_list = [
		'compare.htm',
		'nearequalext.htm',
		'boolfun.htm',
		'con_dyn_var.htm',
		'equalopseq.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
