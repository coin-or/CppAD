// Child table for section ad
document.write('\
<select onchange="ad_child(this)">\
<option>ad-&gt;</option>\
<option>ad_ctor</option>\
<option>ad_assign</option>\
<option>Convert</option>\
<option>ADValued</option>\
<option>BoolValued</option>\
<option>VecAD</option>\
<option>base_require</option>\
</select>\
');
function ad_child(item)
{	var child_list = [
		'ad_ctor.htm',
		'ad_assign.htm',
		'convert.htm',
		'advalued.htm',
		'boolvalued.htm',
		'vecad.htm',
		'base_require.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
