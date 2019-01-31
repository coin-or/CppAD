// Child table for section numeric_ad
document.write('\
<select onchange="numeric_ad_child(this)">\
<option>numeric_ad-&gt;</option>\
<option>BenderQuad</option>\
<option>opt_val_hes</option>\
<option>LuRatio</option>\
</select>\
');
function numeric_ad_child(item)
{	var child_list = [
		'benderquad.htm',
		'opt_val_hes.htm',
		'luratio.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
