#include <opencv2/opencv.hpp>
#include <iostream>

// 将给定图片转换为正方形，使用左上角的颜色填充
cv::Mat convertToSquare(cv::Mat input_image) {
    // 获取输入图像的宽度和高度
    int width = input_image.cols;
    int height = input_image.rows;

    // 获取左上角像素的颜色 (BGR)
    cv::Vec3b topLeftColor = input_image.at<cv::Vec3b>(0, 0);

    // 如果图像已经是正方形，直接返回
    if (width == height) {
        return input_image;
    }

    // 计算目标正方形的尺寸（取宽度和高度中的较大值）
    int squareSize = std::max(width, height);

    // 创建一个正方形图像，并用左上角的颜色填充
    cv::Mat squareImage(squareSize, squareSize, input_image.type(), topLeftColor);

    // 计算要将原图放置在正方形图像中的哪个区域
    int x_offset = (squareSize - width) / 2;
    int y_offset = (squareSize - height) / 2;

    // 将原图复制到正方形图像的中心
    input_image.copyTo(squareImage(cv::Rect(x_offset, y_offset, width, height)));

    return squareImage;
}

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