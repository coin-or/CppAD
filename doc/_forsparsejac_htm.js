var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'adfun.htm',
'sparsity_pattern.htm',
'forsparsejac.htm'
];
var list_current0 = [
'forsparsejac.htm#Syntax',
'forsparsejac.htm#Purpose',
'forsparsejac.htm#f',
'forsparsejac.htm#f.size_forward_bool',
'forsparsejac.htm#f.size_forward_set',
'forsparsejac.htm#x',
'forsparsejac.htm#q',
'forsparsejac.htm#transpose',
'forsparsejac.htm#dependency',
'forsparsejac.htm#r',
'forsparsejac.htm#r.transpose false',
'forsparsejac.htm#r.transpose true',
'forsparsejac.htm#s',
'forsparsejac.htm#s.transpose false',
'forsparsejac.htm#s.transpose true',
'forsparsejac.htm#SetVector',
'forsparsejac.htm#Entire Sparsity Pattern',
'forsparsejac.htm#Example'
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
function choose_down3(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down3[index-1];
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
