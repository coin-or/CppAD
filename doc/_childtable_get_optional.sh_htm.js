// Child table for section get_optional.sh
document.write('\
<select onchange="get_optional__46__sh_child(this)">\
<option>get_optional.sh-&gt;</option>\
<option>get_colpack.sh</option>\
<option>get_adolc.sh</option>\
<option>get_eigen.sh</option>\
<option>get_fadbad.sh</option>\
<option>get_ipopt.sh</option>\
<option>get_sacado.sh</option>\
</select>\
');
function get_optional__46__sh_child(item)
{	var child_list = [
		'get_colpack.sh.htm',
		'get_adolc.sh.htm',
		'get_eigen.sh.htm',
		'get_fadbad.sh.htm',
		'get_ipopt.sh.htm',
		'get_sacado.sh.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
