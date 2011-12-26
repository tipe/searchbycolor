#ifndef GRAPH_H
#define GRAPH_H

#include <vector>


class Graph
{
private:
	bool **adjacency_matrix;
	int vertex_count;

	bool isInCollection(int val, std::vector<int> &collection);
	int contained(std::vector<int> &vec1, std::vector<int> &vec2);

public:
	Graph(int vertex_count);
	~Graph();

	void setAdjacency(int i, int j, bool adjacency);

	bool getAdjacency(int i, int j);

	void getCliques(std::vector<std::vector<int> > &cliques);
};


#endif // GRAPF_H