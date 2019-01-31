var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'appendix.htm',
'faq.htm'
];
var list_current0 = [
'faq.htm#Assignment and Independent',
'faq.htm#Bugs',
'faq.htm#CompareChange',
'faq.htm#Complex Types',
'faq.htm#Exceptions',
'faq.htm#Independent Variables',
'faq.htm#Matrix Inverse',
'faq.htm#Matrix Inverse.LuSolve',
'faq.htm#Matrix Inverse.Atomic Operation',
'faq.htm#Mode: Forward or Reverse',
'faq.htm#Namespace',
'faq.htm#Namespace.Test Vector Preprocessor Symbol',
'faq.htm#Speed',
'faq.htm#Speed.NDEBUG',
'faq.htm#Speed.Optimize',
'faq.htm#Speed.Memory Allocation',
'faq.htm#Tape Storage: Disk or Memory'
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
