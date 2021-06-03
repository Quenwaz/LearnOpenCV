#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>
#include <numeric>


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
	const char* img_path = R"(C:\Users\Quenwaz\Desktop\TCP_Stop.png)";
	cv::Mat mat = cv::imread(img_path, cv::IMREAD_COLOR);

	// lookup_rgb_channels(mat);
	// lookup_hsv_channels(mat);

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
