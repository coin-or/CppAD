// Child table for section det_grad_33
document.write('\
<select onchange="det_grad_33_child(this)">\
<option>det_grad_33-&gt;</option>\
<option>det_grad_33.hpp</option>\
</select>\
');
function det_grad_33_child(item)
{	var child_list = [
		'det_grad_33.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
