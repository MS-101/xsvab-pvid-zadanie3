#pragma once

#include <opencv2/opencv.hpp>


struct OutputArgs {
	std::string name;
	std::string dir;
	std::string extension;
};

void outputImage(std::vector<std::vector<cv::Mat>> images, OutputArgs outputArgs);
void outputImage(cv::Mat image, OutputArgs outputArgs);
void outputDiceScore(std::string name, cv::Mat image1, cv::Mat image2);

void grayToBGR(cv::Mat gray, cv::Mat& bgr);

void createTruthMask(cv::Mat input, cv::Mat& output, int blue, int green, int red);
void outputTruthMask(std::string name, cv::Mat input, cv::Mat label, cv::Mat truthMask, cv::Mat truth);
void applyMask(cv::Mat input, cv::Mat mask, cv::Mat& output);
