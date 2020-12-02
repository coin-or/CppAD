var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'install.htm',
'pkgconfig.htm'
];
var list_current0 = [
'pkgconfig.htm#Purpose',
'pkgconfig.htm#cppad.pc',
'pkgconfig.htm#cppad.pc.PKG_CONFIG_PATH',
'pkgconfig.htm#cppad.pc.Compile Flags',
'pkgconfig.htm#cppad.pc.Link Flags',
'pkgconfig.htm#cppad.pc.Extra Definitions',
'pkgconfig.htm#cppad-uninstalled.pc',
'pkgconfig.htm#cppad-uninstalled.pc.PKG_CONFIG_PATH',
'pkgconfig.htm#cppad-uninstalled.pc.Compile Flags',
'pkgconfig.htm#cppad-uninstalled.pc.Link Flags',
'pkgconfig.htm#cppad-uninstalled.pc.Extra Definitions'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_up0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_up0[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}
