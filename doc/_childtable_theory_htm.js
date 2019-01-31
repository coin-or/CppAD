// Child table for section theory
document.write('\
<select onchange="theory_child(this)">\
<option>theory-&gt;</option>\
<option>Introduction</option>\
<option>ForwardTheory</option>\
<option>ReverseTheory</option>\
<option>reverse_identity</option>\
<option>taylor_ode</option>\
<option>research</option>\
</select>\
');
function theory_child(item)
{	var child_list = [
		'introduction.htm',
		'forwardtheory.htm',
		'reversetheory.htm',
		'reverse_identity.htm',
		'taylor_ode.htm',
		'research.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
