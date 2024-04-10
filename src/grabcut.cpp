#include "grabcut.h"
#include "common.h"
#include <iostream>


void grabcut(std::string name, cv::Mat input, cv::Mat mask, cv::Mat truthMask, cv::Mat truth)
{
	cv::Mat maskGrabcut;
	toGrabcutMask(mask, maskGrabcut);

	cv::Mat foreground, background;
	cv::grabCut(input, maskGrabcut, cv::Rect(), background, foreground, 10, cv::GC_INIT_WITH_MASK);

	cv::Mat maskAfter;
	toDisplayMask(maskGrabcut, maskAfter);

	cv::Mat output;
	applyMask(input, maskAfter, output);

	outputGrabcut(name, input, mask, maskAfter, output, truthMask, truth);
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

void outputGrabcut(std::string name, cv::Mat input, cv::Mat maskBefore, cv::Mat maskAfter, cv::Mat output, cv::Mat truthMask, cv::Mat truth)
{
	cv::Mat maskBeforeBGR;
	grayToBGR(maskBefore, maskBeforeBGR);

	cv::Mat maskAfterBGR;
	grayToBGR(maskAfter, maskAfterBGR);

	cv::Mat truthMaskBGR;
	grayToBGR(truthMask, truthMaskBGR);

	std::vector<std::vector<cv::Mat>> images = {
		{ input, maskBeforeBGR },
		{ maskAfterBGR, output },
		{ truthMaskBGR, truth }
	};

	outputImage(images, { name, "../output/VOC12/grabcut/", ".jpg" });
	outputDiceScore(name, maskAfter, truthMask);
}
