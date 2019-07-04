// Child table for section appendix
document.write('\
<select onchange="appendix_child(this)">\
<option>appendix-&gt;</option>\
<option>Faq</option>\
<option>directory</option>\
<option>glossary</option>\
<option>Bib</option>\
<option>wish_list</option>\
<option>whats_new</option>\
<option>deprecated</option>\
<option>compare_c</option>\
<option>numeric_ad</option>\
<option>addon</option>\
<option>License</option>\
</select>\
');
function appendix_child(item)
{	var child_list = [
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
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
