/*
	Author: Sam Steinberg
	Date: April 19th, 2018
*/
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream> 
#include <utility>
#include <string>
using namespace std;

#include "Person.h"
#include "Sptrsort.h"

int main(int argc, char *argv[]) {

	if (argc != 2) {
    cerr << "Check arguments. " << endl;
	return 0;
  }

  //student and faculty pointers
  student *n_student;
  faculty *n_faculty;
  vector<person *> student_sort;
  vector<person *> faculty_sort;
  //student and faculty lists created
  vector<student *> student_list;
  vector<faculty *> faculty_list;
  ifstream fin;
  fin.open(argv[1]);

  if (fin.fail())
	  cerr << "File failed to open." << endl;
  
  person_enum person_type = UNKNOWN;
  int line = 0;
  size_t iL, iR;
  string input;
  string name;
  string category;
  vector<string> course;
  vector<float> gp;
  float new_gp;
  while (getline(fin, input)) {
	line++;
	//Extraction of data, which was provided to us.
	if ((iL = input.find('<')) == string::npos) {
	  continue;

    } else if (input.compare(iL,9,"<faculty>") == 0) {
	  person_type = FACULTY;
	  //Clearing of data after each person
	  course.clear();
	  gp.clear();

	} else if (input.compare(iL,10,"</faculty>") == 0) {
	  faculty_enum faculty_type;

	  if (category.compare("Assistant Professor") == 0)
	    faculty_type = ASST_PROF;
	  else if (category.compare("Associate Professor") == 0)
	    faculty_type = ASSOC_PROF;
	  else if (category.compare("Full Professor") == 0)
	    faculty_type = FULL_PROF;

	 // New faculty object instantiated, then sets all necessary information using the pointer and setter functions I wrote.
	  n_faculty = new faculty;
	  n_faculty->set_name(name);
	  n_faculty->set_type(category);
	  n_faculty->set_faculty_type(faculty_type);
	  faculty_list.push_back(n_faculty);
	//This is where add_course is called, fills faculty courselist with  extracted course
	  for (int i = 0; i < (int)course.size(); i++)
	  {
		  n_faculty->add_course(course[i]);
	  }
	  person_type = UNKNOWN;
	  continue;
//Lines 81-96 clear the previous student data and parses the next ones
	} else if (input.compare(iL,9,"<student>") == 0) {
	  person_type = STUDENT;
	  course.clear();
	  gp.clear();

	} else if (input.compare(iL,10,"</student>") == 0) {
	  student_enum student_type;

	  if (category.compare("Freshman") == 0)
	    student_type = FRESHMAN;
	  else if (category.compare("Sophomore") == 0)
	    student_type = SOPHOMORE;
	  else if (category.compare("Junior") == 0)
	    student_type = JUNIOR;
	  else if (category.compare("Senior") == 0)
	    student_type = SENIOR;

	 // Same as faculty, pointer set to new object and setter functions are utulized/
	  n_student = new student;
	  n_student->set_name(name);
	  n_student->set_type(category);
	  n_student->set_student_type(student_type);
	  student_list.push_back(n_student);
	  //For students, we need the course and the gpa at that course, hence the add_gpa function I wrote stores the gpa
	  for (int i = 0; i < (int)course.size(); i++)
	  {
			n_student->add_course(course[i]);
	  }
	  for (int j = 0; j < (int)gp.size(); j++)
	  {
			n_student->add_gpa(gp[j]);
	  }
	  person_type = UNKNOWN;
	  continue;
	//More extraction with substrings retrieving information. For students, this is also where the gpa vector is read in.
	} else if (input.compare(iL,5,"<name") == 0) {
	  iL = input.find("=\"", iL);
	  iR = input.find("\"/>", iL+2);
	  name = input.substr(iL+2,iR-(iL+2));

	} else if (input.compare(iL,10,"<category=") == 0) {
	  iL = input.find("=\"", iL);
	  iR = input.find("\"/>", iL+2);
	  category = input.substr(iL+2,iR-(iL+2));

	} else if (input.compare(iL,7,"<course") == 0) {
	  iL = input.find("=\"", iL);
	  iR = input.find("\"", iL+2);
	  course.push_back(input.substr(iL+2,iR-(iL+2)));
	
	  iL = iR;
	  if (person_type == FACULTY) {
	    iR = input.find("/>", iL+1);
	  } else if (person_type == STUDENT) {
	    iL = input.find("gp=\"", iL);
	    iR = input.find("\"/>", iL+4);

	    stringstream ss;
	    ss << input.substr(iL+4,iR-(iL+3)); 
		ss >> new_gp;
	    gp.push_back(new_gp);
	  }
	}
  }
	fin.close();
		//Required, tells users what commands they can use for stdin
		cout << "command: person" << endl;
		cout << "command: faculty" << endl;
		cout << "command: student" << endl;
		cout << endl;

  for (int i = 0; i < (int)faculty_list.size(); i++)
  {
		faculty_sort.push_back(faculty_list[i]);		
  }
  for (int i = 0; i < (int)student_list.size(); i++)
  {
		student_sort.push_back(student_list[i]);
  }
  string com;
  cout << "command> ";
  while(cin >> com) //Infinite loop exits with CTRL-D
  {
	//Sorting of student and faculty vectors by name using smart pointers
	sptrsort(student_list);
	sptrsort(faculty_list);
	sptrsort(faculty_sort);
	sptrsort(student_sort);
	//If person, simply output the sorted faculty vector then sorted student vector.
	if (com == "person")
	{	

		for (int i = 0; i < (int)faculty_list.size(); i++)
		{
			cout << *faculty_sort[i] << "\n";
		}
		for (int i=0; i<(int)student_list.size(); i++)
		{
			cout << *student_sort[i] << "\n";
		}
	}

	//If faculty, check to see their type using getters that I wrote. Output accordingly.
	else
		if (com == "faculty")
		{
			for (int i = 0; i < (int)faculty_list.size(); i++)
			{
				cout << *faculty_list[i] << "\n";
			}
		}
		//Same as above, but for students
		else
			if (com == "student")
			{
				for (int i = 0; i < (int)student_list.size(); i++)
				{
					cout << *student_list[i] << "\n";
				}
			}
	cout << "command> "; 
  }


  //Deallocation of memory, releases the memory
  for (int i = 0; i < (int)student_list.size(); i++)
  {
		delete student_list[i];
  }
  for (int i = 0; i < (int)faculty_list.size(); i++)
  {
		delete faculty_list[i];
  }
  student_list.clear();
  faculty_list.clear();
}
