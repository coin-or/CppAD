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
'glossary.htm'
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
'faq.htm',
'directory.htm',
'theory.htm',
'glossary.htm',
'bib.htm',
'wish_list.htm',
'whats_new.htm',
'deprecated.htm',
'compare_c.htm',
'numeric_ad.htm',
'addon.htm',
'license.htm'
];
var list_current0 = [
'glossary.htm#AD Function',
'glossary.htm#AD of Base',
'glossary.htm#AD Type Above Base',
'glossary.htm#Base Function',
'glossary.htm#Base Type',
'glossary.htm#Dynamic Parameter',
'glossary.htm#Elementary Vector',
'glossary.htm#Operation',
'glossary.htm#Operation.Atomic',
'glossary.htm#Operation.Sequence',
'glossary.htm#Operation.Dependent',
'glossary.htm#Operation.Independent',
'glossary.htm#Parameter',
'glossary.htm#Row-major Representation',
'glossary.htm#Sparsity Pattern',
'glossary.htm#Sparsity Pattern.Row and Column Index Vectors',
'glossary.htm#Sparsity Pattern.Boolean Vector',
'glossary.htm#Sparsity Pattern.Vector of Sets',
'glossary.htm#Tape',
'glossary.htm#Tape.Active',
'glossary.htm#Tape.Inactive',
'glossary.htm#Tape.Independent Variable',
'glossary.htm#Tape.Dependent Variables',
'glossary.htm#Taylor Coefficient',
'glossary.htm#Variable'
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
