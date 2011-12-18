#ifndef GRAPH_H
#define GRAPH_H



class Graph
{
private:
	bool **adjacency_matrix;
	int tops_count;

public:
	Graph(int tops_count);
	~Graph();

	void setAdjacency(int i, int j, bool adjacency);

	bool getAdjacency(int i, int j);

	void getClique();
};


#endif // GRAPF_H