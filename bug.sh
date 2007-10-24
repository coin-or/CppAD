#! /bin/bash
#
echo "Assuming ../configure was executed with no arguments,"
echo "This generates a segmentation falut under Cygwin (when -O2 is used):"
echo
#
cat << EOF > bug0.hpp
# include <string>

class TrackElement {
	
public:
	std::string   file;   // corresponding file name
	void          *ptr;   // value returned by TrackNew
	TrackElement *next;   // next element in linked list

	TrackElement(void *p)
	: file(""), ptr(p), next(0)
	{ }

	// There is only one tracking list and it starts it here
	static TrackElement *root_for(void)
	{	static TrackElement root(0);
		return &root;
	}

}; 
inline double *TrackNewVec(size_t len)
{
	// try to allocate the new memrory
	double *newptr = new double[len];
	if( newptr == 0 )
	{	std::string str("test");
		char message[5];
		message[0] = str[0];
		message[1] = '\0';
	}
	// create tracking element
	void *vptr = static_cast<void *>(newptr);
	TrackElement *E = new TrackElement(vptr);

	// get the root
	TrackElement *root = TrackElement::root_for();

	// put this elemenent at the front of linked list
	E->next    = root->next;  
	root->next = E;

	return newptr;
}
inline void TrackDelVec(double *oldptr )
{
	TrackElement        *P;
	TrackElement        *E;

	// search list for pointer
	P          = TrackElement::root_for();
	E          = P->next;
	void *vptr = static_cast<void *>(oldptr);
	while(E != 0 && E->ptr != vptr)
	{	P = E;
		E = E->next;
	}

	// remove tracking element from list
	if( E != 0 )
		P->next = E->next;

	// delete allocated pointer
	delete [] oldptr;

	// delete tracking element
	delete E;

	return;
}
inline size_t TrackCount(void)
{
	size_t count = 0;
	TrackElement *E = TrackElement::root_for();

	while( E->next != 0 ) 
	{	++count;
		E = E->next;
	}
	return count;
}
EOF
cat << EOF >  bug1.cpp
# include "bug0.hpp"
extern bool bug2(void);
int main(void)
{	size_t count = TrackCount();
	bug2();
	return 0;
}
EOF
cat << EOF > bug2.cpp
# include "bug0.hpp"
bool bug2(void)
{	double *x = 0;
	x = TrackNewVec(1);
	x[0] = .4; 
	TrackDelVec(x);
	return true;
}
EOF
echo "g++ bug1.cpp bug2.cpp -O1 -o bug.exe"
g++ bug1.cpp bug2.cpp  -O1 -o bug.exe
echo "./bug"
./bug
echo "g++ bug1.cpp bug2.cpp -O2 -o bug.exe"
g++ bug1.cpp bug2.cpp  -O2 -o bug.exe
echo "./bug"
./bug
