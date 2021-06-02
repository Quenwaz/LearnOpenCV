#include <cstdlib>
#include <iostream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"


size_t ParseCmdArgs(int argc, char** argv, std::vector<cv::Mat>& vecImgs)
{
	if (argc < 3)
	{
		printf("Usage: %s [imgpath...]", argv[0]);
		return 0;
	}
	
	for (size_t i = 1;i < argc; ++i)
	{
		cv::Mat img = cv::imread(argv[i]);
		if (img.empty())
		{
			continue;
		}

		vecImgs.push_back(img);
	}

	return vecImgs.size();
}



int main(int argc, char* argv[])
{
	std::vector<cv::Mat> vecImgs;
	if (ParseCmdArgs(argc, argv, vecImgs) < 2)
	{
		return EXIT_FAILURE;
	}
	
	try
	{
		cv::Mat pano;
		cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::SCANS);
		const cv::Stitcher::Status status = stitcher->stitch(vecImgs, pano);
		if (status != cv::Stitcher::OK)
		{
			std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
			return EXIT_FAILURE;
		}

		cv::imshow("stitch image", pano);
		cv::waitKey();
	}
	catch (const std::exception& e)
	{
	}
	getchar();
	return EXIT_SUCCESS;
}
