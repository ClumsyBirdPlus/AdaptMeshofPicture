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

  // 对结果向量进行排序
  std::sort(result.begin(), result.end(), std::greater<int>());
  return result;
}


cv::Mat kmeansSegmentation(const cv::Mat& input_image, int k) {
    // 将图像转换为 2D 数组，每一行是一个像素的 RGB 值
    cv::Mat data;
    input_image.convertTo(data, CV_32F); // 转换为 float 类型以便 K-means 处理
    data = data.reshape(1, input_image.total()); // 将图像展平成一维数据

    // 设置 K-means 聚类的停止准则
    cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 50, 1.0);

    // K-means 输出
    cv::Mat labels, centers;

    // 执行 K-means 聚类
    cv::kmeans(data, k, labels, criteria, 5, cv::KMEANS_RANDOM_CENTERS, centers);

    // 将中心点转换回 uint8 格式
    centers = centers.reshape(3, centers.rows);
    centers.convertTo(centers, CV_8U);

    // 根据聚类结果重新生成图像
    cv::Mat segmented_image(input_image.size(), input_image.type());
    for (int i = 0; i < input_image.total(); ++i) {
        segmented_image.at<cv::Vec3b>(i / input_image.cols, i % input_image.cols) = centers.at<cv::Vec3b>(labels.at<int>(i));
    }

    return segmented_image;
}