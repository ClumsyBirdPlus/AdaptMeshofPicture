#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

std::vector<int> kMeansGrayScaleCenters(cv::Mat& grayImage, int k) {
  std::vector<int> result;
  // 将灰度图像转换为 1D 矩阵，每个像素是一个样本
  cv::Mat samples(grayImage.rows * grayImage.cols, 1, CV_32F);
  for (int y = 0; y < grayImage.rows; y++) {
    for (int x = 0; x < grayImage.cols; x++) {
      samples.at<float>(y + x * grayImage.rows, 0) = static_cast<float>(grayImage.at<uchar>(y, x));
    }
  }
  // K-means 聚类参数
  cv::Mat labels; // 输出的类别标签
  cv::Mat centers; // 聚类中心
  // 执行 K-means 聚类
  cv::kmeans(samples, k, labels,
             cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0),
             3, cv::KMEANS_PP_CENTERS, centers);
  for (int i = 0; i < k; i++) {
    result.push_back(static_cast<int>(centers.at<float>(i, 0)));
  }

  for (int y = 0; y < grayImage.rows; y++) {
    for (int x = 0; x < grayImage.cols; x++) {
      int clusterIdx = labels.at<int>(y + x * grayImage.rows, 0);
      grayImage.at<uchar>(y, x) = static_cast<uchar>(centers.at<float>(clusterIdx, 0));
    }
  }

  // 对结果向量进行排序，从暗到亮（从小到大）
  std::sort(result.begin(), result.end());
  return result;
}
