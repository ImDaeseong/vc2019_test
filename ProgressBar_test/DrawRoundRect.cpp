#include "pch.h"
#include "DrawRoundRect.h"

DrawRoundRect::DrawRoundRect()
{
}

DrawRoundRect::~DrawRoundRect()
{
}

void DrawRoundRect::OnRoundRectPath(Gdiplus::GraphicsPath& path, Gdiplus::RectF rect, float radius)
{
    Gdiplus::REAL diam{ 2 * radius };
    path.AddArc(rect.X, rect.Y, diam, diam, 180, 90);
    path.AddLine(rect.X + radius, rect.Y, rect.GetRight() - radius, rect.Y);

    path.AddArc(rect.GetRight() - diam, rect.Y, diam, diam, 270, 90);
    path.AddLine(rect.GetRight(), rect.Y + radius, rect.GetRight(), rect.GetBottom() - radius);

    path.AddArc(rect.GetRight() - diam, rect.GetBottom() - diam, diam, diam, 0, 90);
    path.AddLine(rect.GetRight() - radius, rect.GetBottom(), rect.X + radius, rect.GetBottom());

    path.AddArc(rect.X, rect.GetBottom() - diam, diam, diam, 90, 90);
    path.AddLine(rect.X, rect.Y + radius, rect.X, rect.GetBottom() - radius);
}

void DrawRoundRect::OnDrawRoundRect(Graphics& Gps, Gdiplus::RectF rect, Gdiplus::Color color, int radius)
{
    Gdiplus::GraphicsPath path;
    OnRoundRectPath(path, rect, radius);

    Gps.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
    Gdiplus::SolidBrush brush(color);

    Gps.FillPath(&brush, &path);
    Gps.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeNone);
}

void DrawRoundRect::OnDrawRoundRectLine(Graphics& Gps, Gdiplus::RectF rect, Gdiplus::Color color, int radius)
{
    Gdiplus::GraphicsPath path;
    OnRoundRectPath(path, rect, radius);

    Gps.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
    Gdiplus::Pen* pen = new Gdiplus::Pen(color, 1);

    Gps.DrawPath(pen, &path);
    Gps.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeNone);
}