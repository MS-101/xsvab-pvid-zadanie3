#include "superpixels.h"
#include "common.h"
#include <opencv2/ximgproc.hpp>


void superpixels(std::string name, cv::Mat input, cv::Mat truthMask, cv::Mat truth)
{
    cv::Mat blur;
    cv::GaussianBlur(input, blur, cv::Size(5, 5), 0);

    cv::Mat converted;
    cvtColor(blur, converted, cv::COLOR_BGR2Lab);
    
    auto slic = cv::ximgproc::createSuperpixelSLIC(converted, cv::ximgproc::SLIC, 25, 10.0f);
    slic->iterate(50);

    cv::Mat mask;
    slic->getLabelContourMask(mask, false);

    cv::Mat output = input.clone();
    output.setTo(cv::Scalar(0, 0, 255), mask);
    
    outputSuperpixels(name, input, blur, mask, output, truthMask, truth);
}

void outputSuperpixels(std::string name, cv::Mat input, cv::Mat blur, cv::Mat mask, cv::Mat output, cv::Mat truthMask, cv::Mat truth)
{
    cv::Mat maskBGR;
    grayToBGR(mask, maskBGR);

    cv::Mat truthMaskBGR;
    grayToBGR(truthMask, truthMaskBGR);

    std::vector<std::vector<cv::Mat>> images = {
		{ input, blur },
        { maskBGR, output },
        { truthMaskBGR, truth }
	};

    outputImage(images, { name, "../output/VOC12/superpixels/", ".jpg" });
}
