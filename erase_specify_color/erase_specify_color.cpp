#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>




int main(int argc, char const* argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [imgpath]", argv[0]);
		return 1;
	}
	
	const char* img_path = argv[1];
	cv::Mat mat = cv::imread(img_path, cv::IMREAD_COLOR);


	cv::Mat mid;
	cv::cvtColor(mat, mid, cv::COLOR_BGR2HSV);
	cv::Mat dst;
	cv::inRange(mid, cv::Scalar(10, 0, 200), cv::Scalar(100, 20, 250), dst);
	
	cv::imshow("gray", dst);
	cv::imshow("before", mat);
	cv::Mat matResult=mat.clone();
	
	uint rows = matResult.rows /** mat.channels()*/;
	uint cols = matResult.cols;
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

			if(dst.at<uchar>(i, j) > 0)
			{
				const size_t N = 3;
				std::vector<uchar> vecR, vecG, vecB;
				for (int x = j - N; x < (j + N) && x < cols; ++x)
				{
					if (x < 0) continue;
					vecB.push_back(matResult.at<cv::Vec3b>(i, x)[0]);
					vecG.push_back(matResult.at<cv::Vec3b>(i, x)[1]);
					vecR.push_back(matResult.at<cv::Vec3b>(i, x)[2]);
				}

				
				uchar r = *std::max_element(vecR.begin(), vecR.end());
				uchar g = *std::max_element(vecG.begin(), vecG.end());
				uchar b = *std::max_element(vecB.begin(), vecB.end()) ;

				matResult.at<cv::Vec3b>(i, j) = { b, g, r };

			}
		}
	}

	cv::imshow("after", matResult);
	cv::waitKey();
	cv::imwrite(img_path, matResult);
	return 0;
}
