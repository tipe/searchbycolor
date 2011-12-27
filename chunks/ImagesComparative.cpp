#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "Graph.h"
#include "Distribution.h"

#include "ImagesComparative.h"



ImagesComparative::ImagesComparative()
{
	
}


ImagesComparative::~ImagesComparative()
{
	
}


void ImagesComparative::buildGraph(std::vector<Image*> &images, Graph *graph)
{
	std::vector<Distribution*> images_dists;

	for(unsigned int i = 0; i < images.size(); ++i)
	{
		Distribution *image_dist = new Distribution();

		images[i]->getDistribution(image_dist);
		
		images_dists.push_back(image_dist);
	}

	for(unsigned int i = 0; i < images.size(); ++i)
	{
		for(unsigned int j = i+1; j < images.size(); ++j)
		{
			if(i == j) break;

			bool similar = images_dists[i]->compare(images_dists[j]);
			graph->setAdjacency(i,j,similar);
		}
	}

	
	for(unsigned int i = 0; i < images_dists.size(); ++i)
	{
		delete images_dists[i];
	}

	images_dists.clear();
}


void ImagesComparative::getSimilarImages(std::vector<Image*> &images, std::vector<std::vector<int> > &similar_images)
{
	Graph graph(images.size());
	buildGraph(images, &graph);

	graph.getCliques(similar_images);	
}