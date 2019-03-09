#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
using namespace std;

#include "Person.h"
//Operator calls the print details and print courses functions. Will print uniquely for student or faculty because the base class person
//is a passed parameter
ostream& operator<<(ostream &out, person &rhs)
{
	rhs.print_details(rhs);
	rhs.print_courses();
	return out;
}
//Sorting by name, mainly used for sorting of all persons
bool person::operator<(const person &rhs)
{ 
	if (name < rhs.get_name())
	{
		return true;
	}
	else
		return false;
}
//Adds course to list
void student::add_course(string c)
{
	courselist.push_back(c);
}
//Adds gpa to list
void student::add_gpa(float f)
{
	gpa.push_back(f);
}
//Printing of details with appropriate spacing
void student::print_details(const person &p)
{
	cout << setw(12) << right << fixed << "Name: " << p.get_name() << endl;
	cout << setw(12) << right << fixed << "Category: " << p.get_type() << endl;
}
//Student sorting
bool student::operator<(student &rhs)
{
	if (student_type < rhs.get_student_type())
	{
		return true;
	}
	if (student_type == rhs.get_student_type())
	{
	if (this->get_name() < rhs.get_name())
	{
		return true;
	}
	
	}
	else
		return false;
}
/*
	Printing of student courses requires a vector pair be made with the respective gpa so that they do not get shuffled
	when the course names are sorted. Once this is done and this list is sorted, the cumulative gpa is found by summing and dividing
	by the number of courses at each iteration as required. This is then output in required format.
 */
void student::print_courses()
{
	vector< pair<string,float> > gpa_pair;

	for (int i = 0; i < (int)courselist.size(); i++)
	{
		gpa_pair.push_back(make_pair(courselist[i], gpa[i]));
	}
	sort(courselist.begin(), courselist.end());

	vector<float> c_gpa;
	float sum = 0.0;
	for (int i = 0; i < (int)gpa.size(); i++)
	{
		sum+=gpa[i];
		float k = (sum/(i+1));
		c_gpa.push_back(k);
	}
	cout << setprecision(2);
	for(int i = 0; i < (int)courselist.size(); i++)
	{
		cout << setw(12) << right << fixed << "Course: " << setw(28) << left << setfill(' ') << gpa_pair[i].first <<  gpa_pair[i].second << "  " << setprecision(2) << fixed << c_gpa[i] << endl;
	}
}
//Creation of faculty course list
void faculty::add_course(string co)
{
	courselist.push_back(co);
}
//Required outputting
void faculty::print_details(const person &p)
{
	cout << setw(12) << right << fixed << "Name: " << p.get_name() << endl;
	cout << setw(12) << right << fixed << "Category: " << p.get_type() << endl;
}
//faculty sorting
bool faculty::operator<(faculty& f)
{
	if (faculty_type < f.get_faculty_type())
	{
		return true;
	}
	if (faculty_type == f.get_faculty_type())
	{
		if (this->get_name() < f.get_name())
		{
			return true;
		}
	}
	else
		return false;
}
//Required outputting from filled vector
void faculty::print_courses()
{
	sort(courselist.begin(), courselist.end());
	for (int i = 0; i < (int)courselist.size(); i++)
	{
		cout << setw(12) << right << fixed <<  "Course: " << courselist[i] << endl;
	}
}
