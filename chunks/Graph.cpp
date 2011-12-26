#include <iostream>

#include "Graph.h"

Graph::Graph(int vertex_count)
{
	this->vertex_count = vertex_count;

	adjacency_matrix = new bool*[vertex_count];
	for(int i = 0; i < vertex_count; ++i)
	{
		adjacency_matrix[i] = new bool[vertex_count];
		for(int j = 0; j < vertex_count; ++j)
		{
			adjacency_matrix[i][j] = 0;
		}
	}
}


Graph::~Graph()
{
	for(int i = 0; i < vertex_count; ++i)
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
		if(collection[i] == val)
		{
			return 1;
		}
	}
	return 0;
}


int Graph::contained(std::vector<int> &vec1, std::vector<int> &vec2)
{
	std::vector<int> *vecIn;
	std::vector<int> *vecOut;

	int res = 1;

	if(vec1.size() >= vec2.size())
	{
		vecOut = &vec1;
		vecIn = &vec2;

		res = -1;
	}
	else
	{
		vecOut = &vec2;
		vecIn = &vec1;		
	}

	for(unsigned int i = 0; i < vecIn->size(); ++i)
	{
		if(!isInCollection(*(vecIn->begin() + i), *vecOut))
		{
			return 0;
		}
	}

	return res;
}



void Graph::getCliques(std::vector<std::vector<int> > &cliques)
{
	std::vector<int> tmp;
	int tmp_size = 0, j_compare, i_compare, num;

	for(int i = 0; i < vertex_count; ++i) // begin reading row
	{
		tmp.push_back(i);
		for(int j = i+1; j < vertex_count; ++j) // search 1 in row
		{
			if(adjacency_matrix[i][j] == 1)
			{
				tmp_size = 1;

				for(j_compare = j; j_compare < vertex_count; ++j_compare)
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

		cliques.push_back(tmp);
		tmp.clear();
	}

	
	std::vector<int> collection;

	for(unsigned int i = 0; i < cliques.size(); ++i)
	{
		if(cliques[i].size() <= 1)
		{
			cliques.erase(cliques.begin()+i);
			i--;			
		}
		else
		{
			collection.push_back(cliques[i][0]);

			for(unsigned int j = 1; j < cliques[i].size(); ++j)
			{
				if(isInCollection(cliques[i][j], collection) == 1)
				{
					cliques[i].erase((cliques[i]).begin()+j);
					j--;
				}
				else
				{
					collection.push_back(cliques[i][j]);
				}
				
			}
		}

		collection.clear();
	}

	for(unsigned int i = 0; i < cliques.size(); ++i)
	{
		for(unsigned int j = i+1; j < cliques.size(); ++j)
		{
			if(contained(cliques[i], cliques[j]) == 1)
			{
				cliques.erase(cliques.begin()+i);
				i--;
				break;
			}
			else
			if(contained(cliques[i], cliques[j]) == -1)
			{
				cliques.erase(cliques.begin()+j);
				j--;
			}
		}
	}
}