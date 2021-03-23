// Classification.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <set>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


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

#include <queue>

std::deque<bool> Fingerprint(const char* pszImg)
{
	cv::Mat input = cv::imread(pszImg);

	const auto nMaxinput = std::max(input.rows, input.cols);
	auto normal_row = 256;
	if (nMaxinput <= normal_row) normal_row = 32;
	else normal_row = 64;
	const auto normal_col = normal_row + 1;

	cv::Mat matOfResize;
	if (input.rows != normal_row || input.cols != normal_col)
	{
		matOfResize = cv::Mat::zeros(normal_row, normal_col, input.type());
		cv::Size resize(normal_col, normal_row);
		if (input.rows > input.cols)
		{
			resize = {
				static_cast<int>(std::round((static_cast<double>(input.cols) / input.rows) * normal_col)), normal_row
			};
		}
		else if (input.rows < input.cols)
		{
			resize = {
				normal_col, static_cast<int>(std::round((static_cast<double>(input.rows) / input.cols) * normal_row))
			};
		}

		cv::resize(input, input, resize, cv::INTER_NEAREST);

		if (input.rows == normal_row)
		{
			input.copyTo(matOfResize(cv::Rect((matOfResize.cols - input.cols) * 0.5, 0, input.cols, input.rows)));
		}
		else if (input.cols == normal_col)
		{
			input.copyTo(matOfResize(cv::Rect(0, (matOfResize.rows - input.rows) * 0.5, input.cols, input.rows)));
		}
		else
			input.copyTo(matOfResize);
	}
	else matOfResize = input;

	auto matOfGray = ToGray(matOfResize);

	std::deque<bool> result(normal_row * normal_row, false);
	for (int i = 0; i < (matOfGray.rows); ++i)
		for (int j = 0; j < (matOfGray.cols - 1); ++j)
		{
			const auto diff = matOfGray(i, j + 1) - matOfGray(i, j);
			if (diff > 0)
			{
				result[normal_row * i + j] = true;
			}
		}

	return result;
}

double CompareFingerprint(const std::deque<bool>& f1, const std::deque<bool>& f2)
{
	assert(f1.size() == f2.size());

	size_t nMatched = 0;
	for (size_t i = 0; i < f1.size(); ++i)
	{
		if (f1[i] == f2[i]) ++nMatched;
	}

	return static_cast<double>(nMatched) / f1.size();
}


int fingerprint(cv::Mat src, cv::Mat* hash)
{
	try
	{
		resize(src, src, cv::Size(8, 8));
		src.convertTo(src, CV_32F);
		cv::Mat srcDCT;
		dct(src, srcDCT);
		srcDCT = abs(srcDCT);
		double sum = 0;
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				sum += srcDCT.at<float>(i, j);

		double average = sum / 64;
		cv::Mat phashcode = cv::Mat::zeros(cv::Size(8, 8), CV_8U);
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				phashcode.at<char>(i, j) = srcDCT.at<float>(i, j) > average ? 1 : 0;

		*hash = phashcode.reshape(0, 1).clone();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}

std::string pHashValue(cv::Mat& src)
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


std::string GetDiffHash(cv::Mat src)
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


#include <io.h>

#include <vector>

struct FileInfo
{
	std::string strPath{ "" };
	std::string strFileName{ "" };
	FileInfo(const char* pszRoot, const char* pszName) : strPath(pszRoot), strFileName(pszName) {}

	bool operator<(const FileInfo& rhs)const
	{
		return this->strFileName < rhs.strFileName;
	}
};

size_t listFiles(const char* dir, std::vector<FileInfo>& vecFiles)
{
	char dirNew[260] = { 0 };
	strcpy_s(dirNew, dir);
	strcat_s(dirNew, "/*.jpg"); // 在目录后面加上"\\*.*"进行第一次搜索

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1) // 检查是否成功
		return 0;

	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			strcpy_s(dirNew, dir);
			strcat_s(dirNew, "/");
			strcat_s(dirNew, findData.name);

			listFiles(dirNew, vecFiles);
		}
		else
		{
			// strcpy_s(dirNew, dir);
			// strcat_s(dirNew, "/");
			// strcat_s(dirNew, findData.name);
			vecFiles.emplace_back(dir, findData.name);
		}
	} 	while (_findnext(handle, &findData) == 0);

	_findclose(handle); // 关闭搜索句柄
	return vecFiles.size();
}

#include <fstream>
int CopyFile(const char* pszSrc, const char* pszNewFile)
{
	try
	{
		std::ifstream in;
		in.open(pszSrc, std::ios::binary);
		if (in.fail())
		{
			std::cout << "Error 1: Fail to open the source file." << std::endl;
			in.close();
			return 0;
		}

		std::ofstream out;
		out.open(pszNewFile, std::ios::binary);
		if (out.fail())
		{
			std::cout << "Error 2: Fail to create the new file." << std::endl;
			out.close();
			in.close();
			return 0;
		}

		out << in.rdbuf();
		out.close();
		in.close();
		return 1;
	}
	catch (std::exception e)
	{
	}
}


#include <direct.h>

bool create_folder(const char* pszPath)
{
	if (_access(pszPath, 0) == -1)
	{
		//如果文件夹不存在
		_mkdir(pszPath);
	}

	return true;

}

void classification(const std::vector<FileInfo>& vecFiles)
{
	if (vecFiles.size() < 2)
	{
		return;
	}

	std::vector<std::pair<FileInfo, std::string>> vecFileFeature;
	for (auto file : vecFiles)
	{
		auto mat = cv::imread(file.strPath + "/" + file.strFileName);
		vecFileFeature.emplace_back(file, GetDiffHash(mat));
	}


	std::deque<bool> queInvalid(vecFileFeature.size(), false);
	std::vector<std::set<FileInfo>> vecResult;
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

			create_folder(dst.c_str());

			dst += "/";
			dst += file.strFileName;

			CopyFile((file.strPath + "/" + file.strFileName).c_str(), dst.c_str());
		}
	}
}



int main(int argc, char* argv[])
{
	std::vector<FileInfo> vecFiles;
	auto f = listFiles(R"(H:\template)", vecFiles);
	classification(vecFiles);

	return EXIT_SUCCESS;
	cv::Mat matf1 = cv::imread(R"(C:\Users\232323\Desktop\test1.jpg)");
	// ,matf2 = cv::imread(R"(C:\Users\232323\Desktop\test4.jpg)");

	auto f3 = GetDiffHash(matf1);
	auto f1_ = pHashValue(matf1);
	// auto f2_ = pHashValue(matf2);

	// auto re = HanmingDistance(f1_, f2_);

	auto f1 = Fingerprint(R"(C:\Users\232323\Desktop\test1.jpg)");
	// auto f2 = Fingerprint(R"(C:\Users\232323\Desktop\test4.jpg)");
	//
	// std::cout << "[";
	// for (auto f : f1) std::cout << f << ",";
	// std::cout << "]"<<std::endl;


	// const auto res = CompareFingerprint(f1, f2);
	// std::cout << "result:" << res << std::endl;

	cv::waitKey(0);
	return 0;
}
