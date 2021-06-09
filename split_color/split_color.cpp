#include <cstdio>
#include <opencv2/highgui.hpp>

#include "opencv2/core.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


void lookup_rgb_channels(cv::Mat mat)
{
	cv::Mat dst_R = cv::Mat::zeros(mat.size(), CV_8UC3);
	cv::Mat dst_G = cv::Mat::zeros(mat.size(), CV_8UC3);
	cv::Mat dst_B = cv::Mat::zeros(mat.size(), CV_8UC3);
	int ch[] = { 0, 0 };
	cv::mixChannels(&mat, 1, &dst_B, 1, ch, 1);
	cv::imshow("B", dst_B);
	ch[0] = 1;
	ch[1] = 1;
	cv::mixChannels(&mat, 1, &dst_G, 1, ch, 1);
	cv::imshow("G", dst_G);
	ch[0] = 2;
	ch[1] = 2;
	cv::mixChannels(&mat, 1, &dst_R, 1, ch, 1);
	cv::imshow("R", dst_R);
	cv::waitKey();
}


void lookup_hsv_channels(cv::Mat mat)
{
	cv::cvtColor(mat, mat, cv::COLOR_BGR2HSV);
	cv::Mat dst_R, dst_G, dst_B;
	dst_R.create(mat.size(), CV_8U);
	dst_G.create(mat.size(), CV_8U);
	dst_B.create(mat.size(), CV_8U);
	int ch[] = { 0, 0 };
	cv::mixChannels(&mat, 1, &dst_B, 1, ch, 1);
	cv::imshow("H", dst_B);
	ch[0] = 1;
	cv::mixChannels(&mat, 1, &dst_G, 1, ch, 1);
	cv::imshow("S", dst_G);
	ch[0] = 2;
	cv::mixChannels(&mat, 1, &dst_R, 1, ch, 1);
	cv::imshow("V", dst_R);
	cv::waitKey();
}



int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [imgpath]", argv[0]);
		return 1;
	}

	cv::Mat mat = cv::imread(argv[1], cv::IMREAD_COLOR);
	if (mat.empty())
	{
		fprintf(stderr, "Input image is invalid.\n");
		return 1; 
	}

	CV_Assert(mat.channels() == 3);
	
	std::vector<cv::Mat> vecSplits(mat.channels());
	cv::split(mat, vecSplits);

	cv::imshow("channel_b", vecSplits.at(0));
	cv::imshow("channel_g", vecSplits.at(1));
	cv::imshow("channel_r", vecSplits.at(2));

	lookup_rgb_channels(mat);
	cv::waitKey(0);
	
	
	return 0;
}
