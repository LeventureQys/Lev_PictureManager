//   Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "Leventure_ModelInfer.h"
#include "include/object_detector.h"
#include <glog/logging.h>

#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <filesystem>
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#elif LINUX
#include <stdarg.h>
#include <sys/stat.h>
#endif

#include <gflags/gflags.h>






void PrintBenchmarkLog(std::vector<double> det_time, int img_num) {
}
void Lev_ModelInfer::PrintBenchmarkLog(std::vector<double> det_time, int img_num) {
	PrintBenchmarkLog(det_time, img_num);
}
string Lev_ModelInfer::DirName(const std::string& filepath) {
	auto pos = filepath.rfind(OS_PATH_SEP);
	if (pos == std::string::npos) {
		return "";
	}
	return filepath.substr(0, pos);
}


bool Lev_ModelInfer::PathExists(const std::string& path) {
#ifdef _WIN32
	struct _stat buffer;
	return (_stat(path.c_str(), &buffer) == 0);
#else
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
#endif  // !_WIN32
}


void Lev_ModelInfer::MkDir(const std::string& path) {
	if (PathExists(path)) return;
	int ret = 0;
#ifdef _WIN32
	ret = _mkdir(path.c_str());
#else
	ret = mkdir(path.c_str(), 0755);
#endif  // !_WIN32
	if (ret != 0) {
		std::string path_error(path);
		path_error += " mkdir failed!";
		throw std::runtime_error(path_error);
	}
}

void Lev_ModelInfer::MkDirs(const std::string& path) {
	if (path.empty()) return;
	if (PathExists(path)) return;

	MkDirs(DirName(path));
	MkDir(path);
}


void Lev_ModelInfer::GetAllFiles(const char* dir_name,
	std::vector<std::string>& all_inputs) {
	if (NULL == dir_name) {
		std::cout << " dir_name is null ! " << std::endl;
		return;
	}
	struct stat s;
	stat(dir_name, &s);
	if (!S_ISDIR(s.st_mode)) {
		std::cout << "dir_name is not a valid directory !" << std::endl;
		all_inputs.push_back(dir_name);
		return;
	}
	else {
		struct dirent* filename; // return value for readdir()
		DIR* dir;                // return value for opendir()
		dir = opendir(dir_name);
		if (NULL == dir) {
			std::cout << "Can not open dir " << dir_name << std::endl;
			return;
		}
		std::cout << "Successfully opened the dir !" << std::endl;
		while ((filename = readdir(dir)) != NULL) {
			if (strcmp(filename->d_name, ".") == 0 ||
				strcmp(filename->d_name, "..") == 0)
				continue;
			all_inputs.push_back(dir_name + std::string("/") +
				std::string(filename->d_name));
		}
	}
}


//void Lev_ModelInfer::PredictVideo(const std::string& video_path,
//	PaddleDetection::ObjectDetector* det) {
//	// Open video
//	cv::VideoCapture capture;
//	if (camera_id != -1) {
//		capture.open(camera_id);
//	}
//	else {
//		capture.open(video_path.c_str());
//	}
//	if (!capture.isOpened()) {
//		printf("can not open video : %s\n", video_path.c_str());
//		return;
//	}
//
//	// Get Video info : resolution, fps
//	int video_width = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
//	int video_height = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));
//	int video_fps = static_cast<int>(capture.get(CV_CAP_PROP_FPS));
//
//	// Create VideoWriter for output
//	cv::VideoWriter video_out;
//	std::string video_out_path = "output.avi";
//	video_out.open(video_out_path.c_str(),
//		CV_FOURCC('D', 'I', 'V', 'X'),
//		video_fps,
//		cv::Size(video_width, video_height),
//		true);
//	std::cout << video_out.isOpened();
//	if (!video_out.isOpened()) {
//		printf("create video writer failed!\n");
//		return;
//	}
//
//	std::vector<PaddleDetection::ObjectResult> result;
//	std::vector<int> bbox_num;
//	std::vector<double> det_times;
//	auto labels = det->GetLabelList();
//	auto colormap = PaddleDetection::GenerateColorMap(labels.size());
//	// Capture all frames and do inference
//	cv::Mat frame;
//	int frame_id = 0;
//	bool is_rbox = false;
//	int icu = 0;
//	while (capture.read(frame)) {
//		icu += 1;
//		std::cout << icu << "frame" << std::endl;
//		if (frame.empty()) {
//			break;
//		}
//		std::vector<cv::Mat> imgs;
//		imgs.push_back(frame);
//		det->Predict(imgs, 0.5, 0, 1, &result, &bbox_num, &det_times);
//		for (const auto& item : result) {
//			if (item.rect.size() > 6) {
//				is_rbox = true;
//				printf("class=%d confidence=%.4f rect=[%d %d %d %d %d %d %d %d]\n",
//					item.class_id,
//					item.confidence,
//					item.rect[0],
//					item.rect[1],
//					item.rect[2],
//					item.rect[3],
//					item.rect[4],
//					item.rect[5],
//					item.rect[6],
//					item.rect[7]);
//			}
//			else {
//				printf("class=%d confidence=%.4f rect=[%d %d %d %d]\n",
//					item.class_id,
//					item.confidence,
//					item.rect[0],
//					item.rect[1],
//					item.rect[2],
//					item.rect[3]);
//			}
//		}
//
//		cv::Mat out_im = PaddleDetection::VisualizeResult(
//			frame, result, labels, colormap, is_rbox);
//
//		video_out.write(out_im);
//		frame_id += 1;
//	}
//	capture.release();
//	video_out.release();
//}



void PredictImage(const std::vector<std::string> all_img_paths,
	const int batch_size,
	const double threshold,
	const bool run_benchmark,
	PaddleDetection::ObjectDetector* det,
	const std::string& output_dir) {
	std::vector<double> det_t = { 0, 0, 0 };
	int steps = ceil(float(all_img_paths.size()) / batch_size);
	printf("total images = %d, batch_size = %d, total steps = %d\n",
		all_img_paths.size(), batch_size, steps);
	for (int idx = 0; idx < steps; idx++) {
		std::vector<cv::Mat> batch_imgs;
		int left_image_cnt = all_img_paths.size() - idx * batch_size;
		if (left_image_cnt > batch_size) {
			left_image_cnt = batch_size;
		}
		for (int bs = 0; bs < left_image_cnt; bs++) {
			std::string image_file_path = all_img_paths.at(idx * batch_size + bs);
			cv::Mat im = cv::imread(image_file_path, 1);
			batch_imgs.insert(batch_imgs.end(), im);
		}

		// Store all detected result
		std::vector<PaddleDetection::ObjectResult> result;
		std::vector<int> bbox_num;
		std::vector<double> det_times;
		bool is_rbox = false;
		if (run_benchmark) {
			det->Predict(batch_imgs, threshold, 10, 10, &result, &bbox_num, &det_times);
		}
		else {
			det->Predict(batch_imgs, threshold, 0, 1, &result, &bbox_num, &det_times);
			// get labels and colormap
			auto labels = det->GetLabelList();
			auto colormap = PaddleDetection::GenerateColorMap(labels.size());

			int item_start_idx = 0;
			for (int i = 0; i < left_image_cnt; i++) {
				std::cout << all_img_paths.at(idx * batch_size + i) << "result" << std::endl;
				if (bbox_num[i] <= 1) {
					continue;
				}
				for (int j = 0; j < bbox_num[i]; j++) {
					PaddleDetection::ObjectResult item = result[item_start_idx + j];
					if (item.rect.size() > 6) {
						is_rbox = true;
						printf("class=%d confidence=%.4f rect=[%d %d %d %d %d %d %d %d]\n",
							item.class_id,
							item.confidence,
							item.rect[0],
							item.rect[1],
							item.rect[2],
							item.rect[3],
							item.rect[4],
							item.rect[5],
							item.rect[6],
							item.rect[7]);
					}
					else {
						printf("class=%d confidence=%.4f rect=[%d %d %d %d]\n",
							item.class_id,
							item.confidence,
							item.rect[0],
							item.rect[1],
							item.rect[2],
							item.rect[3]);
					}
				}
				item_start_idx = item_start_idx + bbox_num[i];
			}
			// Visualization result
			int bbox_idx = 0;
			for (int bs = 0; bs < batch_imgs.size(); bs++) {
				if (bbox_num[bs] <= 1) {
					continue;
				}
				cv::Mat im = batch_imgs[bs];
				std::vector<PaddleDetection::ObjectResult> im_result;
				for (int k = 0; k < bbox_num[bs]; k++) {
					im_result.push_back(result[bbox_idx + k]);
				}
				bbox_idx += bbox_num[bs];
				cv::Mat vis_img = PaddleDetection::VisualizeResult(
					im, im_result, labels, colormap, is_rbox);
				std::vector<int> compression_params;
				compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
				compression_params.push_back(95);
				std::string output_path = output_dir + "\\";
				std::string image_file_path = all_img_paths.at(idx * batch_size + bs);
				output_path += std::filesystem::path(image_file_path).filename().string();
				cv::imwrite(output_path, vis_img, compression_params);
				printf("Visualized output saved as %s\n", output_path.c_str());
				//std::string output_path(output_dir);
				//if (output_dir.rfind(OS_PATH_SEP) != output_dir.size() - 1) {
				//	output_path += OS_PATH_SEP;
				//}
				//std::string image_file_path = all_img_paths.at(idx * batch_size + bs);
				//output_path += image_file_path.substr(image_file_path.find_last_of('/') + 1);
				//cv::imwrite(output_path, vis_img, compression_params);
				//printf("Visualized output saved as %s\n", output_path.c_str());
			}
		}
		det_t[0] += det_times[0];
		det_t[1] += det_times[1];
		det_t[2] += det_times[2];
	}
	//PrintBenchmarkLog(det_t, all_img_paths.size());
}
void Lev_ModelInfer::PredictImage_(const std::vector<std::string> all_img_paths,
	const int batch_size,
	const double threshold,
	const bool run_benchmark,
	ObjDetector det,
	const std::string& output_dir) {
	PaddleDetection::ObjectDetector* model = new PaddleDetection::ObjectDetector(
		det.model_dir, det.device, det.use_mkldnn, det.cpu_threads, det.run_mode, det.batch_size, det.trt_min_shape
		, det.trt_max_shape, det.trt_opt_shape, det.trt_calib_mode
	);

	PredictImage(all_img_paths, batch_size, threshold, run_benchmark, model, output_dir);
}
//std::string model_dir;
//std::string image_file;
//std::string video_file;
//std::string image_dir;
//int batch_size = 1;
//bool use_gpu = true;
//int camera_id = -1;
//double threshold = 0.1;
//std::string output_dir = "output";
//std::string run_mode = "fluid";
//int gpu_id = 0;
//bool run_benchmark = false;
//bool use_mkldnn = false;
//double cpu_threads = 0.9;
//bool use_dynamic_shape = false;
//int trt_min_shape = 1;
//int trt_max_shape = 1280;
//int trt_opt_shape = 640;
//bool trt_calib_mode = false;
//
//int main_test() {
//	model_dir = "D:/projects/PaddleDetection/deploy/cpp/out/Release/models";
//	image_file = "D:/projects/PaddleDetection/deploy/cpp/out/Release/images/1.jpg";
//	//video_file = "bb.mp4";
//	//image_dir = "";
//  // Parsing command-line
//  //google::ParseCommandLineFlags(&argc, &argv, true);
//
//	if (model_dir.empty()
//		|| (image_file.empty() && image_dir.empty() && video_file.empty())) {
//		std::cout << "Usage: ./main --model_dir=/PATH/TO/INFERENCE_MODEL/ "
//			<< "--image_file=/PATH/TO/INPUT/IMAGE/" << std::endl;
//	}
//	if (!(run_mode == "fluid" || run_mode == "trt_fp32"
//		|| run_mode == "trt_fp16" || run_mode == "trt_int8")) {
//		std::cout << "run_mode should be 'fluid', 'trt_fp32', 'trt_fp16' or 'trt_int8'.";
//		return -1;
//	}
//	// Load model and create a object detector
//	PaddleDetection::ObjectDetector det(model_dir, use_gpu, use_mkldnn,
//		threshold, run_mode, batch_size, gpu_id, use_dynamic_shape,
//		trt_min_shape, trt_max_shape, trt_opt_shape, trt_calib_mode);
//	// Do inference on input video or image
//	MyClass predictvideo;
//	if (!video_file.empty() || camera_id != -1) {
//		predictvideo.PredictVideo(video_file, &det);
//	}
//	else if (!image_file.empty() || !image_dir.empty()) {
//		if (!predictvideo.PathExists(output_dir)) {
//			predictvideo.MkDirs(output_dir);
//		}
//		std::vector<std::string> all_imgs;
//		if (!image_file.empty()) {
//			all_imgs.push_back(image_file);
//			if (batch_size > 1) {
//				std::cout << "batch_size should be 1, when image_file is not None" << std::endl;
//				batch_size = 1;
//			}
//		}
//		else {
//			predictvideo.GetAllFiles((char*)image_dir.c_str(), all_imgs);
//		}
//		predictvideo.PredictImage(all_imgs, batch_size, threshold, run_benchmark, &det, output_dir);
//	}
//	return 0;
//}


int main(int argc, char** argv) {
}
