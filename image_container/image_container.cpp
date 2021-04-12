#include <cstdlib>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
	try
	{
		printf("Hello world\n");
		cv::Mat A, C;									// creates just the header parts
		A = cv::imread("C:\\Users\\Quenwaz\\Pictures\\IMG\\3.jpg", cv::IMREAD_COLOR);	// here we'll know the method used (allocate matrix)
		cv::Mat B(A);									// Use the copy constructor
							
		C = A;											// Assignment operator
	}
	catch (const std::exception& e)
	{
		
	}
	getchar();
	return EXIT_SUCCESS;
}
