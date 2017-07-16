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
'download.htm'
];
var list_down2 = [
'install.htm',
'introduction.htm',
'ad.htm',
'adfun.htm',
'preprocessor.htm',
'multi_thread.htm',
'utility.htm',
'ipopt_solve.htm',
'example.htm',
'speed.htm',
'appendix.htm'
];
var list_down1 = [
'download.htm',
'cmake.htm',
'cmake_check.htm',
'pkgconfig.htm'
];
var list_current0 = [
'download.htm#Purpose',
'download.htm#Distribution Directory',
'download.htm#Compressed Archives',
'download.htm#Current Version',
'download.htm#Current Version.Compressed Archives',
'download.htm#Current Version.Subversion',
'download.htm#Current Version.Git',
'download.htm#Release Versions',
'download.htm#Release Versions.Coin',
'download.htm#Release Versions.github',
'download.htm#Monthly Versions',
'download.htm#Daily Versions',
'download.htm#Windows File Extraction and Testing',
'download.htm#Install Instructions',
'download.htm#No Documentation',
'download.htm#No Documentation.Building Documentation'
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
