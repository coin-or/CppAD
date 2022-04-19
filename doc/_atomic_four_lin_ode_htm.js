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
'advalued.htm',
'atomic.htm',
'atomic_four.htm',
'atomic_four_example.htm',
'atomic_four_lin_ode.htm'
];
var list_current0 = [
'atomic_four_lin_ode.htm#Under construction',
'atomic_four_lin_ode.htm#Syntax',
'atomic_four_lin_ode.htm#Purpose',
'atomic_four_lin_ode.htm#y',
'atomic_four_lin_ode.htm#y.m',
'atomic_four_lin_ode.htm#call_id',
'atomic_four_lin_ode.htm#r',
'atomic_four_lin_ode.htm#pattern',
'atomic_four_lin_ode.htm#pattern.nnz',
'atomic_four_lin_ode.htm#pattern.row',
'atomic_four_lin_ode.htm#pattern.col',
'atomic_four_lin_ode.htm#transpose',
'atomic_four_lin_ode.htm#x',
'atomic_four_lin_ode.htm#x.n',
'atomic_four_lin_ode.htm#x.A(x)',
'atomic_four_lin_ode.htm#x.b(x)',
'atomic_four_lin_ode.htm#Theory',
'atomic_four_lin_ode.htm#Theory.Forward',
'atomic_four_lin_ode.htm#Theory.Reverse',
'atomic_four_lin_ode.htm#Method',
'atomic_four_lin_ode.htm#Method.Rosen34',
'atomic_four_lin_ode.htm#Method.Simpon\'s Rule',
'atomic_four_lin_ode.htm#Contents'
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
