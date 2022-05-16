var Cookie            = document.cookie;
var Open              = '';
var SectionCount      = 882   ;
var ImagesInitialized = false;
Children              = new Array();
Initialize();

function Initialize()
{	var i;

	if( Cookie.length == 10 + SectionCount )
		Open = Cookie.substring(9, 10 + SectionCount);
	else
	{	Open = '_T';
		for(i = 2; i <= SectionCount; i++)
			Open = Open + 'F';	
	}
	for (i = 1; i <= SectionCount; i++)
	{
		var child   = document.getElementById('children'+i);
		Children[i] = child;
		if( child != null )
		{	if( Open.charAt(i) == 'T' )
				child.style.display  = '';     // visible
			else	child.style.display  = 'none'; // invisible
		}
	}
}
function InitializeImages()
{	// would like to do this during initialization
	// but image variable seems to come out null then

	for (i = 1; i <= SectionCount; i++)
	{
		if( Children[i] != null )
		{	var image   = document.images['folder'+i];
			if( Open.charAt(i) == 'T' )
				image.src  = '_open.gif';
			else	image.src  = '_close.gif';
		}
	}
	ImagesInitialized = true;
}

// select a folder
function Select(index)
{
	if( ! ImagesInitialized )
		InitializeImages();

	// cannot be null becasue is visible
	var child = Children[index];
	var image = document.images['folder'+index];

	if( child.style.display == 'none' )
	{	image.src           = '_openblue.gif';
		child.style.display = '';
		Open = Open.substring(0, index) 
		     + 'T' + Open.substring(index, SectionCount);
	}
	else
	{	image.src           = '_closeblue.gif';
		child.style.display = 'none';
		Open = Open.substring(0, index) 
		     + 'F' + Open.substring(index, SectionCount);
	}
	document.cookie = 'contents=' + Open;
}

// high light a folder
function MouseOver(index)
{
	if( ! ImagesInitialized )
		InitializeImages();

	// cannot be null becasue is visible
	var child = Children[index];
	var image = document.images['folder'+index];

	if( child.style.display == '' )
		image.src = '_openblue.gif';
	else	image.src = '_closeblue.gif';
}

// remove high lighting
function MouseOut(index)
{
	if( ! ImagesInitialized )
		InitializeImages();

	// cannot be null becasue is visible
	var child = Children[index];
	var image = document.images['folder'+index];

	if( child.style.display == '' )
		image.src = '_open.gif';
	else	image.src = '_close.gif';
}
