// Child table for section base2ad
document.write('\
<select onchange="base2ad_child(this)">\
<option>base2ad-&gt;</option>\
<option>base2ad.cpp</option>\
<option>base2vec_ad.cpp</option>\
</select>\
');
function base2ad_child(item)
{	var child_list = [
		'base2ad.cpp.htm',
		'base2vec_ad.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
