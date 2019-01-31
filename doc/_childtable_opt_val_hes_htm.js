// Child table for section opt_val_hes
document.write('\
<select onchange="opt_val_hes_child(this)">\
<option>opt_val_hes-&gt;</option>\
<option>opt_val_hes.cpp</option>\
</select>\
');
function opt_val_hes_child(item)
{	var child_list = [
		'opt_val_hes.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
