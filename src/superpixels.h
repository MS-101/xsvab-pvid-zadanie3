#pragma once

#include <opencv2/opencv.hpp>


void superpixels(std::string name, cv::Mat input, cv::Mat truthMask, cv::Mat truth);
void outputSuperpixels(std::string name, cv::Mat input, cv::Mat blur, cv::Mat mask, cv::Mat output, cv::Mat truthMask, cv::Mat truth);
