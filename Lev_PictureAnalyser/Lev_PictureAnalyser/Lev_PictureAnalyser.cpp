// Lev_PictureManager.cpp: 定义应用程序的入口点。
//

#include <iostream>
// TODO: 在此处引用程序需要的其他标头。
#include "filesystem"
#include "opencv2/opencv.hpp"
#include "Lev_PictureAnalyser.h"
#include "qstring.h"
#include "qlist.h"
#include "qbytearray.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qfileinfo.h"
#include "qfile.h"
#include "qpoint.h"
#include "qdir.h"
#include "qimage.h"
#include "qbuffer.h"
#include "qrandom.h"
#pragma execution_character_set("utf-8") 

using namespace std;
namespace fs = std::filesystem;
//使用命名空间是一种美德:D
namespace Lev_PictureManager {

	int image_width = 5120;
	int image_height = 5120;
	struct Det_Obj {
		QString json_path = "";
		QString flags = "";
		QList<QPoint> list_points;
	};

	//子图片的空间表示
	struct subImage {
		QPoint lt;
		double width = 0.00;
		double height = 0.00;
	};

	bool isImageFileExtend(const std::string extension) {
		// 在这里添加你需要支持的图片格式的判断条件
		return extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp";
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
		std::vector<std::vector<cv::Mat>> matrix;
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
	bool TearPicture(const std::string& insert_picture, const std::string& output_path, int row, int col)
	{
		//判断输入图片是否是支持的文件
		fs::path insertPath(insert_picture);
		if (!fs::is_regular_file(insertPath)) return false;
		if (!isImageFileExtend(insertPath.extension().string())) return false;

		//不带后缀的后缀名
		std::string insert_picture_name = insertPath.stem().string();

		//将图片切分成vector
		cv::Mat mat_insert = cv::imread(insert_picture);

		std::vector<cv::Mat> vec_ret = splitImage(mat_insert, row, col);

		//将vector中的图片保存到指定路径
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j) {
				std::string output_name = output_path + "/" + insert_picture_name + "_" + std::to_string(i) + "-" + std::to_string(j) + fs::path(insert_picture).extension().string();
				if (!cv::imwrite(output_name, vec_ret[i * col + j])) return false;
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
	QList<Det_Obj> loadJson(const QString& jsonInsert) {
		//检查jsonInsert文件是否存在
		QFile file(jsonInsert);
		QFileInfo fileinfo(jsonInsert);
		if (!file.exists()) {
			qDebug() << "file not exists";
			return QList<Det_Obj>();
		}
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) { qDebug() << "Cant open"; return QList<Det_Obj>(); };
		QByteArray json_string = file.readAll();
		// 解析JSON字符串
		QJsonDocument json_doc = QJsonDocument::fromJson(json_string);
		QJsonObject json_obj = json_doc.object();
		QJsonArray data_array = json_obj["data"].toArray();
		// 遍历data数组
		QList<Det_Obj> det_objects;
		for (int i = 0; i < data_array.size(); ++i) {
			Det_Obj det_obj;
			QJsonObject data_obj = data_array[i].toObject();
			// 读取flag
			det_obj.flags = data_obj["flag"].toString();
			// 读取points数组
			QJsonArray points_array = data_obj["points"].toArray();
			for (int j = 0; j < points_array.size(); ++j) {
				QJsonArray point_array = points_array[j].toArray();
				QPoint det_point;
				det_point.setX(point_array[0].toDouble());
				det_point.setY(point_array[1].toDouble());
				det_obj.list_points.append(det_point);
			}
			det_obj.json_path = jsonInsert;
			det_objects.append(det_obj);
		}
		return det_objects;
	}
	/// <summary>
	/// 将QList<Det_Obj>按照要求写入到json文件里面去
	/// </summary>
	/// <param name="json_path"></param>
	/// <param name="det_objects"></param>
	/// <returns></returns>
	bool WriteJsonFile(const QString& json_path, const QList<Det_Obj>& det_objects) {
		//检查jsonInsert文件是否存在
		QFile file(json_path);
		QFileInfo fileinfo(json_path);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { qDebug() << "Cant open"; return false; };
		QJsonObject obj_ret;
		//key:data
		QJsonArray data_array;
		for (Det_Obj item : det_objects) {
			QJsonObject obj_item;
			obj_item.insert("flag", item.flags);
			QJsonArray points_array;
			for (QPoint point : item.list_points) {
				QJsonArray point_array;
				point_array.append(point.x());
				point_array.append(point.y());
				points_array.append(point_array);
			}
			obj_item.insert("points", points_array);
			data_array.append(obj_item);
		}
		obj_ret.insert("data", data_array);
		QJsonDocument doc_ret;
		doc_ret.setObject(obj_ret);
		QByteArray byte_ret = doc_ret.toJson();



		file.write(byte_ret);
		file.close();
		return true;
	}
	bool TearDataJson(const std::string& insert_json, const std::string& output_path, int row, int col)
	{
		fs::path insertPath(insert_json);
		if (!fs::is_regular_file(insertPath)) return false;
		if (insertPath.extension().string() != ".json") return false;


		//读取json文件
		QString insert_path_ = QString::fromUtf8(insert_json.c_str());
		//需要获得以下这个json文件的本名，以供后续调用
		QFileInfo info(insert_path_);

		QList<Det_Obj> det_objects = loadJson(insert_path_);


		//我们已知图片的宽度image_width和高度image_height,现在我们要按照给定的row和col，将json中的点按照不同的图片进行切分
		//我们需要计算并找到每个子图片
		double subImageWidth = image_width / col;
		double subImageHeight = image_height / row;


		//以行列来表示
		//现在已经知道每个点的位置，用点去除子图宽度和高度，获得指定子图的索引，取余就可以获得子图的坐标
		QVector<QVector<QList<Det_Obj>>> matrix;

		matrix.resize(row);
		for (int i = 0; i < row; ++i) {
			matrix[i].resize(col);
		}

		for (Det_Obj obj : det_objects) {
			QVector<QVector<Det_Obj>> matrix_obj;
			matrix_obj.resize(row);
			for (int i = 0; i < row; ++i) {
				matrix_obj[i].resize(col);
			}
			for (QPoint point : obj.list_points) {
				int index_w = point.x() / subImageWidth;
				int index_h = point.y() / subImageHeight;
				QPoint actualPoint = QPoint(point.x() - index_w * subImageWidth, point.y() - index_h * subImageHeight);
				matrix_obj[index_w][index_h].list_points.append(actualPoint);
				matrix_obj[index_w][index_h].flags = obj.flags;
			}

			//获得了这个matrix_obj之后，将这个matrix_obj按照次序插入到matrix中去
			for (int w = 0; w < row; ++w) {
				for (int h = 0; h < col; ++h) {
					matrix[w][h].append(matrix_obj[w][h]);
				}
			}
		}

		//将martix的内容写入到文件中去
		for (int w = 0; w < row; ++w) {
			for (int h = 0; h < col; ++h) {
				QString output_path_ = QString::fromUtf8(output_path.c_str()) + QDir::separator() + info.baseName() + "_" + QString::number(w) + "-" + QString::number(h) + ".json";
				WriteJsonFile(output_path_, matrix[h][w]);
			}
		}





		return false;
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

	bool ReformDataset(const std::string& input_path, const std::string& output_path)
	{
		return false;
	}

	QString imageToBase64(const QString& imagePath) {
		// 读取图像
		QImage image(imagePath);

		// 根据图像的格式，将其保存为 JPEG 或 PNG
		QByteArray imageData;
		QBuffer buffer(&imageData);
		buffer.open(QIODevice::WriteOnly);
		//判断文件结尾

		if (imagePath.endsWith(".jpeg") || imagePath.endsWith(".jpg")) {
			image.save(&buffer, "JPEG");
		}
		else {
			image.save(&buffer, "PNG");
		}

		// 获取内存中的图像数据
		QString base64String = QString(imageData.toBase64());

		return base64String;
	}


	/// <summary>
	/// 输入一个TSD数据，输出一个labelme数据
	/// </summary>
	/// <param name="insert_img"></param>
	/// <param name="insert_json"></param>
	/// <param name="output_path"></param>
	/// <returns></returns>
	bool TSDToLabelme_Single(const std::string& insert_img, const std::string& insert_json, const std::string& output_path, bool random_suffix) {
		//检查insert_img和insert_json,output_path是否合法
		fs::path insert_img_path(insert_img);
		fs::path insert_json_path(insert_json);
		fs::path output_path_path(output_path);
		if (!fs::is_regular_file(insert_img_path)) return false;
		if (!fs::is_regular_file(insert_json_path)) return false;
		if (!fs::is_directory(output_path_path)) return false;

		//获得输入文件的名称
		std::string filename = insert_img_path.filename().string();


		QString image_base64 = imageToBase64(QString::fromUtf8(insert_img.c_str()));
		QList<Det_Obj> list_objs = loadJson(QString::fromUtf8(insert_json.c_str()));

		QJsonObject obj_ret;
		QJsonObject obj_version;

		QJsonObject obj_flags;
		obj_flags.insert("flags", QJsonObject());

		QJsonArray arr_shapes;
		for (Det_Obj item : list_objs) {
			QJsonObject obj_item;
			obj_item.insert("label", item.flags);
			obj_item.insert("group_id", "null");
			obj_item.insert("shape_type", "polygon");
			obj_item.insert("description", "");
			obj_item.insert("flags", QJsonObject());
			QJsonArray arr_points;
			for (auto point : item.list_points) {
				QJsonArray arr_point;
				arr_point.append(point.x());
				arr_point.append(point.y());
				arr_points.append(arr_point);
			}
			obj_item.insert("points", arr_points);
			arr_shapes.append(obj_item);
		}

		obj_ret.insert("imageHeight", image_height);
		obj_ret.insert("imageWidth", image_width);
		obj_ret.insert("imageData", image_base64);
		obj_ret.insert("imagePath", QString::fromUtf8(filename.c_str()));
		obj_ret.insert("shapes", arr_shapes);
		obj_ret.insert("flags", obj_flags);
		obj_ret.insert("version", "5.2.1");
		int randomNumber;
		QString outputJson_name;
		QString outputImg_name;
		if (random_suffix) {
			randomNumber = QRandomGenerator::global()->bounded(10000);
			outputJson_name = QString::fromUtf8(output_path.c_str()) + QDir::separator() + QString::fromUtf8(insert_img_path.stem().string().c_str()) + "#" + QString::number(randomNumber) + ".json";
			//移动图片
			outputImg_name = QString::fromUtf8(output_path.c_str()) + QDir::separator() + QString::fromUtf8(insert_img_path.stem().string().c_str()) + +"#" + QString::number(randomNumber) + QString::fromUtf8(insert_img_path.extension().string().c_str());
		}
		else {
			randomNumber = 0;
			outputJson_name = QString::fromUtf8(output_path.c_str()) + QDir::separator() + QString::fromUtf8(insert_img_path.stem().string().c_str()) + ".json";
			//移动图片
			outputImg_name = QString::fromUtf8(output_path.c_str()) + QDir::separator() + QString::fromUtf8(insert_img_path.stem().string().c_str()) + QString::fromUtf8(insert_img_path.extension().string().c_str());

		}





		QFile file_outputJson(outputJson_name);
		if (!file_outputJson.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

		//写入
		QByteArray arr = QJsonDocument(obj_ret).toJson();
		file_outputJson.write(arr);
		file_outputJson.close();
		//移动图片
		if (!QFile::copy(QString::fromUtf8(insert_img.c_str()), outputImg_name)) return false;
		return true;

	}

	void DeleteEmptyJsonCombine(const std::string& input)
	{
		QString input_path = QString::fromUtf8(input.c_str());
		QDir dir(input_path);
		QStringList nameFilters;
		nameFilters << "*.json";
		QList<QFileInfo> fileInfo = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
		for (auto item : fileInfo) {
			//打开文件
			QFile file(item.absoluteFilePath());
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				qDebug() << "Could not open the file for reading";
				return;
			}
			QByteArray arr = file.readAll();
			file.close();
			QJsonDocument doc = QJsonDocument::fromJson(arr);
			QJsonObject obj = doc.object();
			QJsonArray arr_ = obj.value("data").toArray();
			if (arr_.at(0).toObject().value("points").toArray().size() == 0) {
				//删除该文件夹下的json文件和同名图片
				QString filename = item.baseName();
				QString image_path = item.absolutePath() + "/" + filename + ".bmp";
				QFile::remove(image_path);
				QFile::remove(item.absoluteFilePath());
			}
		}
	}

	void DeleteNoneJsonImages(const std::string& path_, const std::string& image_suffix_)
	{
		QString path = QString::fromUtf8(path_.c_str());
		QString image_suffix = QString::fromUtf8(image_suffix_.c_str());
		//删除文件夹内不包含同名json文件的图片文件
		QDir dir(path);
		QStringList nameFilters;
		nameFilters << "*." + image_suffix;
		QList<QFileInfo> fileInfo = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
		for (auto item : fileInfo) {
			QString path = item.absoluteFilePath();
			QString json_path = item.absolutePath() + "/" + item.baseName() + ".json";
			QFileInfo json_info(json_path);
			if (!json_info.exists()) {
				QFile::remove(path);
			}
		}
	}

	void DeleteNoShapeLabelMeData(const std::string& input_path)
	{


	}


	void DeleteEmptyImagesCombine(const std::string& input_path)
	{


	}

	bool TSDToLabelme(const std::string& input_path, const std::string& output_path)
	{
		//将文件夹下的所有TSD数据格式的数据集结构转换成Labelme的数据集结构
		//找到所有的json文件，并获得一个同名的图片文件

		return false;
	}




}
