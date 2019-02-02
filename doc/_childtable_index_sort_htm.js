// Child table for section index_sort
document.write('\
<select onchange="index_sort_child(this)">\
<option>index_sort-&gt;</option>\
<option>index_sort.cpp</option>\
</select>\
');
function index_sort_child(item)
{	var child_list = [
		'index_sort.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
