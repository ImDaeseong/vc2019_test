#pragma once
class DrawRoundRect
{
public:
	DrawRoundRect();
	~DrawRoundRect();

	static void OnRoundRectPath(Gdiplus::GraphicsPath& path, Gdiplus::RectF rect, float radius);
	static void OnDrawRoundRect(Gdiplus::Graphics& Gps, Gdiplus::RectF rect, Gdiplus::Color color, int radius);
	static void OnDrawRoundRectLine(Gdiplus::Graphics& Gps, Gdiplus::RectF rect, Gdiplus::Color color, int radius);
};
