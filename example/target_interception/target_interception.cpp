#include <cstdlib>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc.hpp>

/*

图像增强：
https://www.cnblogs.com/jukan/p/7815722.html
*/


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s [imgpath...]", argv[0]);
		return EXIT_FAILURE;
	}
	
	try
	{
        cv::Mat img = cv::imread(argv[1]);
        cv::Mat mid;
        cv::cvtColor(img, mid, cv::COLOR_BGR2HSV);
        cv::Mat dst;
	    cv::inRange(mid, cv::Scalar(35, 43, 100), cv::Scalar(99, 255, 255), dst);
     
        cv::Mat bw;
        cv::bitwise_and(img, img, bw, dst);
        cv::cvtColor(bw, bw, CV_BGR2GRAY);  
        cv::blur(bw, bw, cv::Size(9, 9));
        cv::dilate(bw,bw,cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11)));
        cv::erode(bw,bw,cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9)));
        cv::threshold(bw, bw, 50, 255, cv::THRESH_BINARY);


        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(bw, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        std::vector<cv::Point2f> approx;  
        cv::approxPolyDP(contours[0], approx, cv::arcLength(contours[0], true) * 0.02, true);  
        // cv::Canny(bw, bw, 100, 100, 3); 
        // std::vector<cv::Vec4i> lines;  
        // cv::HoughLinesP(bw, lines, 1, CV_PI/180, 50, 50, 10);  

        cv::Mat quad = cv::Mat::zeros(200, 300, CV_8UC3);  
  
        std::vector<cv::Point2f> quad_pts; 
 
        quad_pts.push_back(cv::Point2f(quad.cols, 0));  
        quad_pts.push_back(cv::Point2f(0, 0));  
        quad_pts.push_back(cv::Point2f(0, quad.rows));  
        quad_pts.push_back(cv::Point2f(quad.cols, quad.rows)); 


        cv::Mat transmtx = cv::getPerspectiveTransform(approx, quad_pts);  
        cv::warpPerspective(img, quad, transmtx, quad.size());  

        cv::Mat imageLaplace; 
        cv::Mat kernel = (cv::Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);  
        cv::filter2D(quad, imageLaplace, CV_8UC3, kernel);  

        cv::Mat imagebinary; 
        cv::cvtColor(quad, imagebinary, CV_BGR2GRAY);  
        cv::GaussianBlur(imagebinary, imagebinary, cv::Size(5,5), 0, 0);
        cv::threshold(imagebinary, imagebinary, 200, 255, cv::THRESH_BINARY);

        cv::imshow("original image", img);
        cv::imshow("license plate Laplace", imageLaplace);
        cv::imshow("license plate Laplace Binary", imagebinary);
		cv::imshow("license plate", quad);
		cv::waitKey();
	}
	catch (const std::exception& e)
	{
	}
	getchar();
	return EXIT_SUCCESS;
}
