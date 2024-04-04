#include "grabcut.h"
#include "common.h"


void grabcut(std::string inputPath, std::string maskPath, std::string truthPath)
{
	cv::Mat input = cv::imread(inputPath);
	cv::Mat maskBefore = cv::imread(maskPath, cv::IMREAD_GRAYSCALE);

	cv::Mat maskGrabcut;
	toGrabcutMask(maskBefore, maskGrabcut);

	cv::Mat foreground, background;
	cv::grabCut(input, maskGrabcut, cv::Rect(), background, foreground, 10, cv::GC_INIT_WITH_MASK);

	cv::Mat maskAfter;
	toDisplayMask(maskGrabcut, maskAfter);

	cv::Mat output;
	applyGrabcutMask(input, maskAfter, output);

	cv::Mat truth = cv::imread(truthPath);

	outputGrabcutResults(input, maskBefore, maskAfter, output, truth);
	cv::waitKey();
}

void toGrabcutMask(cv::Mat mask, cv::Mat& output)
{
	output = mask.clone();

	for (int y = 0; y < output.rows; y++) {
		for (int x = 0; x < output.cols; x++) {
			uchar& pixelValue = output.at<uchar>(y, x);

			switch (pixelValue) {
			case 0:
				pixelValue = cv::GC_BGD;
				break;
			case 255:
				pixelValue = cv::GC_FGD;
				break;
			default:
				pixelValue = cv::GC_PR_BGD;
				break;
			}
		}
	}
}

void toDisplayMask(cv::Mat mask, cv::Mat& output)
{
	output = mask.clone();

	for (int y = 0; y < output.rows; y++) {
		for (int x = 0; x < output.cols; x++) {
			uchar& pixelValue = output.at<uchar>(y, x);

			switch (pixelValue) {
			case cv::GC_BGD:
			case cv::GC_PR_BGD:
				pixelValue = 0;
				break;
			case cv::GC_FGD:
			case cv::GC_PR_FGD:
				pixelValue = 255;
				break;
			}
		}
	}
}

void applyGrabcutMask(cv::Mat input, cv::Mat mask, cv::Mat& output)
{
	output = input.clone();

	for (int y = 0; y < output.rows; y++) {
		for (int x = 0; x < output.cols; x++) {
			cv::Vec3b& imagePixel = output.at<cv::Vec3b>(y, x);
			uchar maskPixel = mask.at<uchar>(y, x);

			if (maskPixel == 0) {
				imagePixel[0] = 0;
				imagePixel[1] = 0;
				imagePixel[2] = 0;
			}
		}
	}
}

void outputGrabcutResults(cv::Mat input, cv::Mat maskBefore, cv::Mat maskAfter, cv::Mat output, cv::Mat truth)
{
	std::vector<cv::Mat> maskBeforeChannels = { maskBefore, maskBefore, maskBefore };
	cv::Mat maskBeforeBGR;
	cv::merge(maskBeforeChannels, maskBeforeBGR);

	std::vector<cv::Mat> maskAfterChannels = { maskAfter, maskAfter, maskAfter };
	cv::Mat maskAfterBGR;
	cv::merge(maskAfterChannels, maskAfterBGR);

	cv::Mat empty = input.clone();
	empty *= 0;

	std::vector<std::vector<cv::Mat>> images = {
		{ input, maskBeforeBGR, maskAfterBGR },
		{ output, truth, empty }
	};
	outputImage(images, { "grabcut", "output\\grabcut\\", ".jpeg" });
}
