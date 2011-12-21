#ifndef GRAPH_H
#define GRAPH_H

#include <vector>


class Graph
{
private:
	bool **adjacency_matrix;
	int tops_count;	//TODO rename to vertex_count

	bool isInCollection(int val, std::vector<int> &collection);

public:
	Graph(int tops_count);
	~Graph();

	void setAdjacency(int i, int j, bool adjacency);

	bool getAdjacency(int i, int j);

	void getClique(std::vector<std::vector<int> > &similar_images);
};


#endif // GRAPF_H
