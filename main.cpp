#include <AFEPack/HGeometry.h>
#include "./imageProcess/imageProcess.h"
#include "./kMeans/kMeans.h"


int main(int argc, char **argv)
{
  // 用户输入文件路径
  // std::cout << "Enter the path to the image file: ";
  // std::getline(std::cin, inputPath);
  // 读取图像
  std::string inputPath =
    "hu.png"
    // "zhang.jpg"
    // "wang.jpg"
    ;
  // 调参
  int totalLevel = 6;
  std::vector<int> refineTimes = {0,1,0,1,1,1,1};

  cv::Mat image_origin = cv::imread(inputPath, cv::IMREAD_COLOR);
  if (image_origin.empty()) {
    std::cerr << "Could not open or find the image!" << std::endl;
    return -1;
  }
  // 转为灰度图片
  cv::Mat squareImage = convertToGrey(image_origin);

  int imageSize = squareImage.cols;
  // // 压缩图像
  // int imageSize = 1024;
  // cv::resize(squareImage, squareImage, cv::Size(imageSize, imageSize), 0, 0, cv::INTER_LINEAR);

  // double minArea = MAX FLOAT;
  // cv::GaussianBlur(squareImage, squareImage, cv::Size(5, 5), 1.5);
  // cv::Mat histImage;
  // cv::equalizeHist(squareImage, histImage);

  auto grayCenters = kMeansGrayScaleCenters(squareImage, totalLevel);
  grayCenters.push_back(0);
  // // 查找图像中的轮廓
  // cv::Mat edges;
  // cv::Canny(histImage, edges, 100, 200);
  // std::vector<std::vector<cv::Point>> contours;
  // std::vector<cv::Vec4i> hierarchy;
  // cv::findContours(edges, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
  // // 在原图像上绘制轮廓
  // cv::drawContours(squareImage, contours, -1, cv::Scalar(0), 2); //边缘颜色

  // cv::imwrite("EdgeImage.png", edges);
  cv::imwrite("ClusteredImage.png", squareImage);

  // return 0;

  // 声明几何遗传树，并从 Easymesh 格式的文件中读入数据
  HGeometryTree<2> h_tree;
  h_tree.readEasyMesh("Coarse");

  /// 在背景网格上建立第一个非正则网格
  IrregularMesh<2> irregular_mesh(h_tree);
  // irregular_mesh.globalRefine(4);

  // 开始加密
  for (auto itTimes = refineTimes.begin(); itTimes != refineTimes.end(); ++itTimes) {
    // for (auto itLevel = grayCenters.begin()+1; itLevel != grayCenters.end(); ++itLevel) {
    // for (refineLevel = 3; refineLevel != totalLevel; ++refineLevel) {
    // for (refineLevel = totalLevel - 2; refineLevel != -1; --refineLevel) {
    // refineLevel = 0;
    // for (int i = 0; i < 2; ++i) {
    for (int count = 0; count < *itTimes; ++count) {
      /// 对非正则网格做半正则化和正则化
      irregular_mesh.semiregularize();
      irregular_mesh.regularize(false);

      /// 这就是通过正则化得到的网格
      RegularMesh<2>& regular_mesh = irregular_mesh.regularMesh();

      Indicator<2> indicator(regular_mesh);
      for (int i = 0;i < regular_mesh.n_geometry(2);i ++) {
        /// 这是三角形的三个顶点。对于三角形和双生三角形都是这样的。
        Point<2>& p0 = regular_mesh.point(regular_mesh.geometry(2,i).vertex(0));
        Point<2>& p1 = regular_mesh.point(regular_mesh.geometry(2,i).vertex(1));
        Point<2>& p2 = regular_mesh.point(regular_mesh.geometry(2,i).vertex(2));

        /// 这是三角形的重心
        Point<2> p((p0[0] + p1[0] + p2[0])/3., (p0[1] + p1[1] + p2[1])/3.);

        /// 手工计算三角形的面积
        double area = ((p1[0] - p0[0])*(p2[1] - p0[1]) -
                       (p2[0] - p0[0])*(p1[1] - p0[1]));
        if (
            ifRefine(getGrayValueAt(squareImage,p[0],p[1]), grayCenters[itTimes - refineTimes.begin()])
            )
          { /// 在环状区域中设置指示子
            indicator[i] = area;
          }
        // minArea =(minArea < area ? minArea : area);
      }
      /// 下面的几行调用进行自适应的函数，都是套话。
      MeshAdaptor<2> mesh_adaptor(irregular_mesh);
      mesh_adaptor.convergenceOrder() = 0.; /// 设置收敛阶为0
      mesh_adaptor.refineStep() = 1; /// 最多允许加密一步
      mesh_adaptor.setIndicator(indicator);
      mesh_adaptor.is_refine_only() = true;
      mesh_adaptor.tolerence() = 2e-6; /// 自适应的忍量
      mesh_adaptor.adapt(); /// 完成自适应
    }
  };
  RegularMesh<2>& regular_mesh = irregular_mesh.regularMesh();
  regular_mesh.writeOpenDXData("result.dx");
  std::cout << "Successful Output" << "\n";

  return 0;
}