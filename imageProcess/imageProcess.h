#include <opencv2/opencv.hpp>
#include <AFEPack/HGeometry.h>
#include <iostream>


cv::Mat convertToGrey(cv::Mat input_image) {
  // 创建灰度图像
  cv::Mat grayImage;
  cv::cvtColor(input_image, grayImage, cv::COLOR_BGR2GRAY);
  // 计算正方形尺寸
  int imageWidth = grayImage.cols;
  int imageHeight = grayImage.rows;
  int squareSize = std::max(imageWidth, imageHeight);

  // 创建一个全白的正方形图像
  cv::Mat squareImage(squareSize, squareSize, CV_8UC1, cv::Scalar(255));

  // 计算原图像的放置位置（中心对齐）
  int xOffset = (squareSize - imageWidth) / 2;
  int yOffset = (squareSize - imageHeight) / 2;

  // 将原图像复制到正方形图像上
  grayImage.copyTo(squareImage(cv::Rect(xOffset, yOffset, imageWidth, imageHeight)));

  return squareImage;
}

bool ifRefine(int grayValue, int centerValue) {
  return (grayValue == centerValue) ;
}

int getGrayValueAt(cv::Mat& grayImage, double x, double y) {
    // 获取图像尺寸
    int N = grayImage.rows; // 假设灰度图是方形的

    // 将 (x, y) 映射到像素索引 (i, j)
    int i = std::floor(x * (N - 1)); // x 对应的行索引
    int j = std::floor(y * (N - 1)); // y 对应的列索引

    // 获取对应像素的灰度值
    return grayImage.at<uchar>(i, j);
}