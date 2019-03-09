/*
	Author: Sam Steinberg
	Date: February 26th, 2018
*/
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <utility>
#include <stack>
#include <queue>
#include <map>
#include <iterator>
using namespace std;

//Setter and getter function in city allow access to value of private variables when called
class city
{
	public:
		string get_name() { return name; }
		string get_type() { return type; }
		int get_zone() { return zone; }
		int get_population() { return population; }
		float get_latitude() { return latitude; }
		float get_longitude() { return longitude; }
		void set_name(string n) { name = n; }
		void set_type(string t) { type = t; }
		void set_zone(int z) { zone = z; }
		void set_population(int p) { population = p; }
		void set_latitude(float l) { latitude = l; }
		void set_longitude(float ltude) { longitude = ltude; }
		friend ostream& operator<<(ostream&, city&);
		friend istream& operator>>(istream &, city&);
	private: 
		
		string name, type;
		int zone, population;
		float latitude, longitude;
};

//Overloaded operator for writing, fixed with required spacing
ostream& operator<<(ostream &out, city &c)
{
	out << " " << setw(20) << left << c.name;
	out << setw(12) << c.type;
	out << setw(2) << c.zone;
	out << setw(10) << right << c.population << setw(8);
	out<< setprecision(2) << fixed << c.latitude << setw(8) << c.longitude << endl;
	return out;
}
//Overloaded operator for reading, decomposes the read in line in order(used in read)
istream& operator>>(istream &in, city &c)
{
	in >> c.zone >> c.name >> c.type;
	in >> c.latitude >> c.longitude >> c.population;
	return in;
}
/*
	Opens a text file named "citylist" where the city info for the lab is stored. If the file opens, I created a flag initialized to false,
	but is changed to true when a line begins with a # or is empty. After this, a stringstream reads the city (istream overloaded handles this)
	and if the flag is still false then the city is pushed back onto a vector of cities.
*/
void read_cityinfo(vector<city> &r_city, map<string, int> &m)
{
	stringstream ss;
	ifstream fin;
	fin.open("citylist.txt");
	if (fin.fail())
	{
		cerr << "File failed to be read." << endl;
		exit(0);
	}
	city c;
	string line;
	int count = 0;
	bool flag = false;
	while (getline(fin, line))
	{
		flag = false;
		ss.str(line);
		if (line[0] == '#' || line.empty())
		{
			flag = true;
		}
		/*
		ss >> c;

		if (flag == false)
		{
			r_city.push_back(c);
		}
		ss.clear();
		*/

		if (flag == false)
		{
			ss >> c;
			r_city.push_back(c);
			m[r_city[count].get_name()] = count;
			count++;
		}
		ss.clear();
	}
	fin.close();
}
/*
	Writes city info to a text file "cityinfo.txt". The passed vector is populated by read, and a counter is outputted next to each city written in 
	the function (overloaded ostream operator) as is required. 
*/
void write_cityinfo(vector<city> &w_city)
{
	ofstream fout;
	fout.open("cityinfo.txt");
	if (fout.fail())
	{
		cerr << "File failed to be written." << endl;
		exit(0);
	}
	int count = 0;
	fout << "CITY INFO (N=" << w_city.size() << "):" << endl << endl;
	for (unsigned int i = 0; i < w_city.size(); i++)
	{
		fout << ' ' << right << setw(2) << i;
		fout << w_city[i];
		
	}

	fout.close();
}
/*
	Class dtable uses a buf that creates a table of size (N*(N+1))/2), with N being the size of the vector of cities being passed to it.
	The constructor populates this vector after its resized. Deconstructor resizes vector to 0.
*/
class dtable
{
	public:
		dtable(vector<city> &);
		~dtable() { buf.resize(0); } 
		//int * operator[] (int i) { return &buf[(i*(i+1))/2]; }
		int operator() (int , int );
		void populate(vector<city> &);
	private:
		int N;
		vector<float> buf;
};
dtable::dtable(vector<city> &c)
{
	N = c.size();
	buf.resize((N*(N+1))/2);
	populate(c);
}
/*
	Populate class swaps i and j if j is greater than i to exploit distance symmetry. After this and running through the rows and cols,
	longitude and latitude are converted to radians and the central angle is calculated using the haversine formula, since d= radius*central angle.
	After being multiplied by the radius of Earth we get the unrounded distance, which is then rounded. The distance is then added at its respective
	position in the table.
 */
void dtable::populate(vector<city> &v)
{
	float distance;
	int rounded_distance;
	float central_ang, x1, x2, y1, y2;

	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j <= i; j++)
		{
			if ( i < j )
			{
				swap(i,j);
			}
			x1 = v[i].get_latitude() *(M_PI/180);
			x2 = v[j].get_latitude() *(M_PI/180);
			y1 = v[i].get_longitude() *(M_PI/180);
			y2 = v[j].get_longitude() *(M_PI/180);			
			central_ang = 2.0*asin(sqrt(pow((sin((x2-x1)/2)) , 2) + cos(x1) * cos(x2) * pow(sin((y2-y1)/2) , 2)));
			distance = 3982*central_ang;
			rounded_distance = 5.0*round(distance / 5.0);
			buf[((i*(i+1))/2) + j] = rounded_distance;
		}
	}
}
/*
	Overloaded function operator again checks i and j, then returns the distance at the position in the table.
*/
int dtable::operator()(int i, int j)
{
	if (i < j)
	{
		swap(i,j);
	}
	return buf[((i*(i+1))/2) + j];
}

/*
	This function writes to a file "citydtable", where its looped and and written accordingly. To add the space after each iteration besides the first	  when i !=0 there is an added space. dtable computed distance at position (i,j).
 */
void write_citydtable(vector<city> &d_city, int max, dtable &d)
{
	ofstream fout;
	fout.open("citydtable.txt");
	if (fout.fail())
	{
		cerr << "File failed to be written." << endl;
		exit(0);
	}
	fout << "DISTANCE TABLE:" << endl << endl;	
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < i; j++)
		{
			fout << "" << setw(3) << setfill(' ') << i<< " " << setw(max) << setfill('.') << left <<  d_city[i].get_name() + " to " + d_city[j].get_name() + " ";
			fout << setw(5) << setfill(' ') << right << d(i,j) << " miles"  << endl;
		}
		if (i !=0 )
		fout << endl;
	}
	fout.close();
}
/*
	Resize vector of vectors to the size of the city vector to allocate enough space. The first nested for loop will take care of calculations for
	Regional-to-Regional and Gateway-to-Gateway when in the same zone. Then, the closest regional-to-gateway when in the same zone is calculated.
	The minimum distance and index must be reset above the second for loop so they are reset for each city iteration. The result is pushed back
	after the second for loop but within the first so the final distance is pushed back for EACH i.

	Second for loop explained below.
*/
void create_citygraph(vector<city> &g_city, vector< vector<int> > &E, dtable &d) 
{
	
	E.resize(g_city.size());
	int min_distance = INT_MAX;
	int min_index;

		for(int i = 0; i < g_city.size(); i++)
		{
			min_distance = INT_MAX;
			min_index = -1;
			for (int j = 0; j < g_city.size(); j++)
			{		
				if ( i == j)
					continue;
								
				if (  (g_city[i].get_zone() == g_city[j].get_zone()) )
				{
					if ( g_city[i].get_type() == g_city[j].get_type() )
					{
							E[i].push_back(j);
							E[j].push_back(i);
					}
						
				
					if ( g_city[i].get_type() == "REGIONAL" && g_city[j].get_type() == "GATEWAY")
					{
						if(d(i,j) < min_distance)
						{
							min_distance = d(i,j);
							min_index = j;
						}
					}
 
				
				}
				
			}	
			if (min_index != -1){
			E[i].push_back(min_index);
			E[min_index].push_back(i);
			}
		}
		
		/*
			Here, the cities are traversed and when it is a gateway, a loop for 1-6 is enabled to allow for the closest gateway in the other
			zones to be calculated. If the cities being compared are in the same zone, it is skipped with 'continue'.
			A third for loop will then traverse the cities to see if they are a GATEWAY and in the same zone as the second for loop,
			and if so a minimum distance in calculated. Once the shortest distance is found in each zone the index is saved and
			outside of the third for loop but within the 'zone' for loop the distance is checked to be within 6000 miles and if so,
			is pushed back onto the vector of vectors. This repeats for each zone due to the second for loop.
		*/
		for (int i = 0; i < g_city.size(); i++)
		{
			if (g_city[i].get_type() == "GATEWAY")
			{
				for (int zo = 0; zo <= 6; zo++)
				{
					min_distance = INT_MAX;
					min_index = -1;
					
					if (g_city[i].get_zone() == zo)
						continue;
				
					for (int j = 0; j < g_city.size(); j++)
					{
						if ((g_city[j].get_type() == "GATEWAY") && (g_city[j].get_zone() == zo))
						{
							if ( d(i,j) < min_distance)
							{
								min_distance = d(i,j);
								min_index = j;
							}	
						}
					}
					if ((min_index != -1) && (d(i,min_index) <= 6000))
					{
						E[i].push_back(min_index);
						E[min_index].push_back(i);
					}
				}
			}
		}


		
		//This gets rid of all repetitions after sorting through vector of vectors
		for (int i = 0; i < E.size(); i++)
		{	
			sort(E[i].begin(), E[i].end());
			E[i].erase( unique(E[i].begin(), E[i].end()), E[i].end());
		}	

}

/*
 Output of City graph, a file is opened called "citygraph.txt" and the required information calculated from create_citygraph 
 is outputted according to the requirements.
*/
void write_citygraph( vector<city> &w, vector< vector<int> > &A, dtable &d)
{
	ofstream fout;
	fout.open("citygraph.txt");
	if (fout.fail())
	{
		cerr << "Graph failed to be written." << endl;
		exit(0);
	}
	fout << "CITY GRAPH:" << endl << endl;
	for (int i = 0; i < A.size(); i++)
	{
		fout  << setw(3) << setfill(' ') <<  i << " " << w[i].get_name() << endl;
		for (int j = 0; j < A[i].size(); j++)
		{
			fout << setw(6) << setfill(' ') <<  right <<  A[i][j]  << " " << w[A[i][j]].get_name() << ": " << d(i,A[i][j]) << " miles" << endl;
		}
		if (i !=29)
		fout << endl;
	}

}
//Calculates path distances...I named it shortest_path before I realized its not necessarily the shortest one
int shortest_path(vector<int> &path, dtable &d)
{
	int distance = 0;

	for (int i = 1; i < path.size(); i++)
	{
		int j = i - 1;
		distance = distance + d(path[i], path[j]);
	}

	return distance;
}
/*
  BFS function contains vectors: path, link, and distance to keep track of distances, link lengths, and the penultimate path.
  link and dist are initialized to the size of the city vector and assigned a default values of -1 and INT_MAX respectively.
  The source city is pushed onto the queue and as long as it isnt the sink a distance is calculated and a link is made and it is pushed back onto the queue.
  After this sequence the path is created using a stack. Formatted output is below and is designed according to whether the user gives the show option.
*/
void bfs_route(int source, int sink, vector<city> &b_city, dtable &d,  vector < vector<int> > &E, bool option)
{
	int p_dist = 0;

	vector<int> path;
	vector<int> link;
	vector<int> dist;

	link.assign(b_city.size(), -1);
	dist.assign(b_city.size(), INT_MAX);

	dist[source] = 0;
	link[source] = source;

	queue<int> Q;
	Q.push(source);

	while (!(Q.empty()))
	{
		int i= Q.front();
		Q.pop();
		if (i == sink)
		{
			break;
		}
		for (int k = 0; k < E[i].size(); k++)
		{
			int j = E[i][k];
			if (dist[j] == INT_MAX)
			{
				dist[j] = dist[i] + 1;
				link[j] = i;
				Q.push(j);
			}
		}
	}

	while(!(Q.empty()))
	{
		Q.pop();
	}

	if (dist[sink] == INT_MAX)
	{
		cout << "There is no route from " << b_city[source].get_name() << " to " << b_city[sink].get_name() << endl;
		return;
	}

	stack<int> S;

	for (int i = sink; i != source; i = link[i])
	{
		S.push(i);
	}

	S.push(source);

	while (!(S.empty()))
	{
		int i = S.top();
		S.pop();
		path.push_back(i);
	}

	if (option)
	{
		cout << " " << right << setw(7) << p_dist << " miles : " << right << " " << path[0] << " " << b_city[path[0]].get_name();

		for (int i = 1; i < path.size(); i++)
		{
			p_dist = p_dist + d(path[i], path[i-1]);
			cout << endl;
			cout << " " << right << setw(7) << p_dist << " miles : " << right << setw(2) << right << path[i] << " " << setw(18) << left << b_city[path[i]].get_name();
			cout << setw(10) << right << d(path[i], path[i-1]) << right <<  " miles";
		}
		cout << endl << endl;
	}
	else
	{
		p_dist = shortest_path(path, d);
		cout << source << " " << b_city[source].get_name() << " to  " << sink << " " << b_city[sink].get_name();
		cout << " : " << p_dist << " miles" << endl << endl;
	}
}	
/*
	Dijkstra route contains path, link, dist, and a color value of WHITE or BLACK, being initialized to WHITE.
	The while loop will run through and as long as the color index hasnt been visited and the distance at that city is less than the current min, the indexes are recorded
	and the min distance changed. After the nodes have been visited they are switched to BLACK, and a similar loop will take place again for the visited nodes. The path is created with the stack,
	and the output is the same as BFS route, determined by the given option.
*/
void dijkstra_route(int source, int sink, vector<city> &d_city, dtable &d, vector< vector<int> > E, bool option)
{
	int p_dist = 0;

	vector<int> path;
	vector<int> link;
	vector<int> dist;
	vector<int> color;

	typedef enum { WHITE, BLACK } color_t;

	color.assign(d_city.size(), WHITE);
	link.assign(d_city.size(), -1);
	link[source] = source;
	dist.assign(d_city.size(), INT_MAX);
	dist[source] = 0;

	dist[source] = 0;
	link[source] = source;

	while(1)
	{
		int len;
		int min_in = -1;
		int min_dist = INT_MAX;
		for (int i = 0; i < (int)color.size(); i++)
		{
			if ((color[i] == WHITE) && (min_dist > dist[i]))
			{
				min_in = i;
				min_dist = dist[i];
			}
		}
		if ((len = min_in) == -1)
			return;

		color[len] = BLACK;
		if (len == sink)
			break;

		for (int j = 0; j < E[len].size(); j++)
		{
			int k = E[len][j];
			int Wij = d(len,k);
			
			if (color[k] == WHITE)
			{
				if (dist[k] > (dist[len] + Wij))
				{
					dist[k] = dist[len] + Wij;
					link[k] = len;
				}
			}
		}
	}

	stack<int> S;

	for (int i = sink; i != source; i = link[i])
	{
		S.push(i);
	}

	S.push(source);
	
	while (!(S.empty()))
	{
		int top = S.top();
		S.pop();
		path.push_back(top);
	}

	if (option)
	{
		cout << " " << right << setw(7) << p_dist << " miles : " << right << setw(2) << right << path[0] << " " << d_city[path[0]].get_name();

		for (int i = 1; i < path.size(); i++)
		{
			p_dist = p_dist + d(path[i], path[i-1]);
			cout << endl;
			cout << " " << right << setw(7) << p_dist << " miles : " << right << setw(2) << right << path[i] << " " << setw(18) << left << d_city[path[i]].get_name();
			cout << setw(10) << right << d(path[i], path[i-1]) << right <<  " miles";
		}
		cout << endl << endl;
	}
	else
	{
		p_dist = shortest_path(path, d);
		cout << source << " " << d_city[source].get_name() << " to  " << sink << " " << d_city[sink].get_name();
		cout << " : " << p_dist << " miles" << endl << endl;
	}

}


int main(int argc, char *argv[])
{
  //Declarations and objects needed for lab
  city c;
  vector<city> m_city;
  int field;
  vector< vector<int> > adj;
  map<string, int> c_map;
  map<string, int>::iterator upper, prev;

  //Error checks to see if more than 3 commands are given, or the first arg is not a proper command
  if (string(argv[1]) != "-write_info" && string(argv[1]) != "-write_dtable" && string(argv[1]) != "-write_graph" && string(argv[1]) != "-mode_bfs" && string(argv[1]) != "-mode_dijkstra")
	  cerr << "Error. Proper command needed." << endl;

  if (argc > 3)
	  cerr << "Check number of arguments." << endl;
  
  
 read_cityinfo(m_city, c_map); //City info read, needed for whole lab


  if (string(argv[1]) == "-write_info")
  {
		write_cityinfo(m_city);
  }
  
  //Calculation for the name field, 2*(max city chars)+4
  for (unsigned int i = 0; i < m_city.size(); i++)
  {
		unsigned int max = 0;
		string temp = m_city[i].get_name();
		if ( temp.length() > max)
		{
			max = temp.length();
		}
		field = (2*max) + 4;
  }


  dtable d(m_city); //Table of distances calculated, needed for parts 2-4

  if (string(argv[1]) == "-write_dtable")
	{
		write_citydtable(m_city, field, d);
	}

  if (string(argv[1]) == "-write_graph")
  {
	    create_citygraph(m_city, adj, d);
		write_citygraph(m_city, adj, d);
  }
  
  bool s_option = false;
  string y, c_initial, c_dest;
  
  //If a 3rd command is given, it must be "-show", or else the program gives error message. "-show" if true will pass to part 4 and trigger it
  if (argc == 3)
  {
	  y = string(argv[2]);
	  if (y == "-show")
		  s_option = true;
	  else
	  {
		  cerr << "Proper command needed (-show)." << endl;
		  return 0;
	  }
  }
  
  
  if ((string(argv[1]) == "-mode_bfs") || (string(argv[1]) == "-mode_dijkstra"))
  {
	  create_citygraph(m_city,adj,d);
	  cout << "Enter> ";
	  while (cin >> c_initial >> c_dest)
	  {
		  //temporary flags to denote the city inputted is an exact city in list, temporary ints to hold indexes
		int i_index = -1;
		int d_index = -1;
		 bool flag_i = true;
		 bool flag_d = true;
		 int temp_i, temp_d;
		 for (int i = 0; i < m_city.size(); i++)
		 {
				if (c_initial == m_city[i].get_name())
				{
					temp_i = i;
					flag_i = false;
					i_index = temp_i;
				}
				if (c_dest == m_city[i].get_name())
				{
					temp_d = i;
					flag_d = false;
					d_index = temp_d;
				}

			
		}
		 

		 //If the cities arent entered in complete form this does the upper bound calcs
		 if (flag_i && flag_d)
		 {
			upper = c_map.upper_bound(c_initial);

			 if (upper != c_map.end())
				{
					i_index = upper -> second;
				}
			

		  upper = c_map.upper_bound(c_dest);

			if (upper != c_map.end())
				{
					d_index = upper -> second;
				}

		 }

		 
		  //If the city is valid, the value will be greater than -1, triggering the call for the bfs/dijkstra functions
		  if ((i_index > -1) && (d_index > -1))
		  {
				if ( string(argv[1]) == "-mode_bfs")
					bfs_route(i_index, d_index, m_city, d, adj, s_option);
				if (string(argv[1]) == "-mode_dijkstra")
					dijkstra_route(i_index, d_index, m_city, d, adj, s_option);
		  }
		  
		  //flags are not hit and indexes arent -1, the user is alerted
		  else
		  {
				if (flag_i == true)
					cout << c_initial << " unknown." << endl;
				if (flag_d == true)
					cout << c_dest << " unknown." << endl;
		  }

		  cout << "Enter> ";
	  }
  }

}
