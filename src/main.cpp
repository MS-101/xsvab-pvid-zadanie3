#include "grabcut.h"
#include "superpixels.h"
#include "floodfill.h"
#include "threshold.h"
#include "common.h"
#include <opencv2/opencv.hpp>
#include <unordered_map>


int main() {
	// VOC12 dataset segmentation

	std::string inputDir = "../input/VOC12/images/";
	std::string labelDir = "../input/VOC12/labels/";
	std::string maskDir = "../input/VOC12/masks/";

	std::string inputExtension = ".jpg";
	std::string labelExtension = ".png";
	std::string maskExtension = ".jpg";
	std::string outputExtension = ".jpg";

	std::vector<std::string> imageNames = {
		"2007_000033",
		"2007_000123",
		"2007_000063"
	};

	for (std::string imageName : imageNames)
	{
		// load input images

		cv::Mat input = cv::imread(inputDir + imageName + inputExtension);
		cv::Mat label = cv::imread(labelDir + imageName + labelExtension);
		cv::Mat mask = cv::imread(maskDir + imageName + maskExtension, cv::IMREAD_GRAYSCALE);

		// create truth mask

		cv::Mat truthMask, truth;
		createTruthMask(label, truthMask, 0, 0, 128);
		applyMask(input, truthMask, truth);
		outputTruthMask(imageName, input, label, truthMask, truth);

		// perform segmentation

		std::string imageNameFull = imageName + outputExtension;

		grabcut(imageNameFull, input, mask, truthMask, truth);
		superpixels(imageNameFull, input, truthMask, truth);
		floodfill(imageNameFull, input, truthMask, truth);
		threshold(imageNameFull, input, truthMask, truth);
	}

	return 0;
}
