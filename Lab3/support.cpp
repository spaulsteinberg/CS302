/*
	Author: Sam Steinberg
	Date: February 13th, 2018 
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <numeric>
using namespace std;

#include "support.h"

const char ETX = 0x3;

void ppm::read(string s) 
{
	ifstream fin;
	//Conversion to c-style string
	fin.open(s.c_str());
	
	//If file fails to actually open
	if (fin.fail())
	{
		cerr << "Usage: File failed to open." << endl;
		exit(0);
	}
	//If the file is open...
	else
		if (fin.is_open())
		{
			//title, column, row, max color value read
			fin >> title >> Ncols >> Nrows >> max_value; 
			while(fin.get() != '\n') { }			
			//Error checking of header: max color value and title
			if ((255 != max_value) || (title != "P6"))
			{
				cerr << "Usage: Incorrect header information read in." << endl;
				exit(0);
			}
			//Calculation of correct number of bytes, assign will allocate the data
			int num_bytes = 3*Ncols*Nrows;
			img.assign(Ncols, Nrows);

			//file read in and image data is casted, underneath in the error check gcount counts the num bytes and will compare
			//with what should be the maximum number of bytes
			fin.read((char*)img.data(), num_bytes);
			
			if (fin.gcount() != num_bytes)
			{
				cerr << "Usage: Check number of bytes read." << endl;
				exit(0);
			}

		}
	fin.close();

}

void ppm::write(string s) 
{ 
	ofstream fout;
	//string function replace goes 4 chars in and will put _wmsg in between the image name and .ppm
	s.replace(s.length()-4, 0, "_wmsg"); 

	//Conversion to c-style string
	fout.open(s.c_str());
	
	//If the file fails the program is killed
	if (fout.fail())
	{
		cerr << "Usage: File failed to be written." << endl;
		exit(0);
	}
	//Or else if it doesnt fail and is open...
	else
		if (fout.is_open())
		{
			//title, columns, rows, max value written in with spaces and end lines in their respective places
			fout << title << endl << Ncols << " " << Nrows << endl << max_value << endl;
			
			//max value/title error check, same as in read
			if ((255 != max_value) || (title != "P6"))
			{
				cerr << "Usage: Incorrect header information written." << endl;
				exit(0);
			}
			//Calculation of the number of bytes, written in to the data function 
			int num_bytes = 3*Ncols*Nrows;			
			fout.write((char*)img.data(), num_bytes);
			
		}
	fout.close();
}

void rnumgen::pdf(const vector<int> &v)
{
	F.resize(v.size());
	partial_sum(v.begin(), v.end(), F.begin());
	transform(F.begin(), F.end(), F.begin(), bind2nd(divides<float>(), *(F.end()-1)));
}
int rnumgen::rand() const
{
	const float randnorm = RAND_MAX+1.0f;
	const float p = (float)std::rand()/randnorm;
	return upper_bound(F.begin(), F.end(), p) - F.begin();
}
