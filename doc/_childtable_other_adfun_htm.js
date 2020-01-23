// Child table for section other_adfun
document.write('\
<select onchange="other_adfun_child(this)">\
<option>other_adfun-&gt;</option>\
<option>base2ad</option>\
<option>json_ad_graph</option>\
<option>cpp_ad_graph</option>\
<option>abs_normal_fun</option>\
</select>\
');
function other_adfun_child(item)
{	var child_list = [
		'base2ad.htm',
		'json_ad_graph.htm',
		'cpp_ad_graph.htm',
		'abs_normal_fun.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
