#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "FeatureDetector/FeatureDetector.h"
#include "FeatureDetector/Draw/DrawDebug.h"
using namespace cv;
#define IMG_NAME	"1.jpg"
#define TAG_PICK	1
#define TAG_SCREW	2
#define PICK_COLOR cv::Scalar(255, 255, 255)
#define SCREW_COLOR cv::Scalar(255, 0, 0)

int main()
{
	Mat input_img = imread(IMG_NAME, IMREAD_GRAYSCALE);
		SHOW_N_WAIT(input_img);
	Mat img_bin = Binarize(input_img);

	//These conditions can be
	std::vector<type_condition> cond;
	cond.resize(2);
	cond[0].size_ratio[0] = 1; // for picks
	cond[0].size_ratio[1] = 1.5;
	cond[0].rect[0].width = 50;
	cond[0].rect[1].width = 300;
	cond[0].rect[0].height = 50;
	cond[0].rect[1].height = 300;
	cond[0].tag = TAG_PICK;
	cond[0].tag_name = "Pick";

	cond[1].size_ratio[0] = 2; // for screws
	cond[1].size_ratio[1] = 7;
	cond[1].rect[0].width = 20;
	cond[1].rect[1].width = 300;
	cond[1].rect[0].height = 20;
	cond[1].rect[1].height = 300;
	cond[1].tag = TAG_SCREW;
	cond[1].tag_name = "Screw";

	std::vector<int> check = {FEATURE_CHECK_SIZE | FEATURE_CHECK_SIZE_RATIO};
	Mat img_obj = Mat::zeros(img_bin.size(), CV_8UC3);
	std::vector<Obj2d> objects = FindObjects(img_bin, cond, check, RETR_EXTERNAL, 0);
	cvtColor(input_img, input_img, CV_GRAY2BGR);
#pragma region COLORING
	DrawConfig draw_config;
	draw_conf pick_conf;
	draw_conf screw_conf;
	pick_conf.stroke_type = STROKE_TYPE::STROKE_CONTOUR;
	pick_conf.stroke_color = PICK_COLOR;
	screw_conf.stroke_type = STROKE_TYPE::STROKE_CONTOUR;
	screw_conf.stroke_color = SCREW_COLOR;
	pick_conf.fill_color = PICK_COLOR;
	screw_conf.fill_color = SCREW_COLOR;
	pick_conf.caption_color = PICK_COLOR;
	screw_conf.caption_color = SCREW_COLOR;
	draw_config.SetTag(TAG_PICK, &pick_conf);
	draw_config.SetTag(TAG_SCREW, &screw_conf);
#pragma endregion
//tests
	cv::Mat temp_img;
	temp_img = input_img.clone();
	DrawObjects(temp_img, objects, draw_config);
		SHOW_N_WAIT(temp_img);
	//haha, I can change it like this, because of pointer
	pick_conf.stroke_type = STROKE_TYPE::STROKE_RECT;
	screw_conf.stroke_type = STROKE_TYPE::STROKE_RECT;
	temp_img = input_img.clone();
	DrawObjects(temp_img, objects, draw_config);
		SHOW_N_WAIT(temp_img);
	pick_conf.stroke_type = STROKE_TYPE::STROKE_CIRCLE;
	screw_conf.stroke_type = STROKE_TYPE::STROKE_CIRCLE;
	temp_img = input_img.clone();
	DrawObjects(temp_img, objects, draw_config);
		SHOW_N_WAIT(temp_img);
	pick_conf.stroke_type = STROKE_TYPE::STROKE_RRECT;
	screw_conf.stroke_type = STROKE_TYPE::STROKE_RRECT;
	temp_img = input_img.clone();
	DrawObjects(temp_img, objects, draw_config);
		SHOW_N_WAIT(temp_img);
	pick_conf.fill_type = FILL_TYPE::FILL_SOLID;
	screw_conf.fill_type = FILL_TYPE::FILL_SOLID;
	temp_img = input_img.clone();
	DrawObjects(temp_img, objects, draw_config);
		SHOW_N_WAIT(temp_img);
	pick_conf.caption_type = CAPTION_TYPE::CAPTION_BOTTOM_LEFT;
	screw_conf.caption_type = CAPTION_TYPE::CAPTION_TOP_RIGHT;
	pick_conf.stroke_type = STROKE_TYPE::STROKE_RECT;
	screw_conf.stroke_type = STROKE_TYPE::STROKE_RECT;
	temp_img = input_img.clone();
	DrawObjects(temp_img, objects, draw_config);
		SHOW_N_WAIT(temp_img);
	return 0;
}