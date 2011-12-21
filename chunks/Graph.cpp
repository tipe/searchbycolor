#include <iostream>

#include "Graph.h"

Graph::Graph(int tops_count)
{
	this->tops_count = tops_count;

	adjacency_matrix = new bool*[tops_count];
	for(int i = 0; i < tops_count; ++i)
	{
		adjacency_matrix[i] = new bool[tops_count];
		for(int j = 0; j < tops_count; ++j)
		{
			adjacency_matrix[i][j] = 0;
		}
	}
}


Graph::~Graph()
{
	for(int i = 0; i < tops_count; ++i)
	{
		delete[] adjacency_matrix[i];
	}

	delete[] adjacency_matrix;
}


void Graph::setAdjacency(int i, int j, bool adjacency)
{
	adjacency_matrix[i][j] = adjacency_matrix[j][i] = adjacency;
}


bool Graph::getAdjacency(int i, int j)
{
	return adjacency_matrix[i][j];
}


bool Graph::isInCollection(int val, std::vector<int> &collection)
{
	for(unsigned int i = 0; i < collection.size(); ++i)
	{
		if((int)collection[i] == val)
		{
			return 1;
		}
	}
	return 0;
}



void Graph::getClique(std::vector<std::vector<int> > &similar_images)
{
	std::cout<<"!!!!!!!!"<<std::endl;

	std::vector<int> tmp;
	int tmp_size = 0, j_compare, i_compare, num;

	for(int i = 0; i < tops_count; ++i) // begin reading row
	{
		tmp.push_back(i);
		for(int j = i+1; j < tops_count; ++j) // search 1 in row
		{
			if(adjacency_matrix[i][j] == 1)
			{
				tmp_size = 1;

				for(j_compare = j; j_compare < tops_count; ++j_compare)
				{
					i_compare = i;
					num = 0;

					while(num < tmp_size && adjacency_matrix[tmp[num]][j_compare] == 1)
					{
						i_compare = tmp[num];
						num++;
					}

					if(num == tmp_size)
					{
						tmp_size++;
						tmp.push_back(j_compare);
					}
				}			
			}
		}

		similar_images.push_back(tmp);
		tmp.clear();
	}

	
	std::vector<int> collection;

	for(unsigned int i = 0; i < similar_images.size(); ++i)
	{
		if(similar_images[i].size() <= 1)
		{
			similar_images.erase(similar_images.begin()+i);
			i--;			
		}
		else
		{
			collection.push_back(similar_images[i][0]);

			for(unsigned int j = 1; j < similar_images[i].size(); ++j)
			{
				if(isInCollection(similar_images[i][j], collection) == 1)
				{
					similar_images[i].erase((similar_images[i]).begin()+j);
					j--;
				}
				else
				{
					collection.push_back(similar_images[i][j]);
				}
				
			}
		}

		collection.clear();
	}
}