#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>


struct ChooseData
{
	cv::Mat* image;
	std::function<void(const cv::Rect&)> fnChoosed;
};

void on_MouseHandle(int event, int x, int y, int flags, void* param)
{
	ChooseData choosedata = *(ChooseData*)param;
	static cv::Point2i ptclick(-1, -1), ptclick_last;
	switch (event)
	{
	case cv::EVENT_MOUSEMOVE:
	{
		if (ptclick.x > 0 && ptclick.y > 0)
		{
			auto image_copy = choosedata.image->clone();
			rectangle(image_copy, ptclick, cv::Point(x, y), cv::Scalar(0, 255, 0));
			cv::imshow("image", image_copy);
		}
	}
	break;
	case cv::EVENT_LBUTTONDOWN:
	{
		ptclick.x = x;
		ptclick.y = y;
		ptclick_last = ptclick;
	}
	break;
	case cv::EVENT_LBUTTONUP:
	{
		ptclick_last.x = x;
		ptclick_last.y = y;


		choosedata.fnChoosed(cv::Rect(ptclick, ptclick_last));
		ptclick.x = -1;
		ptclick.y = -1;
	}
	break;
	}
}



int main(int argc, char const* argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [imgpath]", argv[0]);
		return 1;
	}

	const char* img_path = argv[1];
	cv::Mat mat = cv::imread(img_path, cv::IMREAD_GRAYSCALE);


	cv::resize(mat, mat, cv::Size(0, 0), 0.3, 0.3);
	cv::imshow("image", mat);
	ChooseData choosedata{ &mat, [&mat](const cv::Rect& roi)
	{
		cv::Rect mat_rect(0, 0, mat.cols, mat.rows);
		if (!mat_rect.contains(roi.tl()) || !mat_rect.contains(roi.br()))
		{
			return;
		}
		
		auto mat_clone = mat.clone();
		auto mat_roi = cv::Mat(mat_clone, roi);
		cv::threshold(mat_roi, mat_roi, 160, 255, cv::THRESH_BINARY_INV);
		cv::imshow("roi", mat_clone);

	} };
	cv::setMouseCallback("image", on_MouseHandle, (void*)&choosedata);
	cv::waitKey(0);
	return 0;
}