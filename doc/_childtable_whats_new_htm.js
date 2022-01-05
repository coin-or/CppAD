// Child table for section whats_new
document.write('\
<select onchange="whats_new_child(this)">\
<option>whats_new-&gt;</option>\
<option>whats_new_22</option>\
<option>whats_new_21</option>\
<option>whats_new_20</option>\
<option>whats_new_19</option>\
<option>whats_new_18</option>\
<option>whats_new_17</option>\
<option>whats_new_16</option>\
<option>whats_new_15</option>\
<option>whats_new_14</option>\
<option>whats_new_13</option>\
<option>whats_new_12</option>\
<option>whats_new_11</option>\
<option>whats_new_10</option>\
<option>whats_new_09</option>\
<option>whats_new_08</option>\
<option>whats_new_07</option>\
<option>whats_new_06</option>\
<option>whats_new_05</option>\
<option>whats_new_04</option>\
<option>whats_new_03</option>\
</select>\
');
function whats_new_child(item)
{	var child_list = [
		'whats_new_22.htm',
		'whats_new_21.htm',
		'whats_new_20.htm',
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
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
