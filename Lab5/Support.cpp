/*
	Author: Sam Steinberg
	Date: March 21st, 2018 
*/
#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <fstream>
#include <stack>
using namespace std;

#include "Support.h"
#include "dset.h"

void maze::create_maze(int rows, int cols)
{ 
	
	//Initial resizing of outside vector, square is my grid of size rows*cols. Then, the inside vector must be resized
	//This is the vector of 4 and a bool c, which is set to true per the instructions and that all walls must initially be true
	bool c = true;
	Nrows= rows;
	Ncols = cols;
	square.resize(rows*cols); 
	for (int i = 0; i < (int)square.size(); i++)
	{
		square[i].resize(4,c);
	}

	
	/*
		Adjacency list creation, five ints of cell_p pushed into one and pushed back on adjacency list, so the coords and position are pushed back.
		if statements check the four cases and account for when on the far right column or bottom row 
		
		TBH: I dont know how/why c(i,j,...) works. I was stuck on this for a while then looked on stackoverflow and somebody said you could do this. 
	*/
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if ( (i == rows-1) && (j == cols-1)) //If at last cell do nothing
			{
				continue;
			}
			if ( ((i-1) >= 0) && ((i-1) <= (rows-1)) )
			{
				cell_p c(i, j, i-1, j, 0); //Adjaceny w/cell below it
				adjacent.push_back(c);
			}
			if ( ((j-1) >= 0) && ((j-1) <= (cols-1)) )
			{
				cell_p c(i, j, i, j-1, 1); //Adjaceny w/ cell to left
				adjacent.push_back(c);
			}
			if ( ((i+1) >= 0) && ((i+1) <= (rows-1)) )
			{
				cell_p c(i, j, i+1, j, 2); //Adjaceny w/ cell above
				adjacent.push_back(c);
			}
			if ( ((j+1) >= 0) && ((j+1) <= (cols-1)) )
			{
				cell_p c(i, j, i, j+1, 3); //Adjaceny w/ cell to right
				adjacent.push_back(c);
			}
		}	
	}

	//Randperm for adjacency vector
	for (int i =(int)adjacent.size()-1; i> 0; --i)
	{
		swap( adjacent[i], adjacent[rand() % (i+1)] );
	}
	
	Dset d(rows*cols); //Dset declaration of size square.size() which is rows*cols

	/*
		Since all adjacencies are in their own set to start, we need to merge them into 1...so when the size of the dset is greater than
		1, perform this action.... Set up temporary cell ID's for the cells currently being looked at, and then used the find function
		on them from dset (this will be used under the wall setting...). Set up of walls is due to position, walls are set to their respective number.
		Back to using find, when these ints dont equal each other it means they are not in the same set. Therefore, they must be merged and their 
		positions/walls knocked down (set to false from default true). The erase is explained below (Im sure there was another way to do this 
		but I was stuck and the TA's helped me out).
	*/
	while (d.size() > 1)
	{
		int temp1 = ((adjacent[0].i1)*cols + (adjacent[0].j1));
		int temp2 = ((adjacent[0].i2)*cols + (adjacent[0].j2));
		int wall;

		if (adjacent[0].position == 2)
		{
			wall = 0;
		}
		if (adjacent[0].position == 3)
		{
			wall = 1;
		}
		if (adjacent[0].position == 0)
		{
			wall = 2;
		}
		if (adjacent[0].position == 1)
		{
			wall = 3;
		}

		int co_id = d.find(temp1);
		int ct_id = d.find(temp2);
		if ( co_id != ct_id)
		{
			d.merge(co_id,ct_id);
			square[temp1][adjacent[0].position] = false;
			square[temp2][wall] = false;
		}

		adjacent.erase(adjacent.begin()); //Takes off the first element of adjacency after each iteration to get to the next value
	};
	square[0][0] = false; // entrance to maze
	square[(square.size()-1)][2] = false; //exit of maze
}

void maze::write_maze()
{
	/*Use Nrows and Ncols entered from stdin and traverse through. This allows for easy indicies look-up,
	  with the formula being (i*Ncols+j).	
	*/
	cout << "MAZE" << " " << Nrows << " " << Ncols << endl;
	for (int i = 0; i < Nrows; i++)
	{
		for (int j = 0; j < Ncols; j++)
		{
			if (square[i*Ncols+j][0])
				cout << (i*Ncols+j) << " " << 0 << endl;
		
			if (square[i*Ncols+j][1])
				cout << (i*Ncols+j) << " " << 1 << endl;
			
			if (square[i*Ncols+j][2])
				cout << (i*Ncols+j) << " " << 2 << endl;
			
			if (square[i*Ncols+j][3])
				cout << (i*Ncols+j) << " " << 3 << endl;
		}
	}
}
/*
	Reading of maze, first the title and number of rows and cols are read in. Then, resizing occurs and the values set to false this time.
	This essentially reads in a blank grid, which I then fill in when I read in the cell and walls it has (by setting to true).
*/
void maze::read_maze()
{
	string title;
	int cell, wall, rows, cols;
	bool c = false;	

	cin >> title >> rows >> cols;

	Nrows = rows;
	Ncols = cols;
	
	square.resize(Nrows*Ncols);
	for (int i = 0; i < (int)square.size(); i++)
	{
		square[i].resize(4,c);
	}
	
	while (cin >> cell >> wall)
	{
		square[cell][wall] = true;
	}
	

}
// One version of whats called in main, source to sink are the first and last cells
bool maze::solve_maze()
{
	int source = 0;
	int sink = (Nrows*Ncols)-1;
	solve_maze(source,sink);
}
//Determines if you are at a dead end or not by checking each wall and is its been visited or not
bool maze::wrong_way(vector<vcolor_t> &vcolor, int cell)
{

	if (square[cell][0] == false && vcolor[cell-Ncols] != BLACK)
		return false;
	if (square[cell][1] == false && vcolor[cell-1] != BLACK)
		return false;
	if (square[cell][2] == false && vcolor[cell+Ncols] != BLACK)
		return false;
	if (square[cell][3] == false && vcolor[cell+1] != BLACK)
		return false;

	else
		return true;

}

/*
	DFS stack-based implementation. All cells are intially set to be WHITE and unvisited, and the source is then pushed onto the stack.
	After this, when the source and sink arent the same, a temp int is set to be the top element on the stack. If it isnt BLACK/visited
	then pop it off and color it. Then push the cell value onto the path. The if statements's will determine the rest of the path.
	If you hit a dead end, the wrong_way function will call and the path will pop_back to try a new direction. After all this, add the sink.
*/
bool maze::solve_maze(int source, int sink)
{
	vcolor.assign(square.size(), WHITE);
	stack<int> S;

	S.push(source);

	if ( source < 0 || source > ((Nrows*Ncols)-1))
	{
		cerr << "Source out of bounds" << endl;
		exit(0);
	}
	if ( sink < 0 || sink > ((Nrows*Ncols)-1))
	{
		cerr << "Sink out of bounds" << endl;
		exit(0);
	}
	if (source > sink)
	{
		cerr << "Check sink" << endl;
		exit(0);
	}

	while (S.top() != sink)
	{
		int i = S.top();
		if (vcolor[i] == BLACK)
			continue;

		S.pop();
		vcolor[i] = BLACK;
		vpath.push_back(i);

		if (square[i][0] == false && vcolor[i-Ncols] != BLACK)
			S.push(i-Ncols);
		if (square[i][1] == false && vcolor[i-1] != BLACK)
			S.push(i-1);
		if (square[i][2] == false && vcolor[i+Ncols] != BLACK)
			S.push(i+Ncols);
		if (square[i][3] == false && vcolor[i+1] != BLACK)
			S.push(i+1);

		else
		{
			while ( wrong_way(vcolor, vpath.back()) )
			{
				vpath.pop_back();
			}
		}
	}
	vpath.push_back(sink);
}
//Path written in required fashion
void maze::write_path()
{
	cout << "PATH " << Nrows << " " <<  Ncols << endl;
	for (int i = 0; i < vpath.size(); i++)
	{
		cout << vpath[i] << endl;
	}
}

