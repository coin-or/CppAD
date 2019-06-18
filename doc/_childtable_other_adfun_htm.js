// Child table for section other_adfun
document.write('\
<select onchange="other_adfun_child(this)">\
<option>other_adfun-&gt;</option>\
<option>json2adfun</option>\
<option>base2ad</option>\
</select>\
');
function other_adfun_child(item)
{	var child_list = [
		'json2adfun.htm',
		'base2ad.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
