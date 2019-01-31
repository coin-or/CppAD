// Child table for section install
document.write('\
<select onchange="install_child(this)">\
<option>install-&gt;</option>\
<option>download</option>\
<option>cmake</option>\
<option>cmake_check</option>\
<option>pkgconfig</option>\
</select>\
');
function install_child(item)
{	var child_list = [
		'download.htm',
		'cmake.htm',
		'cmake_check.htm',
		'pkgconfig.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
