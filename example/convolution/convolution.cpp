#include <cstdlib>
#include <opencv2/opencv.hpp>


void show_mat(const char* wintitle, cv::Mat img)
{
	cv::namedWindow(wintitle);
	cv::imshow(wintitle, img);
}

cv::Mat generate_histogram(const cv::Mat& image){

    // 计算直方图
    cv::Mat hist;
    int histSize = 256; // 直方图尺寸
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

    // 创建直方图图像
    int hist_w = histSize * 3, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(255, 255, 255));

    // 归一化直方图数据
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    // 绘制直方图
    for (int i = 1; i < histSize; i++) {
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
                 cv::Scalar(100, 100, 100), 1, 8, 0);
    }

	const auto offset = 20;
	cv::Scalar color(255, 255, 255); // 白色
	cv::Mat image_with_border;
	cv::copyMakeBorder(histImage, image_with_border, offset, offset, offset, offset, cv::BORDER_CONSTANT, color);
	cv::rectangle(image_with_border,cv::Rect(offset, offset, hist_w,hist_h), cv::Scalar(200, 200, 200), 1, 8, 0);


    return image_with_border;
}


int main(int argc, char* argv[])
{
	try
	{
		cv::Mat src = cv::imread(argv[1], cv::IMREAD_COLOR);
		show_mat("src",src);
		 cv::Mat dst(src.rows/2, src.cols/2, CV_8UC3);
		for (size_t i = 0; i < src.rows; i+=2)
		{
			for (size_t j = 0; j < src.cols; j+=2)
			{
				dst.at<cv::Vec3b>(i/2, j/2) = src.at<cv::Vec3b>(i, j);
			}
		}
		cv::resize(dst, dst, cv::Size(src.cols,src.rows));
		show_mat("dst",dst);
		cv::waitKey(0);

	}
	catch (const std::exception& e)
	{
		fprintf(stderr, "error %s\n", e.what());
	}
	getchar();
	return EXIT_SUCCESS;
}
