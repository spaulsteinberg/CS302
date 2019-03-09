/*
	Author: Sam Steinberg
	Date: January 29th, 2018
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <ctime>
using namespace std;

template <typename Tdata>
//Master while loop to contain both the partioning and quickselect portion, everything else is the same as quickselect
void quickselect(vector<Tdata> &v, int l, int target, int r) 
{	
		while(1)
		{
		int N = r - l + 1;
		int pivot = rand() % N + l;
			if ( r < l )
			{
				return;
			}
			swap(v[pivot],v[r]);
			pivot = r;
	
			int i = l-1; 
			int j = r;
				while(1){
				while(v[++i] < v[pivot]){ }
				while(--j >= l && v[pivot] < v[j]){ }
		
					if (i >= j)
					{
						break;
					}
					swap(v[i], v[j]);
					}		
						swap(v[i],v[pivot]);
						pivot = i;
		
//These if/else statements are for quickselect, returns when the pivot and target are the same. If not, r and l will change
		if (pivot == target)
			return;
		if (target < pivot) quickselect(v,l,target,pivot-1);
		else				quickselect(v,pivot+1,target,r);
		}
}

template <typename Tdata>
void quicksort(vector<Tdata> &v, int l, int r) 
{
	//Keeping right in bounds
	if (r < l)
	{
		return;
	}
	int N = r - l + 1; //N denotes size of vector, 1 added to get actual size of vectors
	int pivot = rand() % N + l; //finding pivot from a random number within the range of vector
	
	swap(v[pivot],v[r]); //pivot moved to right-most position
	pivot = r;
    //i and j are created to inc/dec with the positions of l and r being held
	int i = l-1; 
	int j = r;

	/*Partioning portion: i and j will continue to increment/decrement while the 
	master loop still runs (this stops when i and j cross) */
	while(1){
		/*These while loops will run and do nothing when a change shouldnt be made
		like when the pivot is greater than i. It will break and swap when a change should be made
	*/
		while(v[++i] < v[pivot]){ }
		while(--j >= l && v[pivot] < v[j]){ }
		
		if (i >= j) //When j and i cross
		{
			break;
		}
		swap(v[i], v[j]); //swapping i and j values whil they are both inc/dec and not crossing
	}
		
	swap(v[i],v[pivot]); //makes swap of pivot and i after they cross
	pivot = i;
	
	//Recursion: sorting the left and right around the pivot points
	quicksort(v, l, pivot-1);
	quicksort(v, pivot+1, r);



}

class person_t {
  public:
    person_t() { }

	bool operator<(const person_t &)const;

    friend istream & operator>>(istream &, person_t &);
    friend ostream & operator<<(ostream &, const person_t &);

  private:
    string firstname;
    string lastname;
    string phonenum;
};
//Comparison of person_t objects to determine order, compares first, last, phonenum
//true denotes passed argument is greater
bool person_t::operator<(const person_t &rhs)const
{
	
	if (lastname < rhs.lastname)
	{
		return true;
	}
	
	if (lastname == rhs.lastname)
	{
		if (firstname < rhs.firstname)
		{
			return true;
		}
	
		if (firstname == rhs.firstname)
		{
			if (phonenum < rhs.phonenum)
			{
				return true;
			}
		}
	}
	return false;
}
//Simply reads in data from person_t class
istream & operator>>(istream &in, person_t &r) { 
	in >> r.firstname;
	in >> r.lastname;
	in >> r.phonenum;

}
//Outputs data with a constant field width
ostream & operator<<(ostream &out, const person_t &r) {
    int field_width = 22 - (r.lastname.length());
	out << r.lastname << " " << setw(field_width) << left << r.firstname << " " << r.phonenum;

}
//Template based function that prints the list
template <typename T>
void printlist(T p1, T p2)
{
	while (p1 != p2)
	{
		cout << *p1 << endl;
		++p1;
	}
}

int main(int argc, char *argv[]) {
  // perform command-line check 
  srand(time(NULL));
  vector<person_t> A;

  person_t din;
  while (cin >> din)
    A.push_back(din);
  
  int N = (int)A.size();
  int k0 = 0;
  int k1 = N-1;
  //Error checking: must have either 1 or 3 arguments
  if (argc != 1 && argc !=3)
  {
		cerr << "Usage: Check arguments." << endl;
		return 0;
  }
   //Call for quicksort 
  if (argc == 1) quicksort(A, k0, k1);
  //Call for quickselect, first and second argv's converted to ints and used to sort the selected bounds
  if (argc == 3)
  {
	int k_lower = atoi(argv[1]);
	int k_upper = atoi(argv[2]);
	quickselect(A, 0, k_lower, N-1);
	quickselect(A, k_lower, k_upper, N-1);
  }
  //Call for printing of list that is now sorted
  printlist(A.begin(), A.end());
}
