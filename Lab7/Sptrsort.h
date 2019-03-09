
#ifndef __SPTRSORT_H_
#define __SPTRSORT_H_

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
//Smart pointer wrapper class implementation
template<typename T>
class sptr 
{
	public:
		sptr(T*_ptr = NULL) { ptr = _ptr; }
		bool operator< (const sptr &rhs) const { return *ptr < *rhs.ptr; } //This is what allows sorting in main
		operator T * () const { return ptr; }
	private:
		T* ptr;
};

//Smart pointer vector created and given proper resizing. Then the contructor is utulized to allow for pointers and smart pointers to be 
//compared. The smart pointer vector is then sorted and the list is re-linked.
template <typename T>
void sptrsort(vector<T*> &A)
{
	vector < sptr<T> > Ap;

	Ap.resize(A.size());
	
	for (int i = 0; i < A.size(); i++)
	{
		 sptr<T> temp(A[i]);
		 Ap[i] = temp;
	}

	sort(Ap.begin(), Ap.end());
	
	for (int i = 0; i < A.size(); i++)
	{
		A[i] = Ap[i];
	}

}

#endif
