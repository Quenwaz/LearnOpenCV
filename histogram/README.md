# 图像容器
首先， 我们需要了解一个概念。 所有的数字图像在计算机中的表现形式为一个矩阵和矩阵描述信息， 矩阵中存储着每一点的像素值(0~255)。OpenCV 是一个计算机视觉库，即图像处理库。 主要用于操纵图像信息。 
所以第一步， 我们需要了解OpenCV是如何存储和处理图片的。

## Mat
早期的OpenCV是基于C开发的， 图像容器使用的是IplImage。 因为C的特性， 手动管理内存存在许多问题。 OpenCV 2.0 开始利用C++重写。 此时引入了Mat。
Mat 是一个图像容器， 主要由两部分组成:
- 矩阵头
	包括矩阵的大小，存储的方法以及矩阵地址。 矩阵头数据是常量。
- 矩阵数据指针
	像素存储在矩阵中

为了函数调用过程中传递Mat更高效， OpenCV使用了引用计数技术。这项技术主要是讲矩阵数据共享， 各自拥有其不同的矩阵头数据。 拷贝赋值操作仅仅拷贝矩阵头信息， 数据拷贝其指针而非数据本身(即所谓的浅拷贝)。如下代码即为浅拷贝:
```c++
Mat A, C;									// creates just the header parts
A = imread(argv[1], IMREAD_COLOR);			// here we'll know the method used (allocate matrix)
Mat B(A);									// Use the copy constructor
C = A;     
```
同时， Mat还可以仅仅共享部分矩阵信息ROI。如下:
```C++

```
以上任何操作像素矩阵的Mat对象， 都会影响到其他对象的数据。那么如何进行深拷贝呢? OpenCV提供了方法cv::Mat::clone()和cv::Mat::copyTo()用于深拷贝。 