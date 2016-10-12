#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "FeatureDetector.h"

using namespace cv;
#define IMG_NAME	"1.jpg"
#define WND_NAME_TEMP	""
#define SHOW_N_WAIT(img) imshow(WND_NAME_TEMP, img); waitKey(0);

Mat Binarize(Mat img);
Scalar RandomColor(RNG& rng);


int main()
{
	Mat input_img = imread(IMG_NAME, IMREAD_GRAYSCALE);
		SHOW_N_WAIT(input_img);
	Mat img_blur;
	medianBlur(input_img, img_blur, 5);
		SHOW_N_WAIT(img_blur);
	Mat img_bin;
	Canny(img_blur, img_bin, 50, 150);
		SHOW_N_WAIT(img_bin);
	img_bin = Binarize(img_bin);
		SHOW_N_WAIT(img_bin);
	morphologyEx(img_bin, img_bin, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(5, 5))); // clean
		SHOW_N_WAIT(img_bin);
	std::vector<type_condition> cond;
	cond.resize(2);
	cond[0].size_ratio[0] = 1; // for picks
	cond[0].size_ratio[1] = 1.5;
	cond[0].rect[0].width = 50;
	cond[0].rect[1].width = 300;
	cond[0].rect[0].height = 50;
	cond[0].rect[1].height = 300;

	cond[1].size_ratio[0] = 2; // for screws
	cond[1].size_ratio[1] = 7;
	cond[1].rect[0].width = 20;
	cond[1].rect[1].width = 300;
	cond[1].rect[0].height = 20;
	cond[1].rect[1].height = 300;

	std::vector<int> check = {FEATURE_CHECK_SIZE | FEATURE_CHECK_SIZE_RATIO, FEATURE_CHECK_SIZE | FEATURE_CHECK_SIZE_RATIO};
	Mat img_obj = Mat::zeros(img_bin.size(), CV_8UC3);
	std::vector<Obj2d> objects = FindObjects(img_bin, cond, check, RETR_EXTERNAL, 0);
	std::vector<Scalar> colors = {Scalar::all(255), Scalar::all(0)};
	std::vector<Scalar> rect_colors = {Scalar(255, 0, 0), Scalar(0, 0, 255)};
	RNG rng((unsigned)-1);
	cvtColor(input_img, input_img, CV_GRAY2BGR);
	for(auto& obj: objects)
	{
		//DrawContours(obj.contours, colors, img_obj, Point(), 1);
		//DrawRRect(obj.r_rect, input_img, RandomColor(rng));

		DrawRRect(obj.r_rect, input_img, rect_colors[obj.tag]);
	}
		SHOW_N_WAIT(input_img);
	return 0;
}

Mat Binarize(Mat img)
{
	Mat res;
	threshold(img, res, 0, 255, THRESH_OTSU);
	return res;
}
Scalar RandomColor(RNG& rng)
{
	int color = (unsigned)rng;
	return Scalar(color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
}