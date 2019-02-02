// Child table for section chkpoint_two
document.write('\
<select onchange="chkpoint_two_child(this)">\
<option>chkpoint_two-&gt;</option>\
<option>chkpoint_two_ctor</option>\
<option>chkpoint_two_chk_fun</option>\
<option>chkpoint_two_dynamic</option>\
<option>chkpoint_two_get_started.cpp</option>\
<option>chkpoint_two_compare.cpp</option>\
<option>chkpoint_two_base2ad.cpp</option>\
<option>chkpoint_two_dynamic.cpp</option>\
<option>chkpoint_two_ode.cpp</option>\
</select>\
');
function chkpoint_two_child(item)
{	var child_list = [
		'chkpoint_two_ctor.htm',
		'chkpoint_two_chk_fun.htm',
		'chkpoint_two_dynamic.htm',
		'chkpoint_two_get_started.cpp.htm',
		'chkpoint_two_compare.cpp.htm',
		'chkpoint_two_base2ad.cpp.htm',
		'chkpoint_two_dynamic.cpp.htm',
		'chkpoint_two_ode.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
