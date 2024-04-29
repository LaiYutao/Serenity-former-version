#include"Point.h"
Point::Point(double xPos, double yPos) : XPos(xPos), YPos(yPos) {}

double Point::getXPos() const
{
    return XPos;
}

double Point::getYPos() const
{
    return YPos;
}

void Point::setXPos(double xPos)
{
    XPos = xPos;
}

void Point::setYPos(double yPos)
{
    YPos = yPos;
}

Point Point::operator+(const Point& point) const
{
    return Point(XPos + point.XPos, YPos + point.YPos);
}

Point Point::operator-(const Point& point) const
{
    return Point(XPos - point.XPos, YPos - point.YPos);
}

Point Point::operator*(double factor) const
{
    return Point(XPos * factor, YPos * factor);
}

Point Point::operator/(double factor) const
{
    return Point(XPos / factor, YPos / factor);
}

Point& Point::operator+=(const Point& point)
{
    XPos += point.XPos;
    YPos += point.YPos;
    return *this;
}

Point& Point::operator-=(const Point& point)
{
    XPos -= point.XPos;
    YPos -= point.YPos;
    return *this;
}