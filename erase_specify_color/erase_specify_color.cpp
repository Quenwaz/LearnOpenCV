#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


int main(int argc, char* argv[])
{
	cv::Mat mat = cv::imread(R"(C:\Users\Quenwaz\Desktop\TCP_Stop.png)", cv::IMREAD_COLOR);

	cv::Mat mid;
	cv::cvtColor(mat, mid, cv::COLOR_BGR2HSV);
	cv::Mat dst;
	cv::inRange(mid, cv::Scalar(0, 0, 46), cv::Scalar(180, 43, 220), dst);


	cv::imshow("gray", dst);

	
	uint rows = mat.rows /** mat.channels()*/;
	uint cols = mat.cols;
	// if (mat.isContinuous())
	// {
	// 	cols *= rows;
	// 	rows = 1;
	// }

	for (size_t i =0;i < rows; ++i)
	{
		// uchar* p = mat.ptr<uchar>(i);
		for (size_t j = 0; j < cols; ++j)
		{
			if(dst.at<uchar>(i, j) > 50)
				mat.at<cv::Vec3b>(i, j) = { 255, 255, 255 };
		}
	}

	cv::imshow("org", mat);
	cv::waitKey();
	return 0;
}
