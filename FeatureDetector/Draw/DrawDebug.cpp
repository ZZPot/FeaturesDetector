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
	_config[tag] = conf;
	res = 1;
	return res;
}
int DrawConfig::ResetTag(TAG_TYPE tag)
{
	int res = 0;
	auto found = _config.find(tag);
	if (found != _config.end())
	{
		_config.erase(found);
		res = 1;
	}
	return res;
}
draw_conf* DrawConfig::GetConfByTag(TAG_TYPE tag)
{
	draw_conf* res = nullptr;
	auto found = _config.find(tag);
	if (found != _config.end())
	{
		res = found->second;
	}
	return res;
}
int DrawObjects(cv::Mat& img, std::vector<Obj2d> objects, DrawConfig& config)
{
	int res = 0;
	draw_conf def_conf;
	for (Obj2d& object : objects)
	{
		draw_conf* cur_conf = config.GetConfByTag(object.tag);
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
		case STROKE_TYPE::STROKE_RRECT:
			DrawRRect(object.r_rect, img, config->stroke_color, config->stroke_thickness);
			break;
		case STROKE_TYPE::STROKE_CIRCLE:
			cv::ellipse(img, object.r_rect, config->stroke_color, config->stroke_thickness, global_line_type);
			break;
		case STROKE_TYPE::STROKE_CONTOUR:
			DrawContours(object.contours, {config->stroke_color}, img, cv::Point(), 1, config->stroke_thickness);
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
		int text_len = object.name.length();
		cv::Size text_size = CalcStrSize(object.name, DEFAULT_FONT, config->caption_size);
		std::map<CAPTION_TYPE, cv::Point2d> positions;
		positions[CAPTION_TYPE::CAPTION_TOP] = cv::Point2d(object.rect.x + (object.rect.width - text_size.width) / 2, object.rect.y);
		positions[CAPTION_TYPE::CAPTION_TOP_LEFT] = cv::Point2d(object.rect.x - text_size.width, object.rect.y);
		positions[CAPTION_TYPE::CAPTION_TOP_RIGHT] = cv::Point2d(object.rect.x + object.rect.width, object.rect.y);
		positions[CAPTION_TYPE::CAPTION_BOTTOM] = cv::Point2d(object.rect.x + (object.rect.width - text_size.width) / 2, object.rect.y + object.rect.height + text_size.height);
		positions[CAPTION_TYPE::CAPTION_BOTTOM_LEFT] = cv::Point2d(object.rect.x - text_size.width, object.rect.y + object.rect.height + text_size.height);
		positions[CAPTION_TYPE::CAPTION_BOTTOM_RIGHT] = cv::Point2d(object.rect.x + object.rect.width, object.rect.y + object.rect.height + text_size.height);
		CAPTION_TYPE cur_type = config->caption_type;
		cv::Rect img_rect(cv::Point2i(0, 0), cv::Size(img.rows, img.cols));
		if (config->caption_type == CAPTION_TYPE::CAPTION_AUTO)
		{
			cur_type = CAPTION_TYPE::CAPTION_TOP; // default value for auto
			int max_square = 0;
			for (auto& position : positions)
			{
				cv::Rect text_rect(position.second, text_size);
				auto intersected_rect = img_rect & text_rect;
				int square = intersected_rect.width*intersected_rect.height;
				if (square > max_square)
				{
					max_square = square;
					cur_type = position.first;
				}
			}
		}
		putText(img, object.name.c_str(), positions[cur_type], DEFAULT_FONT, config->caption_size, config->caption_color, 1, global_line_type);
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
	cv::Point offset, int level_limit, int thickness)
{
	if (level_limit < 0)
		level_limit = contours.size();
	for (unsigned i = 0; i < level_limit; i++)
	{
		unsigned color_num = i % colors.size(); // cyclically switching colors
		cv::drawContours(img, contours[i], -1, colors[color_num], thickness, global_line_type, cv::noArray(), INT_MAX, offset);
	}
}
cv::Size CalcStrSize(std::string str, cv::HersheyFonts font, int size)
{
	cv::Size res = cv::Size();
	res.height = size * HERSHEY_FONT_SIZE;
	res.width = str.length()*HERSHEY_FONT_SIZE*size;
	return res;
}
cv::LineTypes global_line_type = DEFAULT_LINE_TYPE;