#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>


cv::Mat extract_specify_color(cv::Mat org, const cv::Scalar& lower, const cv::Scalar& upper)
{
	cv::Mat mid;
	cv::cvtColor(org, mid, cv::COLOR_BGR2HSV);
	cv::Mat dst;
	cv::inRange(mid, lower, upper, dst);

	cv::Mat matResult=cv::Mat::zeros(org.rows, org.cols, org.type());
	for (size_t i =0;i < matResult.rows; ++i)
	{
		// uchar* p = mat.ptr<uchar>(i);
		for (size_t j = 0; j < matResult.cols; ++j)
		{

			if(dst.at<uchar>(i, j) > 0)
			{
				const size_t N = 3;
				std::vector<uchar> vecR, vecG, vecB;
				for (int x = j; x < (j + N) && x < matResult.cols; ++x)
				{
					vecB.push_back(org.at<cv::Vec3b>(i, x)[0]);
					vecG.push_back(org.at<cv::Vec3b>(i, x)[1]);
					vecR.push_back(org.at<cv::Vec3b>(i, x)[2]);
				}

				uchar r = *std::max_element(vecR.begin(), vecR.end());
				uchar g = *std::max_element(vecG.begin(), vecG.end());
				uchar b = *std::max_element(vecB.begin(), vecB.end()) ;

				matResult.at<cv::Vec3b>(i, j) = { b, g, r };

			}
			// else matResult.at<cv::Vec3b>(i, j) = {0, 0, 0};
		}
	}
	return matResult;
}



int main(int argc, char const* argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [imgpath]", argv[0]);
		return 1;
	}
	
	const char* img_path = argv[1];
	cv::Mat mat = cv::imread(img_path, cv::IMREAD_COLOR);
	cv::namedWindow("org",cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
	cv::namedWindow("red",cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
	cv::namedWindow("green",cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
	cv::namedWindow("orange",cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);

	cv::imshow("org", mat);
	cv::Mat red=extract_specify_color(mat, cv::Scalar(0, 43, 46), cv::Scalar(10, 255, 255));
	cv::Mat green=extract_specify_color(mat, cv::Scalar(35, 43, 46), cv::Scalar(77, 255, 255));
	cv::Mat orange=extract_specify_color(mat, cv::Scalar(11, 43, 46), cv::Scalar(25, 255, 255));
	cv::imshow("red", red);
	cv::imshow("green", green);
	cv::imshow("orange", orange);
	cv::waitKey();
	// cv::imwrite(img_path, matResult);
	return 0;
}
