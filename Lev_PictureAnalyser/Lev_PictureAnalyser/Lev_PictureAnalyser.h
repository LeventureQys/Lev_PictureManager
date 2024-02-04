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
	/// 将指定图片路径输入，按照行列切分导到指定目录中去
	/// </summary>
	/// <param name="insert_picture">输入图片路径</param>
	/// <param name="output_path">输出分裂图片的路径</param>
	/// <param name="row">指定切分行数</param>
	/// <param name="col">指定切分列数</param>
	/// <returns>是否切分成功</returns>
	bool TearPicture(const std::string& insert_picture, const std::string& output_path, int row, int col);

	/// <summary>
	/// 输入指定的json文件，并按照行列切分，导入到指定的文件夹下去
	/// </summary>
	/// <param name="insert_json">输入json文件的路径</param>
	/// <param name="output_path">输出分裂json的路径</param>
	/// <param name="row">行</param>
	/// <param name="col">列</param>
	/// <returns>是否成功</returns>
	bool TearDataJson(const std::string& insert_json, const std::string& output_path, int row, int col);

	/// <summary>
	/// 调整图片亮度到最佳
	/// </summary>
	/// <param name="input_picture">输入图片的路径</param>
	/// <param name="output_path">图片输出路径</param>
	/// <returns></returns>
	bool adjustBrightness(const std::string& input_picture, const std::string& output_path);

	/// <summary>
	/// 此接口会主动在input目录下找到所有的带json文件的图片，并尝试将其转换成labelme格式的json文件
	/// </summary>
	/// <param name="input_path"></param>
	/// <param name="output_path"></param>
	/// <returns></returns>
	bool ReformDataset(const std::string& input_path, const std::string& output_path);

	/// <summary>
	/// 输入一个单独的图片，一个json文件，转换到指定目录下
	/// </summary>
	/// <param name="insert_img"></param>
	/// <param name="insert_json"></param>
	/// <param name="output_path"></param>
	/// <returns></returns>
	bool TSDToLabelme_Single(const std::string& insert_img, const std::string& insert_json, const std::string& output_path, bool random_suffix);

	/// <summary>
	/// 删除指定目录下不存在标记点的图片和json组合
	/// </summary>
	/// <param name="input_path"></param>
	void DeleteEmptyJsonCombine(const std::string& input_path);
	/// <summary>
	/// 删除没有json组合的图像
	/// </summary>
	/// <param name="path">处理路径</param>
	/// <param name="image_suffix">图片的后缀名</param>
	void DeleteNoneJsonImages(const std::string& path, const std::string& image_suffix);

	/// <summary>
	/// 删除没有点坐标的LabelMe数据
	/// </summary>
	/// <param name="input_path"></param>
	void DeleteNoShapeLabelMeData(const std::string& input_path);

}