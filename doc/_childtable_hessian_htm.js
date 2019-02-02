// Child table for section hessian
document.write('\
<select onchange="hessian_child(this)">\
<option>hessian-&gt;</option>\
<option>hessian.cpp</option>\
<option>hes_lagrangian.cpp</option>\
</select>\
');
function hessian_child(item)
{	var child_list = [
		'hessian.cpp.htm',
		'hes_lagrangian.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
