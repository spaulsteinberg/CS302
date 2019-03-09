/*
	Author: Samuel Steinberg
	Date: February 5th, 2018
*/
#include <iostream>
#include <list>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <vector>
using namespace std;

template <class T>
//list class which contains a next pointer and data
class slist {
  private:
    struct node {
      node() { data = T(); next = NULL; } //constructor/initialization
      node(const T &key) { data = key; next = NULL; } //constructor/initialization
      bool operator<(const node &rhs)const { return data < rhs.data; } //operator compares node data with passed 

      T data; 
      node *next; 
    };
   //Smart pointer class which contains a node pointer
   class sptr {
	   public:
		   sptr( node *_ptr=NULL) { ptr = _ptr; } // constructor/initialization
		   bool operator<(const sptr &rhs)const { return *ptr < *rhs.ptr; } //operator compares node pointer values
		   operator node * () const { return ptr; } // overloaded operator returns node pointer
		
	   private:
		   node *ptr; 
   }; 

  public:
	class iterator {
	public:
	  iterator() { p = NULL; }
	  T & operator*() { return p->data; }
	  iterator & operator++() { p = p->next; return *this; }
	  bool operator!=(const iterator & rhs) const { return p != rhs.p; }

	  friend class slist<T>;

	private:
	  iterator(node *n_p) { p = n_p; }
	  node *p;
	};

  public:
    slist();
	~slist();

    void push_back(const T &);
	void sort();

	iterator begin() { return iterator(head->next); }
	iterator end() { return iterator(NULL); }

  private:
	node *head;
	node *tail;
};

template <typename T>
slist<T>::slist() {
  head = new node();
  tail = head;
}

template <typename T>
slist<T>::~slist() {
  while (head->next != NULL) {
    node *p = head->next;
    head->next = p->next;
    delete p;
  }
  delete head;

  head = NULL;
  tail = NULL;
}

template <typename T>
void slist<T>::push_back(const T &din) {
  tail->next = new node(din);
  tail = tail->next;
}

template <typename T>
void slist<T>::sort() {
	
	int num_elements= 0;
	node *s = head -> next;
	//While loop to count number of elements by incementing
	while (s)
	{
		num_elements++;
		s = s -> next;
	}
	//Vector of smart pointers that is sized to the number of counted elements
	vector<sptr> Ap;
	Ap.resize(num_elements);
	
	s = head -> next;
	//Creation of the elements in the smart pointer vector
	for (int i = 0; i < Ap.size(); i++)
	{
		Ap[i] = s;
		s = s -> next;
	}
	std::sort(Ap.begin(), Ap.end());
	s = head;
	//Relinking of list by matching up with smart pointers going "back and forth" between list nodes and elements in vector
	for (int j = 0; j < Ap.size(); j++)
	{
	    s -> next = Ap[j];
		s  = s -> next;
	}
	s -> next = NULL; //Relinking the end of the list
	

}
//person_t class and member functions needed to run program
class person_t 
{
	public:
		person_t() { }
		friend istream & operator>>(istream &, person_t &);
		friend ostream & operator<<(ostream &, const person_t &);
		bool operator<(const person_t &)const;
	private: 
		string firstname;
		string lastname;
		string phonenum;
};
//Overloaded operator to read in last, first, phone
istream & operator>>(istream &in, person_t &rhs)
{ 
	in >> rhs.firstname;
	in >> rhs.lastname;
	in >> rhs.phonenum;
	return in;
}
//Overloaded operator which sets the width of the field and organizes the output by lastname-firstname-phone
ostream & operator<<(ostream &out, const person_t &rhs) 
{
	int field_width = 22 - (rhs.lastname.length());
	out << rhs.lastname << " " << setw(field_width) << left << rhs.firstname << " " << rhs.phonenum;
	return out;
}
//Overloaded < operator compares first, last names and phone numbers in order to compare values
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
//template based print function
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
 //Command line check for arguments, will exit if wrong
  if (argc != 1)
  {
		cerr << "Usage: Check arguments." << endl;
		return 0;
  }

  slist<person_t> A;
  //data read in and function calls
  person_t din;
  while (cin >> din)
    A.push_back(din);

  A.sort();

  printlist(A.begin(), A.end());
}
