#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

#include "Distribution.h"




Distribution::Distribution(int size)
{
	size_distribution = size;
	
	colors_distribution = new float**[size_distribution];
	for(int i = 0; i < size_distribution; i++)
	{
		colors_distribution[i] = new float*[size_distribution];
		for(int j = 0; j < size_distribution; j++)
		{
			colors_distribution[i][j] = new float[size_distribution];
			for(int k = 0; k < size_distribution; k++)
			{
				colors_distribution[i][j][k] = 0;
			}
		}
	}
}


Distribution::~Distribution()
{
	for(int i = 0; i < size_distribution; i++)	//TODO simplify 
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


int Distribution::getDivider()	//TODO rename
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
}

int Distribution::compare(Distribution *dist, int range_percent)
{
	if(size_distribution != dist->getDivider())
	{
		throw std::runtime_error("Bad distribution comparing");
	}

	int count = 0;	

	for(int i = 0; i < size_distribution; ++i)
	{
		for(int j = 0; j < size_distribution; ++j)
		{
			for(int k = 0; k < size_distribution; ++k)
			{
				if(abs(colors_distribution[i][j][k] - dist->getValue(i,j,k)) <= range_percent)
				{
					count++;
				}	
			}
		}
	}

	if(count >= pow(size_distribution, 3))	//TODO simplify
	{
		return 1;
	}

	return 0;
}
