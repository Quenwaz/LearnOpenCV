#include <cstdlib>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
	try
	{
		cv::Mat A, C;									// creates just the header parts
														// here we'll know the method used (allocate matrix)
		A = cv::imread("C:\\Users\\Quenwaz\\Pictures\\IMG\\3.jpg", cv::IMREAD_COLOR);
		cv::Mat B(A);									// Use the copy constructor
		C = A;											// Assignment operator
		cv::imshow("img", C);
		cv::waitKey();
	}
	catch (const std::exception& e)
	{
	}
	getchar();
	return EXIT_SUCCESS;
}
