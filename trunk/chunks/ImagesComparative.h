#ifndef IMAGESCOMPARATIVE_H
#define IMAGESCOMPARATIVE_H

#include <vector>
#include <string>

#include "Image.h"
#include "Graph.h"


class ImagesComparative	//TODO rename to ImagesComparison
{
private:
	std::vector<Distribution*> images_dists;
	void buildGraph(std::vector<Image*> &images, Graph *graph);
		

public:
	ImagesComparative();
	~ImagesComparative();

	void getSimilarImages(std::vector<Image*> &images, std::vector<std::vector<int> > &similar_images);
};


#endif // IMAGESCOMPARATIVE_H
