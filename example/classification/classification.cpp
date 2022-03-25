// Classification.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <set>

#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <queue>
#include "common/utils.h"




// https://blog.csdn.net/eternity1118_/article/details/51088019
// http://www.45fan.com/article.php?aid=19121136902391992037418759
// https://blog.csdn.net/zhu_hongji/article/details/81235643


cv::Mat1d ToGray(cv::Mat input)
{
	assert(input.type() == CV_8UC3);
	cv::Mat1d gray;
	try
	{
		cv::Mat3d matOfFlaot;
		input.convertTo(matOfFlaot, CV_64F);
		normalize(matOfFlaot, matOfFlaot, 1, 0, cv::NORM_MINMAX);

		gray = cv::Mat::zeros(matOfFlaot.rows, matOfFlaot.cols, CV_64FC1);

		for (int i = 0; i < matOfFlaot.rows; ++i)
			for (int j = 0; j < matOfFlaot.cols; ++j)
			{
				// Y = 0．3R + 0．59G + 0．11B
				const auto bgr = matOfFlaot(i, j);
				gray.at<double>(i, j) = bgr[2] * 0.3 + bgr[1] * 0.59 + 0.11 * bgr[0];
			}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return gray;
}


cv::Mat ResizeToSquare(cv::Mat input, const cv::Size& sizeOfDst, int interpolation = cv::INTER_LINEAR)
{
	cv::Mat matOfResize;
	if (input.rows != sizeOfDst.height || input.cols != sizeOfDst.width)
	{
		matOfResize = cv::Mat::zeros(sizeOfDst.height, sizeOfDst.width, input.type());
		cv::Size resize(sizeOfDst.width, sizeOfDst.height);
		if (input.rows > input.cols)
		{
			resize = {
				static_cast<int>(std::round((static_cast<double>(input.cols) / input.rows) * sizeOfDst.width)), sizeOfDst.height
			};
		}
		else if (input.rows < input.cols)
		{
			resize = {
				sizeOfDst.width, static_cast<int>(std::round((static_cast<double>(input.rows) / input.cols) * sizeOfDst.height))
			};
		}

		cv::resize(input, input, resize, 0, 0, interpolation);

		if (input.rows == sizeOfDst.height)
		{
			input.copyTo(matOfResize(cv::Rect((matOfResize.cols - input.cols) * 0.5, 0, input.cols, input.rows)));
		}
		else if (input.cols == sizeOfDst.width)
		{
			input.copyTo(matOfResize(cv::Rect(0, (matOfResize.rows - input.rows) * 0.5, input.cols, input.rows)));
		}
		else
			input.copyTo(matOfResize);
	}
	else matOfResize = input;

	return matOfResize;
}

namespace imghash
{

	std::string PerceptualHash(cv::Mat& src)
	{
		cv::Mat img, dst;
		std::string rst(64, '\0');
		double dIdex[64];
		double mean = 0.0;
		int k = 0;
		if (src.channels() == 3)
		{
			cvtColor(src, src, CV_BGR2GRAY);
			img = cv::Mat_<double>(src);
		}
		else
		{
			img = cv::Mat_<double>(src);
		}

		/* 第一步，缩放尺寸*/
		resize(img, img, cv::Size(8, 8));

		/* 第二步，离散余弦变换，DCT系数求取*/
		dct(img, dst);

		/* 第三步，求取DCT系数均值（左上角8*8区块的DCT系数）*/
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				dIdex[k] = dst.at<double>(i, j);
				mean += dst.at<double>(i, j) / 64;
				++k;
			}
		}

		/* 第四步，计算哈希值。*/
		for (int i = 0; i < 64; ++i)
		{
			if (dIdex[i] >= mean)
			{
				rst[i] = '1';
			}
			else
			{
				rst[i] = '0';
			}
		}
		return rst;
	}


	std::string DiffHash(cv::Mat src)
	{
		cv::cvtColor(src, src, CV_BGR2GRAY);

		bool bDebug = false;
		if (src.rows != src.cols)
		{
			bDebug = true;
		}
		src = ResizeToSquare(src, cv::Size(9, 8), cv::INTER_AREA);

		if (bDebug)
		{
			// cv::imshow("test", src);
			// cv::waitKey(0);
		}
		// cv::resize(src, src, cv::Size(9, 8), 0, 0, cv::INTER_AREA);
		assert(src.type() == CV_8UC1);
		std::string strRet(src.rows * src.rows, '0');
		for (int i = 0; i < (src.rows); ++i)
			for (int j = 0; j < (src.cols - 1); ++j)
			{
				const auto diff = src.at<uchar>(i, j + 1) - src.at<uchar>(i, j);
				if (diff > 0)
				{
					strRet[i * src.rows + j] = '1';
				}
			}
		return strRet;
	}
}


//汉明距离计算  
double HanmingDistance(std::string& str1, std::string& str2)
{
	if (str1.size() != str2.size())
		return -1;
	double difference = 0;
	for (int i = 0; i < 64; i++)
	{
		if (str1[i] == str2[i])
			difference++;
	}
	return difference / 64;
}



void classification(const std::vector<utilities::fs::FileInfo>& vecFiles)
{
	if (vecFiles.size() < 2)
	{
		return;
	}

	std::vector<std::pair<utilities::fs::FileInfo, std::string>> vecFileFeature;
	for (auto file : vecFiles)
	{
		auto mat = cv::imread(file.strPath + "/" + file.strFileName);
		vecFileFeature.emplace_back(file, imghash::DiffHash(mat));
	}


	std::deque<bool> queInvalid(vecFileFeature.size(), false);
	std::vector<std::set<utilities::fs::FileInfo>> vecResult;
	for (size_t i = 0; i < vecFileFeature.size(); ++i)
	{
		if (queInvalid[i]) continue;
		auto f1 = vecFileFeature.at(i);
		vecResult.push_back({ f1.first });
		for (size_t j = i + 1; j < vecFileFeature.size(); ++j)
		{
			if (queInvalid[j]) continue;
			auto f2 = vecFileFeature.at(j);
			auto res = HanmingDistance(f1.second, f2.second);
			if (res > 0.95)
			{
				queInvalid[j] = true;
				queInvalid[i] = true;
				vecResult.back().insert(f2.first);
			}
		}
	}

	for (size_t i = 0; i < vecResult.size(); ++i)
	{
		auto group = vecResult.at(i);

		std::string folder = "C";
		folder += std::to_string(i + 1);
		for (auto file : group)
		{
			std::string dst = file.strPath;
			dst += "/";
			dst += folder;

			utilities::fs::create_folder(dst.c_str());

			dst += "/";
			dst += file.strFileName;

			
			utilities::fs::copyfile((file.strPath + "/" + file.strFileName).c_str(), dst.c_str());
		}
	}
}



int main(int argc, char* argv[])
{
	
	std::vector<utilities::fs::FileInfo> vecFiles;
	auto f = utilities::fs::list_files(vecFiles, R"(H:\template)", "*.jpg");
	classification(vecFiles);

	cv::Mat matf1 = cv::imread(R"(C:\Users\232323\Desktop\test1.jpg)");
	// ,matf2 = cv::imread(R"(C:\Users\232323\Desktop\test4.jpg)");

	auto f3 = imghash::DiffHash(matf1);
	auto f1_ = imghash::PerceptualHash(matf1);
	// auto f2_ = pHashValue(matf2);

	// auto re = HanmingDistance(f1_, f2_);

	cv::waitKey(0);
	return 0;
}
