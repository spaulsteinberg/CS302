/*
	Author: Samuel Steinberg
	Date: April 3rd, 2018 
*/
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <climits>
#include <stack>
#include <queue>
using namespace std;
/*
	Matrix class to create a grid of sorts. operator[] allows for 2D look-up, assign function 
	written to allocate size for buffer.
*/
template<typename T>
class matrix 
{
	public:
		matrix()
		{
			buf = NULL;
		}
		~matrix()
		{
			delete[] buf;
		}

		int get_Nrows() const { return Nrows; }
		int get_Ncols() const { return Ncols; }
		T* operator[] (int i) { return &buf[i*Ncols]; }
		void assign(int n_Nrows, int n_Ncols)
		{
			Nrows = n_Nrows;
			Ncols = n_Ncols;
			buf = new T [Nrows*Ncols];
		}
	private:
		int Nrows, Ncols;
		T* buf;
};
/*
	LCS class and member functions to compute the alignment and report differences. text1_push_back and text2_push_back will populate string
	vectors, getter functions for returning these are included as well (needed in main). Insert and delete costs are 1, a match (denoted CHA)
	will either return a cost of 0 or INT_MAX. Delete/Insert/Match moves are given a constant value so that the move can be recorded and 
	easily looked-up/compared. Compute alignment and report difference explained below.
*/
class LCS {
  public:
    void text1_push_back(string h) { text1.push_back(h); }
    void text2_push_back(string g) { text2.push_back(g); }
	void compute_alignment(vector<string>&, vector<string>&);
	void report_difference();
	vector<string> get_text1() { return text1; }
	vector<string> get_text2() { return text2; }
	int CHAcost(string, string); 
	int DELcost(string s) { return 1; }
	int INScost(string s) { return 1; }
  private: 
	void report_difference(int,int);
    vector<string> text1;
    vector<string> text2;
	stack<int> trace;
	matrix<int> cost;
	matrix<int> link;
	int m,n;
	const int DELmove = 1;
	const int INSmove = 2;
	const int CHAmove = 4;
};

int LCS::CHAcost(string s1, string s2)
{
	if (s1 == s2)
	{
		return 0;
	}
	else
		return INT_MAX;
}
/*
	compute_alignment builds cost and link matricies...explained more in code.
*/
void LCS::compute_alignment(vector<string> &x, vector<string> &y)
{
		//Vector sizes needed to make matrix size
		m = x.size();
		n = y.size();

		//This allows for easier look-up
		x.insert(x.begin(), " ");
		y.insert(y.begin(), " ");

		//Cost and link matrix given a size
		cost.assign(m+1, n+1);
		link.assign(m+1, n+1);

		//Upper-left most cell must be 0, this will also act as a base case for the recursion
		cost[0][0] = 0;
		link[0][0] = 0;

		//Below two for-loops set the values for the left and upper walls, must be done starting at row/col 1 instead of 0 due to (0,0)
		//Needing to be 0. 
		for (int i = 1; i <= m; i++)
		{
			cost[i][0] = cost[i-1][0] + DELcost(x[i]);
			link[i][0] = DELmove;
		}
		for (int j = 1; j <= n; j++)
		{
			cost[0][j] = cost[0][j-1] + INScost(y[j]);
			link[0][j] = INSmove;
		}

		//Computations for rest of matrix, first check for change, then insert and then delete. The matrixes are updated for each iteration
		for (int i = 1; i <= m; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				if (CHAcost(x[i],y[j]) == 0)
				{
					cost[i][j] = cost[i-1][j-1];
					link[i][j] = CHAmove;
				}
				else
				{
					cost[i][j] = CHAcost(x[i],y[j]);
				}
				
				int inscost = cost[i][j-1] + INScost(y[j]);
				if (inscost < cost[i][j])
				{
					cost[i][j] = inscost;
					link[i][j] = INSmove;
				}

				int delcost = cost[i-1][j] + DELcost(x[i]);
				if (delcost < cost[i][j])
				{
					cost[i][j] = delcost;
					link[i][j] = DELmove;
				}
			}
		}
	/*	
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << link[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl << endl;

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << cost[i][j] << " ";
			}
			cout << endl;
		}*/

}
/*
	Private report_difference builds the trace with a stack (backtracing). Base case is when both i and j are 0 INDEXES, signaling that
	we are back at the top left. Otherwise, the link[i][j] move is compared and the trace builds accordingly.
*/
void LCS::report_difference(int i, int j)
{

	if ( (i == 0) && (j == 0) )
		return;		
			
	else if (link[i][j] == CHAmove)
				{
					trace.push(CHAmove);
					report_difference(i-1,j-1);
				}	
	else if (link[i][j] == DELmove)
				{
					trace.push(DELmove);
					report_difference(i-1,j);
				}
	else if (link[i][j] == INSmove)
				{
					trace.push(INSmove);
					report_difference(i, j-1);
				}
			
}
//Easier to explain inside the code
void LCS::report_difference()
{
	int i = m;
	int j = n;
	int file1_line = 0;
	int file2_line = 0;
	int Icount = 0, Dcount = 0;
	report_difference(i,j);


	while (!(trace.empty()))
	{
		//k holds the value in the stack being compared...then pop the stack 
		int k = trace.top();
		trace.pop();

		//FOR BELOW 3 IF's: Depending on the move, a sub will increment both line numbers and Icount and Dcount will hold the number
		//of insertions and deletions
		if (k == CHAmove)
		{
			file1_line++;
			file2_line++;
		}
		
		if (k == INSmove)
		{
			Icount = 1;
		}
		if (k == DELmove)
		{
			Dcount = 1;
		}
		//Again, counts insertions and deletions and excludes elements that are a match, since diff doesnt
		//need to do anything
		while (!trace.empty() && trace.top() != CHAmove)
		{
			if (trace.top() == INSmove)
				Icount++;
			if (trace.top() == DELmove)
				Dcount++;

			trace.pop();
		}
		/*When there is a deletion (The count is greater than 0) If there is only one it will simply output the line along with the Dcount 
		  which will be 1 in this case. Otherwise, it will be this with a comma that denothes the line number plus the number of lines
		  that need to be deleted. The insertions follow the same process below. I cant really say why, but then again the people who designed
		  diff6 are a lot smarter than me. And funnier. And better-looking.
		*/
		if (Dcount > 0)
		{
			if (Dcount == 1)
				cout << file1_line + Dcount;
			else
				cout << file1_line+1 << "," << file1_line+Dcount;

			if (Icount > 0)
			{
				cout << "c";
				if (Icount == 1)
				{
					cout << file2_line+Icount;
				}
				else
				{
					cout << file2_line+1 << "," << file2_line+Icount;
				}
			}
			else
			{
				cout << "d" << file2_line;
			}
			cout << endl;
		}
		else if(Icount > 0)
		{
			cout << file1_line << "a";
			if (Icount == 1)
			{
				cout << file2_line+Icount;
			}
			else
			{
				cout << file2_line+1 << "," << file2_line + Icount;
			}
			cout << endl;
		}
		
		//These temp variables are needed so that proper iteration can occur. That is, so the line numbers are added smoothly.
		int temp1 = file1_line;
		int temp2 = file2_line;
		int d = Dcount;
		int i = Icount;

		/*Outputting, in the deletion text1 is being deleted and the text1 line number is incremented. This occurs until there are no 
		  more deletion operations to occur.
		*/
		if (Dcount > 0)
		{
			while (Dcount != 0)
			{
				cout << "< " << text1[file1_line] << endl;
				file1_line++;
				Dcount--;
			}
			//Dashes denote a change of the file being written on
			if (Icount > 0)
			{
				cout << "---" << endl;
			}
		}
		//Outputting in insertion text2 being affected and its line number being incremented.
		if (Icount > 0)
		{
			while (Icount != 0)
			{
				cout << "> " << text2[file2_line] << endl;
				file2_line++;
				Icount--;
			}
		}

		//New iteration of the line number using variables that held the original values before incs/decs in output
		file1_line = temp1 + d;
		file2_line = temp2 + i;
	}
	
}

/*
	Main function retrieves the data from the text files, using the custom _push_back function to populate private members in LCS class.
	Two vectors of strings are then set equal to the private vectors through the getter functions and compute_alignment can be called.
	Report difference will then do the rest of the lifting.
*/
int main(int argc, char *argv[])
{
	//Must be 3 args 
  if (argc != 3)
  {
	  cerr << "Check number of arguments" << endl;
	  exit(0);
  }

  LCS lcs;
  ifstream fin1, fin2;
  string line1,line2;
  vector<string> temp1;
  vector<string> temp2;

//File stream for first text, uses _push_back to populate vector text1 in private
  fin1.open(argv[1]);
  while (getline(fin1, line1))
  {
		lcs.text1_push_back(line1);
  }
  fin1.close();
//File stream for second text, uses _push_back to populate text2 in private
  fin2.open(argv[2]);
  while (getline(fin2,line2))
  {
		lcs.text2_push_back(line2);
  }
  fin2.close();
//Getter functions used to populate temporary vectors that are passed to compute_alignment
  temp1 = lcs.get_text1();
  temp2 = lcs.get_text2();
//calls to compute alignment to create cost/link matrix and report difference
  lcs.compute_alignment(temp1,temp2);
  lcs.report_difference();
}
