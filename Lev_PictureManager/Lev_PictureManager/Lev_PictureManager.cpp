// Lev_PictureManager.cpp: 定义应用程序的入口点。
//

#include <iostream>
// TODO: 在此处引用程序需要的其他标头。
#include "filesystem"
#include "opencv2/opencv.hpp"
#include "Lev_PictureManager.h"
using namespace std;
namespace fs = std::filesystem;
//使用命名空间是一种美德:D
namespace Lev_PictureManager {
	bool isImageFileExtend(const std::string extension){
		// 在这里添加你需要支持的图片格式的判断条件
		return extension == ".jpg" || extension == ".png" || extension == ".jpeg";
	}

	bool ResizeSingleImage(const std::string& insertPath, const std::string& outputPath, int tar_width, int tar_height) {
		cv::Mat originalImage = cv::imread(cv::String(insertPath), cv::IMREAD_COLOR);
		cv::String outString = cv::String(outputPath);
		if (originalImage.empty()) {
			std::cerr << "Failed to open image!" << std::endl;
			return false;
		}

		// 获取原始图像的宽高
		int originalWidth = originalImage.cols;
		int originalHeight = originalImage.rows;

		// 计算缩放比例
		double scaleWidth = static_cast<double>(tar_width) / originalWidth;
		double scaleHeight = static_cast<double>(tar_height) / originalHeight;

		// 使用 resize 函数进行缩放
		cv::Mat resizedImage;
		cv::resize(originalImage, resizedImage, cv::Size(), scaleWidth, scaleHeight, cv::INTER_LINEAR);

		// 保存压缩后的图像
		if (!cv::imwrite(outString, resizedImage)) return false;
		return true;
	}


	
	std::vector<cv::Mat> splitImage(const cv::Mat& inputImage, int rows, int cols) {
		std::vector<cv::Mat> subImages;

		int subImageWidth = inputImage.cols / cols;
		int subImageHeight = inputImage.rows / rows;

		for (int y = 0; y < rows; ++y) {
			for (int x = 0; x < cols; ++x) {
				// 定义矩形区域，切分图像
				cv::Rect roi(x * subImageWidth, y * subImageHeight, subImageWidth, subImageHeight);

				// 获取子图像
				cv::Mat subImage = inputImage(roi).clone();

				// 添加到结果集
				subImages.push_back(subImage);
			}
		}

		return subImages;
	}
	bool TearPicture(const std::string& insert_picture, const std::string& output_path, int row, int col, const std::string& suffix)
	{
		//判断输入图片是否是支持的文件
		fs::path insertPath(insert_picture);
		if(!fs::is_regular_file(insertPath)) return false;
		if(!isImageFileExtend(insertPath.extension().string())) return false;

		//不带后缀的后缀名
		std::string insert_picture_name = insertPath.stem().string();

		//将图片切分成vector
		cv::Mat mat_insert = cv::imread(insert_picture);

		std::vector<cv::Mat> vec_ret = splitImage(mat_insert, row, col);

		//将vector中的图片保存到指定路径
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j) {
				std::string output_name = output_path + "/" + insert_picture_name + "_" + std::to_string(i) + "-" + std::to_string(j) + "." + suffix;
				if (!cv::imwrite(output_name, vec_ret[i + j])) return false;
			}
		}

		return true;
	}

	cv::Mat enhanceBrightnessImage(const cv::Mat& image) {
		// 转换为YUV颜色空间
		cv::Mat imgYUV;
		cv::cvtColor(image, imgYUV, cv::COLOR_BGR2YUV);

		// 应用CLAHE到Y通道(YUV中的Y代表亮度)
		cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
		clahe->apply(0, imgYUV);  // 修正此行代码

		// 将结果转换回BGR格式
		cv::Mat imgOutput;
		cv::cvtColor(imgYUV, imgOutput, cv::COLOR_YUV2BGR);

		return imgOutput;
	}

	bool adjustBrightness(const std::string& input_picture, const std::string& output_path)
	{
		//判断输入图片是否是支持的文件
		fs::path insertPath(input_picture);
		if (!fs::is_regular_file(insertPath)) return false;
		if (!isImageFileExtend(insertPath.extension().string())) return false;

		std::string input_filename = insertPath.filename().string();
		std::string output_filename = output_path + "/" + input_filename;
		cv::Mat mat_input = cv::imread(input_picture);
		cv::Mat mat_output = enhanceBrightnessImage(mat_input);

		if (!cv::imwrite(output_filename, mat_output)) return false;
		return true;
	}


	
}
