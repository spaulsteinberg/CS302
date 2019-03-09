#ifndef __PERSON_H__ 
#define __PERSON_H__

#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

enum person_enum  { UNKNOWN, FACULTY, STUDENT };
enum faculty_enum { ASST_PROF, ASSOC_PROF, FULL_PROF };
enum student_enum { FRESHMAN, SOPHOMORE, JUNIOR, SENIOR };
/*
	Base class, contains a name and person_type(FACULTY or STUDENT) along with the overloads.
	operator< is used for sorting, operator<< for outputting. Setters and getters for accessing members. Pure virtual void for derived.
*/
class person
{
	public:
		friend ostream& operator<<(ostream &, person&);
		string get_name() const { return name; }
		string get_type() const { return person_type; }
		void set_name(string n) { name = n; }
		void set_type(string t) { person_type = t; }
		bool operator <(const person&);
	protected:
		string name;
		string person_type;
		virtual void print_details(const person&) = 0;
		virtual void print_courses() = 0;
};
/*
	Derived class, sets up functions for faculty variations.
*/
class faculty : public person
{
	public:
		void add_course(string);
		faculty_enum get_faculty_type() const { return faculty_type; }
		void set_faculty_type(faculty_enum f) { faculty_type = f; }
		bool operator<(faculty&);
	protected:
		faculty_enum faculty_type;
		vector<string> courselist;
		void print_details(const person&);
		void print_courses();
	
};
/*
	Derived class, sets up functions for student variations.
*/
class student : public person
{
	public:
		void add_course(string);
		void add_gpa(float);
		student_enum get_student_type() const { return student_type; }
		void set_student_type(student_enum s) { student_type = s; }
		bool operator<(student&);
	protected:
		student_enum student_type;
		vector<float> gpa;
		vector<string> courselist;
		void print_details(const person&);
		void print_courses();
	
};//derived class

#endif
