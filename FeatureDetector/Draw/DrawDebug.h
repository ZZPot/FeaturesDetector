#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <map>
#include "../types.h"

//Types definitions
enum STROKE_TYPE
{
	STROKE_NONE = -1,
	STROKE_RECT = 0,
	STROKE_CIRCLE,
	STROKE_CONTOUR
};
enum FILL_TYPE
{
	FILL_NONE = -1,
	FILL_SOLID = 0,
};
enum MARKER_TYPE
{
	MARKER_NONE = -1,
	MARKER_CROSS = cv::MARKER_CROSS,
	MARKER_TILTED_CROSS = cv::MARKER_TILTED_CROSS,
	MARKER_STAR = cv::MARKER_STAR,
	MARKER_DIAMOND = cv::MARKER_DIAMOND,
	MARKER_SQUARE = cv::MARKER_SQUARE,
	MARKER_TRIANGLE_UP = cv::MARKER_TRIANGLE_UP,
	MARKER_TRIANGLE_DOWN = cv::MARKER_TRIANGLE_DOWN
};
enum CAPTION_TYPE
{
	CAPTION_NONE = -1,
	CAPTION_TOP = 0,
	CAPTION_BOTTOM,
	CAPTION_TOP_LEFT,
	CAPTION_TOP_RIGHT,
	CAPTION_BOTTOM_LEFT,
	CAPTION_BOTTOM_RIGHT,
	CAPTION_AUTO
};
enum PATH_TYPE
{
	PATH_NONE = -1,
	PATH_LINES = 0,
	PATH_ARROWS = 1
};
//Defaults definitions
#define DEFAULT_STROKE_TYPE			STROKE_TYPE::STROKE_RECT
#define DEFAULT_STROKE_COLOR		cv::Scalar(255, 0, 0)
#define DEFAULT_STROKE_THICKNESS	1
#define DEFAULT_FILL_TYPE			FILL_NONE
#define DEFAULT_FILL_COLOR			cv::Scalar(255, 255, 255)
#define DEFAULT_CAPTION_TYPE		CAPTION_TYPE::CAPTION_TOP
#define DEFAULT_CAPTION_COLOR		cv::Scalar(255, 255, 255)
#define DEFAULT_CAPTION_SIZE		8
#define DEFAULT_MARKER_TYPE			MARKER_TYPE::MARKER_CROSS
#define DEFAULT_MARKER_COLOR		cv::Scalar(255, 0, 0)
#define DEFAULT_MARKER_SIZE			10
#define DEFAULT_MARKER_THICKNESS	3
#define DEFAULT_PATH_TYPE			PATH_TYPE::PATH_LINES
#define DEFAULT_PATH_COLOR			cv::Scalar(255, 0, 0)
#define DEFAULT_PATH_THICKNESS		1
#define DEFAULT_LINE_TYPE			cv::LineTypes::LINE_4
#define DEFAULT_FONT				cv::FONT_HERSHEY_PLAIN
#define HERSHEY_FONT_SIZE			15

struct draw_conf
{
	draw_conf(STROKE_TYPE stroke = DEFAULT_STROKE_TYPE,
		cv::Scalar stroke_clr = DEFAULT_STROKE_COLOR,
		int stroke_th = DEFAULT_STROKE_THICKNESS,
		FILL_TYPE fill = DEFAULT_FILL_TYPE,
		cv::Scalar fill_clr = DEFAULT_FILL_COLOR,
		CAPTION_TYPE caption = DEFAULT_CAPTION_TYPE,
		cv::Scalar caption_clr = DEFAULT_CAPTION_COLOR,
		int caption_sz = DEFAULT_CAPTION_SIZE,
		MARKER_TYPE marker = DEFAULT_MARKER_TYPE,
		cv::Scalar marker_clr = DEFAULT_MARKER_COLOR,
		int marker_sz = DEFAULT_MARKER_SIZE,
		int marker_th = DEFAULT_MARKER_THICKNESS,
		PATH_TYPE path = DEFAULT_PATH_TYPE,
		cv::Scalar path_clr = DEFAULT_PATH_COLOR,
		int path_th = DEFAULT_PATH_THICKNESS);
	STROKE_TYPE stroke_type;
	cv::Scalar stroke_color;
	int stroke_thickness;
	FILL_TYPE fill_type;
	cv::Scalar fill_color;
	CAPTION_TYPE caption_type;
	cv::Scalar caption_color;
	int caption_size;
	MARKER_TYPE marker_type;
	cv::Scalar marker_color;
	int marker_size;
	int marker_thickness;
	PATH_TYPE path_type;
	cv::Scalar path_color;
	int path_thickness;
};

class DrawConfig
{
public:
	DrawConfig();
	virtual ~DrawConfig();
	int SetTag(TAG_TYPE tag, draw_conf* obj);
	int ResetTag(TAG_TYPE tag);
	draw_conf* GetDrawByTag(TAG_TYPE tag);
protected:
	std::map<TAG_TYPE, draw_conf*> _config;
};
void DrawRect(cv::Rect rect, cv::Mat& img, cv::Scalar color = cv::Scalar(255, 255, 255), int thickness = 1);
void DrawRRect(cv::RotatedRect r_rect, cv::Mat& img, cv::Scalar color = cv::Scalar(255, 255, 255), int thickness = 1);
void DrawContours(std::vector<std::vector<contour_type>> contours,
	std::vector<cv::Scalar> colors, cv::Mat& img,
	cv::Point offset = cv::Point(), int level_limit = -1, cv::LineTypes line_type = cv::LineTypes::FILLED);
int DrawObjects(cv::Mat& img, std::vector<Obj2d> objects, DrawConfig& config);
int DrawObj(cv::Mat& img, Obj2d& object, draw_conf* config);
cv::Point2d CalcStrSize(std::string str, cv::HersheyFonts font, int size);

extern cv::LineTypes global_line_type;