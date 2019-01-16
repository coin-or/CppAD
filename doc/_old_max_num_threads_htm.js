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
'deprecated.htm',
'omp_alloc.htm',
'old_max_num_threads.htm'
];
var list_down3 = [
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
var list_down2 = [
'include_deprecated.htm',
'fundeprecated.htm',
'comparechange.htm',
'omp_max_thread.htm',
'tracknewdel.htm',
'omp_alloc.htm',
'memory_leak.htm',
'epsilon.htm',
'test_vector.htm',
'cppad_ipopt_nlp.htm',
'atomic_one.htm',
'atomic_two.htm',
'atomic_two_example.htm',
'chkpoint_one.htm',
'zdouble.htm',
'autotools.htm'
];
var list_down1 = [
'omp_max_num_threads.htm',
'omp_in_parallel.htm',
'omp_get_thread_num.htm',
'omp_get_memory.htm',
'omp_return_memory.htm',
'omp_free_available.htm',
'omp_inuse.htm',
'omp_available.htm',
'omp_create_array.htm',
'omp_delete_array.htm',
'omp_efficient.htm',
'old_max_num_threads.htm'
];
var list_current0 = [
'old_max_num_threads.htm#Removed',
'old_max_num_threads.htm#Syntax',
'old_max_num_threads.htm#Purpose',
'old_max_num_threads.htm#number',
'old_max_num_threads.htm#Restrictions'
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
