#include <opencv2/opencv.hpp>
#include "grabcut.h"


int main()
{
	grabcut("assets\\JPEGImages\\2007_000033.jpg",
		"assets\\masks\\2007_000033.jpg",
		"assets\\SegmentationObject\\2007_000033.png");

	return 0;
}
