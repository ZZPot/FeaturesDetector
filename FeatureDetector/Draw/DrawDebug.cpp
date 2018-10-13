#include "DrawDebug.h"

draw_conf::draw_conf(STROKE_TYPE stroke,
	cv::Scalar stroke_clr,
	int stroke_th,
	FILL_TYPE fill,
	cv::Scalar fill_clr,
	CAPTION_TYPE caption,
	cv::Scalar caption_clr,
	int caption_sz,
	MARKER_TYPE marker,
	cv::Scalar marker_clr,
	int marker_sz,
	int marker_th,
	PATH_TYPE path,
	cv::Scalar path_clr,
	int path_th)
{
	stroke_type = stroke;
	stroke_color = stroke_clr;
	stroke_thickness = stroke_th;
	fill_type = fill;
	fill_color = fill_clr;
	caption_type = caption;
	caption_color = caption_clr;
	caption_size = caption_sz;
	marker_type = marker;
	marker_color = marker_clr;
	marker_size = marker_sz;
	marker_thickness = marker_th;
	path_type = path;
	path_color = path_clr;
	path_thickness = path_th;
}
DrawConfig::DrawConfig()
{}
DrawConfig::~DrawConfig()
{}
int DrawConfig::SetTag(TAG_TYPE tag, draw_conf* conf)
{
	int res = 0;
	
	return res;
}
int DrawConfig::ResetTag(TAG_TYPE tag)
{
	int res = 0;
	
	return res;
}
draw_conf* DrawConfig::GetDrawByTag(TAG_TYPE tag)
{
	draw_conf* res = nullptr;

	return res;
}
int DrawObjects(cv::Mat& img, std::vector<Obj2d> objects, DrawConfig& config)
{
	int res = 0;
	draw_conf def_conf;
	for (Obj2d& object : objects)
	{
		draw_conf* cur_conf = config.GetDrawByTag(object.tag);
		if (cur_conf == nullptr)
			cur_conf = &def_conf;
		res += DrawObj(img, object, cur_conf);
	}
	return res;
}
int DrawObj(cv::Mat& img, Obj2d& object, draw_conf* config)
{
	int res = 1;
	if (config->stroke_type != STROKE_TYPE::STROKE_NONE)
	{
		switch (config->stroke_type)
		{
		case STROKE_TYPE::STROKE_RECT:
			DrawRect(object.rect, img, config->stroke_color, config->stroke_thickness);
			break;
		case STROKE_TYPE::STROKE_CIRCLE:
			cv::ellipse(img, object.r_rect, config->stroke_color, config->stroke_thickness, global_line_type);
			break;
		case STROKE_TYPE::STROKE_CONTOUR:
			DrawContours(object.contours, {config->stroke_color}, img, cv::Point(), 1, global_line_type);
			break;
		}
	}
	if (config->fill_type != FILL_TYPE::FILL_NONE)
	{
		switch (config->fill_type)
		{
		case FILL_TYPE::FILL_SOLID:
			DrawContours(object.contours, { config->fill_color }, img, cv::Point(), 1, cv::LineTypes::FILLED);
			break;
		}
	}
	if ((config->caption_type != CAPTION_TYPE::CAPTION_NONE) && (object.name.length()))
	{
		cv::Point2d caption_pos;
		int text_len = object.name.length();
		cv::Point2d text_size = CalcStrSize(object.name, DEFAULT_FONT, config->caption_size);
		switch (config->caption_type)
		{
		case CAPTION_TYPE::CAPTION_TOP:
			caption_pos.y = object.rect.y;
			caption_pos.x = object.rect.x - (object.rect.width - text_size.x) / 2;
			break;
		case CAPTION_TYPE::CAPTION_TOP_LEFT:
			caption_pos.y = object.rect.y;
			caption_pos.x = object.rect.x - text_size.x;
			break;
		case CAPTION_TYPE::CAPTION_TOP_RIGHT:
			caption_pos.y = object.rect.y;
			caption_pos.x = object.rect.x - object.rect.width;
			break;
		case CAPTION_TYPE::CAPTION_BOTTOM:
			caption_pos.y = object.rect.y - object.rect.height - text_size.y;
			caption_pos.x = object.rect.x - (object.rect.width - text_size.x) / 2;
			break;
		case CAPTION_TYPE::CAPTION_BOTTOM_LEFT:
			caption_pos.y = object.rect.y - object.rect.height - text_size.y;
			caption_pos.x = object.rect.x - text_size.x;
			break;
		case CAPTION_TYPE::CAPTION_BOTTOM_RIGHT:
			caption_pos.y = object.rect.y - object.rect.height - text_size.y;
			caption_pos.x = object.rect.x - object.rect.width;
			break;
		}
		putText(img, object.name.c_str(), caption_pos, DEFAULT_FONT, config->caption_size, config->caption_color, 1, global_line_type);
	}
	return res;
}
void DrawRect(cv::Rect rect, cv::Mat& img, cv::Scalar color, int thickness)
{
	cv::rectangle(img, rect, color, thickness);
}
void DrawRRect(cv::RotatedRect r_rect, cv::Mat& img, cv::Scalar color, int thickness)
{
	cv::Point2f points_f[4];
	r_rect.points(points_f);
	std::vector<cv::Point> points = { points_f[0], points_f[1], points_f[2], points_f[3] };
	if (thickness < 0)
		cv::fillPoly(img, points, color);
	else
		cv::polylines(img, points, true, color, thickness);
}
void DrawContours(std::vector<std::vector<contour_type>> contours,
	std::vector<cv::Scalar> colors, cv::Mat& img,
	cv::Point offset, int level_limit, cv::LineTypes line_type)
{
	if (level_limit < 0)
		level_limit = contours.size();
	for (unsigned i = 0; i < level_limit; i++)
	{
		unsigned color_num = i % colors.size(); // cyclically switching colors
		cv::drawContours(img, contours[i], -1, colors[color_num], line_type, cv::LINE_8, cv::noArray(), INT_MAX, offset);
	}
}
cv::Point2d CalcStrSize(std::string str, cv::HersheyFonts font, int size)
{
	cv::Point2d res = cv::Point2d();
	//?????????
	return res;
}
cv::LineTypes global_line_type = DEFAULT_LINE_TYPE;