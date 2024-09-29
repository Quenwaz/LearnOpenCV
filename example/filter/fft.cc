#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// 函数声明
void shiftDFT(Mat& mag);
Mat createButterLowpassFilter(Mat& dft_Filter, int D);
Mat createButterHighpassFilter(Mat& dft_Filter, int D);

void show_mat(const char* wintitle, cv::Mat img)
{
	cv::namedWindow(wintitle);
	cv::imshow(wintitle, img);
}

int main(int argc, char const *argv[]){
    // 读取图像
    Mat image = imread(argv[1], IMREAD_GRAYSCALE);
    if (image.empty()) {
        cout << "Error opening image" << endl;
        return -1;
    }

    // 优化图像大小，为2，3，5的倍数时DFT性能最佳
    Mat padded;
    int m = getOptimalDFTSize(image.rows);
    int n = getOptimalDFTSize(image.cols);
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

    // 创建复数矩阵
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);

    // 执行DFT
    dft(complexI, complexI);

    // 计算幅度谱
    split(complexI, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat magI = planes[0];

    // 对数变换
    magI += Scalar::all(1);
    log(magI, magI);

    // 剪切和重新分布幅度图象限
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
    shiftDFT(magI);

    // 归一化
    normalize(magI, magI, 0, 1, NORM_MINMAX);

    // 创建低通滤波器
    Mat lowpassFilter = createButterLowpassFilter(magI, 30);

    // 创建高通滤波器
    Mat highpassFilter = createButterHighpassFilter(magI, 30);

    // 应用低通滤波器
    Mat lowpassResult;
    mulSpectrums(complexI, lowpassFilter, lowpassResult, 0);

    // 应用高通滤波器
    Mat highpassResult;
    mulSpectrums(complexI, highpassFilter, highpassResult, 0);

    // 执行反DFT
    Mat invDFT_low, invDFT_high;
    idft(lowpassResult, invDFT_low, DFT_SCALE | DFT_REAL_OUTPUT);
    idft(highpassResult, invDFT_high, DFT_SCALE | DFT_REAL_OUTPUT);

    // 归一化结果
    normalize(invDFT_low, invDFT_low, 0, 1, NORM_MINMAX);
    normalize(invDFT_high, invDFT_high, 0, 1, NORM_MINMAX);

    // 显示结果
    show_mat("Original Image", image);
    show_mat("Magnitude Spectrum", magI);
    show_mat("Lowpass Filter Result", invDFT_low);
    show_mat("Highpass Filter Result", invDFT_high);
    waitKey();

    return 0;
}

void shiftDFT(Mat& mag) {
    int cx = mag.cols / 2;
    int cy = mag.rows / 2;

    Mat q0(mag, Rect(0, 0, cx, cy));
    Mat q1(mag, Rect(cx, 0, cx, cy));
    Mat q2(mag, Rect(0, cy, cx, cy));
    Mat q3(mag, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

Mat createButterLowpassFilter(Mat& dft_Filter, int D) {
    Mat filter = Mat(dft_Filter.size(), CV_32F);
    Point center = Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
    double radius;

    for (int i = 0; i < dft_Filter.rows; i++) {
        for (int j = 0; j < dft_Filter.cols; j++) {
            radius = sqrt(pow(i - center.x, 2.0) + pow(j - center.y, 2.0));
            filter.at<float>(i, j) = 1 / (1 + pow(radius / D, 4));
        }
    }

    Mat toMerge[] = {filter, filter};
    Mat butterLowpassFilter;
    merge(toMerge, 2, butterLowpassFilter);

    return butterLowpassFilter;
}

Mat createButterHighpassFilter(Mat& dft_Filter, int D) {
    Mat filter = Mat(dft_Filter.size(), CV_32F);
    Point center = Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
    double radius;

    for (int i = 0; i < dft_Filter.rows; i++) {
        for (int j = 0; j < dft_Filter.cols; j++) {
            radius = sqrt(pow(i - center.x, 2.0) + pow(j - center.y, 2.0));
            filter.at<float>(i, j) = 1 / (1 + pow(D / radius, 4));
        }
    }

    Mat toMerge[] = {filter, filter};
    Mat butterHighpassFilter;
    merge(toMerge, 2, butterHighpassFilter);

    return butterHighpassFilter;
}