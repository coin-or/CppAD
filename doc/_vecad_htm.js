var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'ad.htm',
'vecad.htm'
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
'ad_ctor.htm',
'ad_assign.htm',
'convert.htm',
'advalued.htm',
'boolvalued.htm',
'vecad.htm',
'base_require.htm'
];
var list_down0 = [
'vec_ad.cpp.htm'
];
var list_current0 = [
'vecad.htm#Syntax',
'vecad.htm#Purpose',
'vecad.htm#Alternatives',
'vecad.htm#VecAD&lt;Base&gt;::reference',
'vecad.htm#VecAD&lt;Base&gt;::reference.Exceptions',
'vecad.htm#Constructor',
'vecad.htm#Constructor.v',
'vecad.htm#n',
'vecad.htm#size',
'vecad.htm#size_t Indexing',
'vecad.htm#size_t Indexing.i',
'vecad.htm#size_t Indexing.b',
'vecad.htm#AD Indexing',
'vecad.htm#AD Indexing.x',
'vecad.htm#AD Indexing.r',
'vecad.htm#Dynamic Parameters',
'vecad.htm#Example',
'vecad.htm#Speed and Memory'
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
