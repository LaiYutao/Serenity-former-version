#ifndef RAYTIP_H
#define RAYTIP_H
#include"Point.h"
class RayTip : public Point {
public:
	RayTip();//���ڴ���RayTip����
	RayTip(double xPos, double yPos,Point sourcePosition,double directionAngle=0, double distanceToSource=0);
	//���ɣ��᲻�᲻��ҪsourcePosition��������ΪRayTip��������ڳ��ں����������ġ�
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
