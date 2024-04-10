#pragma once

#include <opencv2/opencv.hpp>


void floodfill(std::string name, cv::Mat input, cv::Mat truthMask, cv::Mat truth);
void outputFloodfill(std::string name, cv::Mat input, cv::Mat mask, cv::Mat output, cv::Mat truthMask, cv::Mat truth);
