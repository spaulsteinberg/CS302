/*
	Author: Sam Steinberg
	Date: February 13th, 2018 
*/

#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <iostream>
#include <vector>
typedef unsigned char uchar;
extern const char ETX;

template <typename T>
//buf data pointer set to null, getter functions return number of rows and cols
//assign function allocates memory for the rows and cols while data() returns a pointer to the matrix buffer
class matrix
{
	public:
		matrix()
		{
			buf = NULL;
		}
		~matrix(){delete [] buf;}
		int get_Nrows() const {return Nrows;}
		int get_Ncols() const {return Ncols;}
		T * operator[] (int i) {return &buf[i*Ncols];}
		void assign(int n_Ncols, int n_Nrows)
		{
			Ncols = n_Ncols;
			Nrows = n_Nrows;
			buf = new T [Nrows*Ncols];
		}
		T * data() { return buf; }

	private:
		int Nrows, Ncols;
		T *buf;
};
//Pixel stores row and column indicies that must be initialized and able to take new values
struct pixel 
{ 
	int row, col;
	pixel ()
	{
		row = 0;
		col = 0;
	}
	pixel(int r, int c)
	{
		row = r;
		col = c;
	}
	
};
//RGB stores three chars that must be unsigned since there cannot be negative color
struct RGB
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	RGB()
	{
		R = 0;
		G = 0;
		B = 0;
	}
	
};
//creation of template-based matrix, operator[] returns a row address. Getter functions returns number of rows and cols for
//export
class ppm
{
	public:
		ppm(){ Nrows = 0; Ncols = 0;  max_value = 0; title = " ";}
		void read(string);
		void write(string);
		RGB * operator[](int row){ return img[row];}
		int get_Nrows(){return Nrows;}
		int get_Ncols(){return Ncols;}
	private:
		int Nrows, Ncols, max_value;
		string title;
		matrix<RGB> img;

};
//Code from rnumgen handout, class defintions and setting the seed to 0
class rnumgen
{
	public:
		rnumgen(int seed = 0) {srand(seed);}
		void pdf(const std::vector<int> &);
		int rand() const;
	private:
		std::vector<float> F;
};

#endif
