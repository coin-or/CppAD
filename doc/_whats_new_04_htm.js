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
'whats_new.htm',
'whats_new_04.htm'
];
var list_down3 = [
'install.htm',
'theory.htm',
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
var list_down2 = [
'faq.htm',
'directory.htm',
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
var list_down1 = [
'whats_new_19.htm',
'whats_new_18.htm',
'whats_new_17.htm',
'whats_new_16.htm',
'whats_new_15.htm',
'whats_new_14.htm',
'whats_new_13.htm',
'whats_new_12.htm',
'whats_new_11.htm',
'whats_new_10.htm',
'whats_new_09.htm',
'whats_new_08.htm',
'whats_new_07.htm',
'whats_new_06.htm',
'whats_new_05.htm',
'whats_new_04.htm',
'whats_new_03.htm'
];
var list_current0 = [
'whats_new_04.htm#Introduction',
'whats_new_04.htm#12-11',
'whats_new_04.htm#12-09',
'whats_new_04.htm#12-03',
'whats_new_04.htm#11-17',
'whats_new_04.htm#11-16',
'whats_new_04.htm#11-15',
'whats_new_04.htm#11-14',
'whats_new_04.htm#11-13',
'whats_new_04.htm#11-12',
'whats_new_04.htm#11-10',
'whats_new_04.htm#11-04',
'whats_new_04.htm#11-02',
'whats_new_04.htm#11-01',
'whats_new_04.htm#10-29',
'whats_new_04.htm#10-28',
'whats_new_04.htm#10-27',
'whats_new_04.htm#10-21',
'whats_new_04.htm#10-19',
'whats_new_04.htm#10-16',
'whats_new_04.htm#10-06',
'whats_new_04.htm#09-29',
'whats_new_04.htm#09-26',
'whats_new_04.htm#09-23',
'whats_new_04.htm#09-21',
'whats_new_04.htm#09-13',
'whats_new_04.htm#09-10',
'whats_new_04.htm#09-09',
'whats_new_04.htm#09-07',
'whats_new_04.htm#09-04',
'whats_new_04.htm#09-02',
'whats_new_04.htm#08-27',
'whats_new_04.htm#08-25',
'whats_new_04.htm#08-24',
'whats_new_04.htm#08-12',
'whats_new_04.htm#07-31',
'whats_new_04.htm#07-08',
'whats_new_04.htm#07-07',
'whats_new_04.htm#07-03',
'whats_new_04.htm#07-02',
'whats_new_04.htm#06-29',
'whats_new_04.htm#06-25',
'whats_new_04.htm#06-12',
'whats_new_04.htm#06-04',
'whats_new_04.htm#06-03',
'whats_new_04.htm#06-01',
'whats_new_04.htm#05-30',
'whats_new_04.htm#05-29',
'whats_new_04.htm#05-26',
'whats_new_04.htm#05-25',
'whats_new_04.htm#05-14',
'whats_new_04.htm#05-12',
'whats_new_04.htm#05-09',
'whats_new_04.htm#05-07',
'whats_new_04.htm#05-04',
'whats_new_04.htm#05-03',
'whats_new_04.htm#04-29',
'whats_new_04.htm#04-28',
'whats_new_04.htm#04-25',
'whats_new_04.htm#04-24',
'whats_new_04.htm#04-22',
'whats_new_04.htm#04-21',
'whats_new_04.htm#04-20',
'whats_new_04.htm#04-19',
'whats_new_04.htm#04-09',
'whats_new_04.htm#04-08',
'whats_new_04.htm#04-07',
'whats_new_04.htm#04-03',
'whats_new_04.htm#04-02',
'whats_new_04.htm#04-01',
'whats_new_04.htm#03-30',
'whats_new_04.htm#03-28',
'whats_new_04.htm#03-25',
'whats_new_04.htm#03-18',
'whats_new_04.htm#03-17',
'whats_new_04.htm#03-15',
'whats_new_04.htm#03-12',
'whats_new_04.htm#03-11',
'whats_new_04.htm#03-09',
'whats_new_04.htm#03-07',
'whats_new_04.htm#03-06',
'whats_new_04.htm#03-05',
'whats_new_04.htm#03-04',
'whats_new_04.htm#03-03',
'whats_new_04.htm#03-01',
'whats_new_04.htm#02-29',
'whats_new_04.htm#02-28',
'whats_new_04.htm#02-21',
'whats_new_04.htm#02-20',
'whats_new_04.htm#02-17',
'whats_new_04.htm#02-16',
'whats_new_04.htm#02-15',
'whats_new_04.htm#02-12',
'whats_new_04.htm#02-01',
'whats_new_04.htm#01-29',
'whats_new_04.htm#01-28',
'whats_new_04.htm#01-22'
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
