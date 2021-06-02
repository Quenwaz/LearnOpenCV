#include <cstdlib>
#include <iostream>
#include "opencv.hpp"
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>


// http://yann.lecun.com/exdb/mnist/
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
	const int K{ 3 };
	cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();
	knn->setDefaultK(K);
	knn->setIsClassifier(true);
	knn->setAlgorithmType(cv::ml::KNearest::BRUTE_FORCE);

	const std::string image_path{ "E:/GitCode/NN_Test/data/images/digit/handwriting_0_and_1/" };
	cv::Mat tmp = cv::imread(image_path + "0_1.jpg", 0);
	const int train_samples_number{ 40 }, predict_samples_number{ 40 };
	const int every_class_number{ 10 };

	cv::Mat train_data(train_samples_number, tmp.rows * tmp.cols, CV_32FC1);
	cv::Mat train_labels(train_samples_number, 1, CV_32FC1);
	float* p = (float*)train_labels.data;
	for (int i = 0; i < 4; ++i) {
		std::for_each(p + i * every_class_number, p + (i + 1) * every_class_number, [i](float& v) {v = (float)i; });
	}

	// train data
	for (int i = 0; i < 4; ++i) {
		static const std::vector<std::string> digit{ "0_", "1_", "2_", "3_" };
		static const std::string suffix{ ".jpg" };

		for (int j = 1; j <= every_class_number; ++j) {
			std::string image_name = image_path + digit[i] + std::to_string(j) + suffix;
			cv::Mat image = cv::imread(image_name, 0);
			// CHECK(!image.empty() && image.isContinuous());
			image.convertTo(image, CV_32FC1);

			image = image.reshape(0, 1);
			tmp = train_data.rowRange(i * every_class_number + j - 1, i * every_class_number + j);
			image.copyTo(tmp);
		}
	}

	knn->train(train_data, cv::ml::ROW_SAMPLE, train_labels);

	// predict datta
	cv::Mat predict_data(predict_samples_number, tmp.rows * tmp.cols, CV_32FC1);
	for (int i = 0; i < 4; ++i) {
		static const std::vector<std::string> digit{ "0_", "1_", "2_", "3_" };
		static const std::string suffix{ ".jpg" };

		for (int j = 11; j <= every_class_number + 10; ++j) {
			std::string image_name = image_path + digit[i] + std::to_string(j) + suffix;
			cv::Mat image = cv::imread(image_name, 0);
			// CHECK(!image.empty() && image.isContinuous());
			image.convertTo(image, CV_32FC1);

			image = image.reshape(0, 1);
			tmp = predict_data.rowRange(i * every_class_number + j - 10 - 1, i * every_class_number + j - 10);
			image.copyTo(tmp);
		}
	}

	cv::Mat result;
	knn->findNearest(predict_data, K, result);
	// CHECK(result.rows == predict_samples_number);

	cv::Mat predict_labels(predict_samples_number, 1, CV_32FC1);
	p = (float*)predict_labels.data;
	for (int i = 0; i < 4; ++i) {
		std::for_each(p + i * every_class_number, p + (i + 1) * every_class_number, [i](float& v) {v = (float)i; });
	}

	int count{ 0 };
	for (int i = 0; i < predict_samples_number; ++i) {
		float value1 = ((float*)predict_labels.data)[i];
		float value2 = ((float*)result.data)[i];
		fprintf(stdout, "expected value: %f, actual value: %f\n", value1, value2);

		if (int(value1) == int(value2)) ++count;
	}
	fprintf(stdout, "when K = %d, accuracy: %f\n", K, count * 1.f / predict_samples_number);

	return 0;

}
