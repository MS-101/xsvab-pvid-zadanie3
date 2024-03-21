#include <opencv2/opencv.hpp>


int main()
{
	cv::Mat input = cv::imread("assets\\JPEGImages\\2007_000032.jpg");
	cv::imshow("input", input);

	cv::waitKey();

	return 0;
}
