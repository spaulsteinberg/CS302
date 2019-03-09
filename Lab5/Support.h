/*
	Author: Sam Steinberg
	Date: March 21st, 2018 
*/

#ifndef __SUPPORT_H_
#define __SUPPORT_H_

#include <iostream>
#include <vector>
using namespace std;
//This struct represents a (x,y) and (x2,y2) along with a position in the grid
//Set to passed value in Support.cpp
struct cell_p
{
	int i1, j1, i2, j2, position;

	cell_p(int x1, int y1, int x2, int y2, int index)
	{
			i1 = x1;
			j1 = y1;
			i2 = x2;
			j2 = y2;
			position = index;
	}
};
//maze class with members, adjacent vector stores adjacent interior cells  while square is the grid with walls to be knocked down (done in cpp)
//color, path, and dead end's for mazesolve also here
class maze
{
		public:
		void create_maze(int, int);
		void write_maze();
		
		bool solve_maze(int, int);
		bool solve_maze();
		void read_maze(); 
		void write_path();
		

		vector<cell_p> adjacent; 
		int Nrows, Ncols;
		vector < vector<int>  > square;

		typedef enum { WHITE, BLACK } vcolor_t;
		vector<vcolor_t> vcolor;
		vector<int> vpath;
		bool wrong_way(vector<vcolor_t> &, int); 
};

#endif
