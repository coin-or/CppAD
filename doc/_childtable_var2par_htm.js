// Child table for section var2par
document.write('\
<select onchange="var2par_child(this)">\
<option>var2par-&gt;</option>\
<option>var2par.cpp</option>\
</select>\
');
function var2par_child(item)
{	var child_list = [
		'var2par.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
