#include <cstdlib>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
	try
	{
		cv::Mat src = cv::imread(argv[1], cv::IMREAD_COLOR);
		cvtColor(src, src, cv::COLOR_BGR2GRAY);
		cv::Mat dst;
		equalizeHist(src, dst);
		cv::imshow("Source image", src);
		imshow("Equalized Image", dst);
		cv::waitKey();

	}
	catch (const std::exception&)
	{
		
	}
	getchar();
	return EXIT_SUCCESS;
}
