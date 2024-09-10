#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

cv::Mat detectFaces(const cv::Mat& image, const std::string& modelPath) {
    // 加载YuNet模型
    cv::Ptr<cv::FaceDetectorYN> faceDetector = cv::FaceDetectorYN::create(
        modelPath,  // 模型文件路径现在由参数指定
        "",
        cv::Size(320, 320),  // 输入大小
        0.9f,  // 置信度阈值
        0.3f,  // NMS阈值
        50  // 最大人脸数
    );

    // 设置输入大小
    faceDetector->setInputSize(image.size());

    // 检测人脸
    cv::Mat faces;
    faceDetector->detect(image, faces);

    // 在原图上绘制检测结果
    cv::Mat result = image.clone();

    for (int i = 0; i < faces.rows; i++) {
        // 提取人脸矩形框 (x, y, width, height)
        cv::Rect faceBox(
            faces.at<float>(i, 0),  // x
            faces.at<float>(i, 1),  // y
            faces.at<float>(i, 2),  // width
            faces.at<float>(i, 3)   // height
        );

        // 绘制人脸框
        cv::rectangle(result, faceBox, cv::Scalar(0, 255, 0), 2);

        // 提取五个关键点 (眼睛、鼻子、嘴巴)
        for (int j = 4; j < 14; j += 2) {
            // 关键点的 x 和 y 坐标
            float landmarkX = faces.at<float>(i, j);
            float landmarkY = faces.at<float>(i, j + 1);

            // 在图像上绘制关键点
            cv::circle(result, cv::Point(landmarkX, landmarkY), 3, cv::Scalar(0, 0, 255), -1);
        }
    }

    return result;
}