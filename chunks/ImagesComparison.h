#ifndef IMAGESCOMPARISON_H
#define IMAGESCOMPARISON_H

#include <vector>
#include <string>

#include "Image.h"
#include "Graph.h"


class ImagesComparison
{
private:
	void buildGraph(std::vector<Image*> &images, Graph *graph);
		

public:
	ImagesComparison();
	~ImagesComparison();

	void getSimilarImages(std::vector<Image*> &images, std::vector<std::vector<int> > &similar_images);
};


#endif // IMAGESCOMPARISON_H