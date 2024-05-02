#ifndef POINT_H
#define POINT_H
#define PI 3.14159
#define PI_ 0.318309

class Point 
{
public:
    Point(double xPos, double yPos);
    virtual ~Point() {};
    
    double getXPos() const;
    double getYPos() const;
    void setXPos(double xPos);
    void setYPos(double yPos);
    Point operator+(const Point& point) const;
    Point operator-(const Point& point) const;
    Point operator*(double factor) const;
    Point operator/(double factor) const;
    Point& operator+=(const Point& point);
    Point& operator-=(const Point& point);

private:
	double XPos;
	double YPos;
};
#endif // POINT_H
