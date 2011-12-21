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
	for(unsigned int i = 0; i < images_dists.size(); ++i)
	{
		images_dists[i]->~Distribution();
	}
}


void ImagesComparative::buildGraph(std::vector<Image*> &images, Graph *graph)
{
	std::cerr<<"build graph "<<std::endl;

	int images_count = images.size();

	Distribution *image_dist;


	for(int i = 0; i < images_count; ++i)
	{
		std::cerr<<"i = "<<i<<std::endl;

		image_dist = new Distribution();

		std::cerr<<"111111"<<std::endl;
		images[i]->getDistribution(image_dist);

		std::cerr<<"========="<<std::endl;

		// for(int ii = 0; ii < image_dist->getDivider(); ++ii)
		// 	for(int jj = 0; jj < image_dist->getDivider(); ++jj)
		// 		for(int kk = 0; kk < image_dist->getDivider(); ++kk)
		// 			std::cerr<<"! "<<image_dist->getValue(ii,jj,kk)<<std::endl;

		// std::cerr<<"========="<<std::endl;

		images_dists.push_back(image_dist);
	}

	bool similar;

	std::cerr<<"buildGraph 1"<<std::endl;

	for(int i = 0; i < images_count; ++i)
	{
		for(int j = i+1; j < images_count; ++j)
		{
			if(i == j) break;

			//std::cerr<<"i = "<<i<<" j = "<<j<<std::endl;

			similar = images_dists[i]->compare(images_dists[j]);
			graph->setAdjacency(i,j,similar);
		}
	}

	std::cerr<<"graph:"<<std::endl;

	for(int i = 0; i < images_count; ++i)
	{
		for(int j = 0; j < images_count; ++j)
		{
			std::cerr<<graph->getAdjacency(i,j)<<" ";			
		}
		std::cerr<<std::endl;
	}
}


void ImagesComparative::getSimilarImages(std::vector<Image*> &images, std::vector<std::vector<int> > &similar_images)
{
	std::cout<<"!!!!"<<std::endl;
	Graph graph(images.size());
	buildGraph(images, &graph);

	graph.getClique(similar_images);	
}