// Child table for section sparse2eigen
document.write('\
<select onchange="sparse2eigen_child(this)">\
<option>sparse2eigen-&gt;</option>\
<option>sparse2eigen.cpp</option>\
</select>\
');
function sparse2eigen_child(item)
{	var child_list = [
		'sparse2eigen.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
