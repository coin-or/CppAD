/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
$begin TrackNewDel.cpp$$

$section Tracking Use of New and Delete: Example and Test$$

$index new, example$$
$index example, new$$
$index test, new$$

$index delete, example$$
$index example, delete$$
$index test, delete$$

$code
$verbatim%Example/TrackNewDel.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/TrackNewDel.h>

bool TrackNewDel(void)
{	bool ok = true;

	// initial count
	size_t count = CppADTrackCount();

	// allocate an array of lenght 5
	double *ptr = 0;
	size_t  newlen = 5;
	ptr = CppADTrackNewVec(newlen, ptr);

	// copy data into the array
	size_t ncopy = newlen;
	size_t i;
	for(i = 0; i < ncopy; i++)
		ptr[i] = double(i);

	// extend the buffer to be lenght 10
	newlen = 10;
	ptr    = CppADTrackExtend(newlen, ncopy, ptr);
		
	// copy data into the new part of the array
	for(i = ncopy; i < newlen; i++)
		ptr[i] = double(i);

	// check the values in the array
 	for(i = 0; i < newlen; i++)
		ok &= (ptr[i] == double(i));

	// free the memory allocated since previous call to TrackCount
	CppADTrackDelVec(ptr);

	// check for memory leak
	ok &= (count == CppADTrackCount());

	return ok;
}

// END PROGRAM
