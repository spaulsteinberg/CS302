
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

#include "Support.h"

int main(int argc, char *argv[]) {
  srand(time(NULL));

  if (argc != 3) {
    cerr << "usage: " << argv[0]
		 << " nrows ncols\n";
	return 0;
  }

  maze M;

  int Nrows = atoi(argv[1]);
  int Ncols = atoi(argv[2]);

  M.create_maze(Nrows, Ncols);
  M.write_maze();
}
