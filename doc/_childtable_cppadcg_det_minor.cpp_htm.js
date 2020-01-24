// Child table for section cppadcg_det_minor.cpp
document.write('\
<select onchange="cppadcg_det_minor__46__cpp_child(this)">\
<option>cppadcg_det_minor.cpp-&gt;</option>\
<option>cppadcg_det_minor_grad.c</option>\
<option>cppadcg_det_minor_cg.cpp</option>\
</select>\
');
function cppadcg_det_minor__46__cpp_child(item)
{	var child_list = [
		'cppadcg_det_minor_grad.c.htm',
		'cppadcg_det_minor_cg.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
