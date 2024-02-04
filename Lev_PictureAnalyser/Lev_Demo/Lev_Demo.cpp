// Lev_Demo.cpp: 定义应用程序的入口点。
//

#include "Lev_Demo.h"
#include "Lev_PictureAnalyser.h"
#include "Leventure_ModelInfer.h"
#include <filesystem>
#include <map>
namespace fs = std::filesystem;


//找到一个文件夹下的所有指定后缀的文件
std::map<std::string, std::string> FindAllTargetFile(const std::string& path, const std::string& suffix) {
	std::map<std::string, std::string> map_ret;

	try {
		for (const auto& entry : fs::directory_iterator(path)) {
			if (entry.is_regular_file() && entry.path().extension().string() == suffix) {
				map_ret.insert(std::make_pair(entry.path().stem().string(), entry.path().string()));
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return map_ret;
}

void SplitImgsAndJson(const std::string input, const std::string output) {
	for (int i = 1; i < 16; ++i) {
		if (i == 8 || i == 9) continue;
		std::string str_folder1 = input + "/" + std::to_string(i);
		std::cout << " std_folder1" << str_folder1 << endl;
		//先将图片全部切一边
		for (const auto& item : fs::directory_iterator(str_folder1)) {
			if (item.is_regular_file()) {
				if (fs::path(item).extension() == ".bmp") {
					std::cout << "TearPicture" << item.path().string() << output << endl;
					Lev_PictureManager::TearPicture(item.path().string(), output, 4, 4);
				}
				else if (fs::path(item).extension() == ".json") {
					std::cout << "TearDataJson" << item.path().string() << output << endl;
					Lev_PictureManager::TearDataJson(item.path().string(), output, 4, 4);
				}
			}
		}
	}
}

void transLabel(const std::string input, std::string str_output_path) {
	//遍历其中的所有文件夹
		//找到此目录下的所有同名的bmp文件和.json文件
	std::map<std::string, std::string> map_bmp = FindAllTargetFile(input, ".bmp");
	std::map<std::string, std::string> map_json = FindAllTargetFile(input, ".json");

	//遍历map_bmp，找到同名的json，调用转换函数
	for (auto item = map_bmp.begin(); item != map_bmp.end(); ++item) {
		std::string bmp_path = item->second;
		std::string json_path = map_json[item->first];
		Lev_PictureManager::TSDToLabelme_Single(bmp_path, json_path, str_output_path, false);
		std::cout << "TSDToLabelme_Single" << bmp_path << json_path << str_output_path << endl;
	}
}

int main()
{

	//分割图片和json
	Lev_PictureManager::DeleteNoneJsonImages("J:\\RawDataSet\\TrainDataSet\\1_shangliang_shang", "bmp");
	//fs::path input_path = "J:\\RawDataSet\\RawData\\Path\\6_shangliang_shang";
	//fs::path output_path = "J:\\RawDataSet\\split";


	//for (auto item : fs::directory_iterator(input_path.string())) {
	//	if (fs::is_directory(item)) {
	//		for (auto item_ : fs::directory_iterator(item.path().string())) {
	//			Lev_PictureManager::DeleteNoneJsonImages(item_.path().string(), "bmp");
	//		}

	//		SplitImgsAndJson(item.path().string(), output_path.string());
	//	}
	//}
	//Lev_PictureManager::DeleteEmptyJsonCombine("J:/RawDataSet/split");
	//transLabel("J:\\RawDataSet\\split", "J:\\RawDataSet\\TrainDataSet\\6_shangliang_shang");

	return 0;
}
