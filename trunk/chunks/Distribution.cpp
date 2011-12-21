#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "Distribution.h"


void Distribution::getThreeDimensionalMatrix()
{
	colors_distribution = new float**[size_distribution];
	for(int i = 0; i < size_distribution; i++)	//TODO simplify
	{
		colors_distribution[i] = new float*[size_distribution];
	}
	for(int i = 0; i < size_distribution; i++)
	{
		for(int j = 0; j < size_distribution; j++)
		{
			colors_distribution[i][j] = new float[size_distribution];
		}
	}

	for(int i = 0; i < size_distribution; i++)
	{
		for(int j = 0; j < size_distribution; j++)
		{
			for(int k = 0; k < size_distribution; k++)
			{
				colors_distribution[i][j][k] = 0;
			}
		}		
	}
}


void Distribution::deleteThreeDimensionalMatrix()
{
	for(int i = 0; i < size_distribution; i++) 
	{
		for(int j = 0; j < size_distribution; j++) 
		{
			delete [] colors_distribution[i][j];
		}
	}

	for(int i = 0; i < size_distribution; i++)
	{
		delete[] colors_distribution[i];
	}

	delete[] colors_distribution;
		
}


Distribution::Distribution(int divider)
{
	size_distribution = divider;
	getThreeDimensionalMatrix();	//TODO inline
}


Distribution::~Distribution()
{
	deleteThreeDimensionalMatrix();
}


int Distribution::getDivider()
{
	return size_distribution;
}


void Distribution::setValue(int i, int j, int k, float value)
{
	colors_distribution[i][j][k] = value;
}


float Distribution::getValue(int i, int j, int k)
{
	return colors_distribution[i][j][k];
}


void Distribution::addValue(int i, int j, int k, float delta)
{
	colors_distribution[i][j][k] += delta;
	static int y = 0;
	//std::cerr<<"y="<<y<<std::endl;
	//if(y > 16000) std::cerr<<" y = "<<y;
	y++;
}

int Distribution::compare(Distribution *dist, int range_percent)
{
	if(size_distribution != dist->getDivider())
	{
		return -1;	//TODO it's better to use exception here
	}

	int count = 0;	

	for(int i = 0; i < size_distribution; ++i)
	{
		for(int j = 0; j < size_distribution; ++j)
		{
			for(int k = 0; k < size_distribution; ++k)
			{
				//cout<<"1 = "<<range_colors[ii][i][j][k]<<endl;
				//cout<<"2 = "<<range_colors[jj][i][j][k]<<endl;

				if(abs(colors_distribution[i][j][k] - dist->getValue(i,j,k)) <= range_percent)
				{
					count++;
				}	
			}
		}
	}

	if(count >= pow(size_distribution, 3))
	{
		return 1;
	}

	return 0;
}
