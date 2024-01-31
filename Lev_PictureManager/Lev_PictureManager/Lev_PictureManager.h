// Lev_PictureManager.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once



namespace Lev_PictureManager {
	/// <summary>
	/// 判断文件后缀是否为支持的图片
	/// </summary>
	/// <param name="extension">传入后缀，支持输入.jpg .png .jpeg</param>
	/// <returns>是否支持</returns>
	bool isImageFileExtend(const std::string extension);
	/// <summary>
	/// 缩放图片大小到指定大小
	/// </summary>
	/// <param name="insertPath">输入图片的绝对路径，包括图片名称</param>
	/// <param name="outputPath">输出图片的绝对路径，包括图片名称</param>
	/// <param name="tar_width">目标缩放宽度</param>
	/// <param name="tar_height">目标缩放高度</param>
	/// <returns>成功与否</returns>
	bool ResizeSingleImage(const std::string& insert_picture, const std::string& output_path, int tar_width, int tar_height);
	
	/// <summary>
	/// 将指定图片路径输入，导到指定目录中去
	/// </summary>
	/// <param name="insert_picture">输入图片路径</param>
	/// <param name="output_path">输出分裂图片的路径</param>
	/// <param name="row">指定切分行数</param>
	/// <param name="col">指定切分列数</param>
	/// <param name="suffix">后缀，默认是_，不同的图片会在指定后缀后，加上row-col的后缀比如 test_1-12.jpeg</param>
	/// <returns>是否切分成功</returns>
	bool TearPicture(const std::string& insert_picture, const std::string& output_path, int row, int col,const std::string& suffix);

	/// <summary>
	/// 调整图片亮度到最佳
	/// </summary>
	/// <param name="input_picture">输入图片的路径</param>
	/// <param name="output_path">图片输出路径</param>
	/// <returns></returns>
	bool adjustBrightness(const std::string& input_picture, const std::string& output_path);

}