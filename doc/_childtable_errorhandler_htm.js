// Child table for section errorhandler
document.write('\
<select onchange="errorhandler_child(this)">\
<option>errorhandler-&gt;</option>\
<option>error_handler.cpp</option>\
<option>cppad_assert</option>\
</select>\
');
function errorhandler_child(item)
{	var child_list = [
		'error_handler.cpp.htm',
		'cppad_assert.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
