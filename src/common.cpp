#include "common.h"


void outputImage(std::vector<std::vector<cv::Mat>> images, OutputArgs outputArgs)
{
	std::vector<cv::Mat> mergedImageRows;
	for (auto& imageRow : images) {
		cv::Mat mergedImageRow;
		cv::hconcat(imageRow, mergedImageRow);
		mergedImageRows.push_back(mergedImageRow);
	}

	cv::Mat mergedImage;
	cv::vconcat(mergedImageRows, mergedImage);

	outputImage(mergedImage, outputArgs);
}

void outputImage(cv::Mat image, OutputArgs outputArgs)
{
	cv::imwrite(outputArgs.dir + outputArgs.name + outputArgs.extension, image);
}

void outputDiceScore(std::string name, cv::Mat image1, cv::Mat image2)
{
	cv::Mat intersection = image1 & image2;
    int intersection_count = cv::countNonZero(intersection);
    int union_count = cv::countNonZero(image1) + cv::countNonZero(image2);

    double dice = (2.0 * intersection_count) / union_count;
	std::cout << "[" << name << "] dice score = " << dice << std::endl;
}

void grayToBGR(cv::Mat gray, cv::Mat& bgr)
{
	std::vector<cv::Mat> channels = { gray, gray, gray };
	cv::merge(channels, bgr);
}

void createTruthMask(cv::Mat input, cv::Mat& output, int blue, int green, int red)
{
	output = input.clone();

    for (int y = 0; y < output.rows; ++y) {
        for (int x = 0; x < output.cols; ++x) {
            cv::Vec3b& pixel = output.at<cv::Vec3b>(y, x);

            if (pixel[0] == blue && pixel[1] == green && pixel[2] == red) {
				pixel[0] = 255;
				pixel[1] = 255;
				pixel[2] = 255;
            } else {
				pixel[0] = 0;
				pixel[1] = 0;
				pixel[2] = 0;
			}
        }
    }

	cv::cvtColor(output, output, cv::COLOR_BGR2GRAY);
}

void outputTruthMask(std::string name, cv::Mat input, cv::Mat label, cv::Mat truthMask, cv::Mat truth)
{
	cv::Mat truthMaskBGR;
	grayToBGR(truthMask, truthMaskBGR);

	std::vector<std::vector<cv::Mat>> images = {
		{ input, label },
		{ truthMaskBGR, truth }
	};

	outputImage(images, { name, "../output/VOC12/truth/", ".jpg" });
}

void applyMask(cv::Mat input, cv::Mat mask, cv::Mat& output)
{
	output = input.clone();

	for (int y = 0; y < output.rows; y++) {
		for (int x = 0; x < output.cols; x++) {
			cv::Vec3b& imagePixel = output.at<cv::Vec3b>(y, x);
			uchar maskPixel = mask.at<uchar>(y, x);

			if (maskPixel == 0)
				imagePixel = 0;
		}
	}
}
