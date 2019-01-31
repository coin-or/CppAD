// Child table for section eigen_prefix
document.write('\
<select onchange="eigen_prefix_child(this)">\
<option>eigen_prefix-&gt;</option>\
<option>get_eigen.sh</option>\
</select>\
');
function eigen_prefix_child(item)
{	var child_list = [
		'get_eigen.sh.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
