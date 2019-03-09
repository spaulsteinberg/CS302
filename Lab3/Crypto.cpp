/*
	Author: Sam Steinberg
	Date: February 16th, 2018 
*/
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <istream>
#include <cstdlib>
#include <bitset>
using namespace std;

#include "support.h"

void set_pixel_list(ppm &image, vector<pixel> &coord)
{
	int x, y, color;
	//Declaration and resizing of histogram, setting the values to 0
	vector<int> histogram;
	histogram.resize(4096, 0);
	//push all indicies onto coord vector of pixels
	for (int i = 0; i < image.get_Nrows(); i++)
	{
		for (int j = 0; j < image.get_Ncols(); j++)
		{
			coord.push_back(pixel(i,j));
		}
	}

	for (unsigned int l = 0; l < coord.size(); l++)
	{
		//Retrieval of row and col at indicies determined by for loop
		x = coord.at(l).row;
		y = coord.at(l).col;
		//Take 4 MSB's of colors to make a 12 bit int
		color = ((((image[x][y].R) >> 4) << 8) | (((image[x][y].G) >> 4) << 4) | ((image[x][y].B) >> 4));
		//increment histogram at 12 bit int produced above
		histogram[color]++;
	}
	//set rnumgen seed to 0, generate histogram with pdf function
	rnumgen RNG(0);
	RNG.pdf(histogram);
	int temp;

	for (unsigned int k = (coord.size()-1); k > 0; k--)
	{
		temp = (RNG.rand() << 12 | RNG.rand()); //Generation of 24 bit number that allows much greater processing 
		swap(coord[k], coord[temp%(k+1)]); //swap values, mod to make sure stays in bounds
	}
	
}

void encode(ppm &image) 
{
	vector<pixel> v;
    set_pixel_list(image, v);
	char c;
	unsigned char temp;
	unsigned int count = 0;
	unsigned int x,y;
	//Characters extracted from cin one-by-one
	while (cin.get(c))
	{
		//For each bit...
		for (int k = 0; k < 8; k++)
		{
		
			
			//Retrieval of coordinates 
			x = v.at(count).row;
			y = v.at(count).col;

			//Temp variable is used to store the shifted bits of the character without actually altering it
			temp = c >> k;
			
			/*Switch statement alternates each color, and depending on the count.
			   Inside, Replace LSB of target pixel color with extracted character bit.
			*/
			switch(count % 3)
			{
				case 0:
					image[x][y].R &= 0xFE;
					image[x][y].R |= (temp & 0x1);
					break;
				case 1:
					image[x][y].G &= 0xFE;
					image[x][y].G |= (temp & 0x1);
					break;
				case 2:
					image[x][y].B &= 0xFE;
					image[x][y].B |= (temp & 0x1);
					break;
			}
			//count keeps track of iterations, needed for correct coordinates and colors
			count++;
		}
	}

	/*I am encoding the ETX char here because i couldnt figure out how to integrate them all in one
	  The process is exactly the same as above.
	 * */
	c = ETX;
		for (int k = 0; k < 8; k++)
		{
			
			x = v.at(count).row;
			y = v.at(count).col;

			temp = c >> k; //temp=temp >>k;
			

				switch(count % 3)
				{
					case 0:
						image[x][y].R &= 0xFE;
						image[x][y].R |= (temp & 0x1);
						break;
					case 1:
						image[x][y].G &= 0xFE;
						image[x][y].G |= (temp & 0x1);
						break;
					case 2:
						image[x][y].B &= 0xFE;
						image[x][y].B |= (temp & 0x1);
						break;
				}
			
			count++;
		}
}

void decode(ppm &image) 
{
	vector<pixel> v;
	set_pixel_list(image, v);
	
	char c;
	char temp = 0;
	unsigned int x, y;
	unsigned int count = 0;
	bool flag = false;
	//Flag is created to signal when ETX has been hit and when to stop
	while (flag == false)
	{
		//Char must be reset after each decode char iteration
		c = 0;
		for (int k = 0; k < 8 and count < v.size(); k++)
		{
		
			x = v.at(count).row;
			y = v.at(count).col;
			//LSB extraction and storing into a temporary char
			switch(count % 3)
			{
				case 0:
					temp = ((image[x][y].R & 0x1));
					break;
				case 1:	
					temp = ((image[x][y].G & 0x1));
					break;
				case 2: 
					temp = ((image[x][y].B & 0x1));
					break;
			}
			//Left shift operation on the extracted bit, OR'ed and stored into the char
			temp = temp << k;
			c |= temp;
		//Continuation of count to get correct Pixel  and index
		count++;
		}
		//When ETX has been reached, set the flag to true (ending loop) and break so it is not itself decoded
		if (c == ETX)
		{
			flag = true;
			break;
		}
		//Output of each decoded character in while
		cout << c;
	}	
		
}

//main first error checks arguments, using a substring to check for the correct file format.
//An object of ppm is then created, and is used to read and write to the files. if/elses simply check the user input and performs accordingly
int main(int argc, char *argv[]) 
{	
  if ((string(argv[1]) != "-encode" &&  string(argv[1]) != "-decode") || string(argv[2]).substr(string(argv[2]).length()-4) != ".ppm")
  {
	cerr << "Usage: -encode|decode image.ppm format required." << endl;
	return 0;
  }
//Object of ppm declaration, read file regardless
  ppm image;
  image.read(argv[2]);
  //Function encoded then written, same for decode
if(string(argv[1]) == "-encode")
{
	encode(image);
	image.write(argv[2]);
}
  else
  if (string(argv[1]) == "-decode") 
  {
	  decode(image);
	  image.write(argv[2]);
  }
}
