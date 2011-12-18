#include <vector>
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

void Graph::getClique()
{
	//std::cerr<<"getClique tops_count = "<<tops_count<<std::endl;

	std::vector<std::vector<int> > similar_images;
	std::vector<int> tmp;
	int tmp_size = 0, j_compare, i_compare, num;

	for(int i = 0; i < tops_count; ++i)
	{
		tmp.push_back(i);
		for(int j = i+1; j < tops_count; ++j)
		{
			//std::cerr<<"! j = "<<j<<std::endl;
			if(adjacency_matrix[i][j] == 1)
			{
				tmp_size = 1;

				for(j_compare = j; j_compare < tops_count; ++j_compare)
				{
					//std::cerr<<"!!! j_compare = "<<j_compare<<std::endl;
					i_compare = i;
					num = 0;
					//std::cerr<<"! tmp.size() = "<<tmp.size()<<std::endl;
					//std::cerr<<"! num = "<<num<<" tmp_size = "<<tmp_size<<" tmp[num] = "<<tmp[num]<<std::endl;
					while(num < tmp_size && adjacency_matrix[tmp[num]][j_compare] == 1)
					{
						//std::cerr<<"!!! num = "<<num<<" tmp_size = "<<tmp_size<<std::endl;
						i_compare = tmp[num];
						num++;
						//std::cerr<<"123 num = "<<num<<" tmp_size = "<<tmp_size<<std::endl;
					}

					//std::cerr<<"pred"<<std::endl;

					if(num == tmp_size)
					{
						//std::cerr<<"if if if"<<std::endl;
						tmp_size++;
						tmp.push_back(j_compare);
					}

					//std::cerr<<"! tmp.size() = "<<tmp.size()<<" tmp_size = "<<tmp_size<<std::endl;
					if(tmp_size > 1)
					{
						for(int k = 0; k < tmp.size(); ++k)
						{
							//std::cout<<"!!! "<<tmp[k]<<std::endl;
						}

						similar_images.push_back(tmp);
						tmp.clear();
						//std::cout<<"----"<<std::endl;
					}
				}			
			}
		}
	}
}