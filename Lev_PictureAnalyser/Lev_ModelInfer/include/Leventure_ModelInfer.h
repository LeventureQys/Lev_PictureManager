
//extern "C" __declspec(dllexport) int  main_test();
#include <iostream>
#include <vector>
extern "C" __declspec(dllexport) int add(int a, int b);

int add(int a, int b) {
	return a + b;
}
struct ObjDetector {
	std::string model_dir;
	const std::string device = "CPU";
	bool use_mkldnn = false; int cpu_threads = 1;
	std::string run_mode = std::string("paddle");
	int batch_size = 1; int gpu_id = 0;
	int trt_min_shape = 1;
	int trt_max_shape = 1280;
	int trt_opt_shape = 640;
	bool trt_calib_mode = false;
};


using namespace std;


class __declspec(dllexport) Lev_ModelInfer
{

public:
	void PrintBenchmarkLog(std::vector<double> det_time, int img_num);
	static string DirName(const std::string& filepath);
	static bool PathExists(const std::string& path);
	static void MkDir(const std::string& path);
	static void MkDirs(const std::string& path);
	void GetAllFiles(const char* dir_name, std::vector<std::string>& all_inputs);
	//void PredictVideo(const std::string& video_path, PaddleDetection::ObjectDetector* det);
	void PredictImage_(const std::vector<std::string> all_img_paths,
		const int batch_size,
		const double threshold,
		const bool run_benchmark,
		ObjDetector det,
		const std::string& output_dir = "output");
private:

};