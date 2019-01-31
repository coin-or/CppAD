// Child table for section condexp
document.write('\
<select onchange="condexp_child(this)">\
<option>condexp-&gt;</option>\
<option>cond_exp.cpp</option>\
</select>\
');
function condexp_child(item)
{	var child_list = [
		'cond_exp.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
