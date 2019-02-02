// Child table for section sign
document.write('\
<select onchange="sign_child(this)">\
<option>sign-&gt;</option>\
<option>sign.cpp</option>\
</select>\
');
function sign_child(item)
{	var child_list = [
		'sign.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
