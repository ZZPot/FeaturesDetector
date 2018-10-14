#pragma once
#include <vector>
#include <set>
#include <opencv2/core.hpp>

typedef std::vector<cv::Point> contour_type;

typedef int TAG_TYPE;

struct Obj2d
{
	cv::Rect rect;
	cv::RotatedRect r_rect;
	std::vector<std::vector<contour_type>> contours;
	double square;
	TAG_TYPE tag;
	std::string name;
};

struct Marker2d
{
	cv::Point2d point;
	TAG_TYPE tag;
	std::string name;
};

struct Path2d
{
	std::vector<cv::Point> points;
	TAG_TYPE tag;
};

struct type_condition
{
	cv::Rect rect[2];
	cv::RotatedRect r_rect[2];

	double square[2]; // min max

	double square_ratio[2]; // min max
	double size_ratio[2]; // min max
	double bounding_size_ratio[2]; // min max
	TAG_TYPE tag;
	std::string tag_name;
};