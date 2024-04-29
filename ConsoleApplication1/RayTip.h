#ifndef RAYTIP_H
#define RAYTIP_H
#include"Point.h"
class RayTip : public Point {
public:
	RayTip();//用于创建RayTip数组
	RayTip(double xPos, double yPos,Point sourcePosition,double directionAngle=0, double distanceToSource=0);
	//存疑：会不会不需要sourcePosition参数，因为RayTip本身就是在场内函数被创建的。
	double getDirectionAngle()const;
	double getDistanceToSource()const;

	void setDirectionAngle(const double& directionAngle);
	void setDistanceToSource(const double& distanceToSource);
private:
	Point SourcePosition;
	double DirectionAngle;
	double DistanceToSource;
};
#endif // RAYTIP_H
