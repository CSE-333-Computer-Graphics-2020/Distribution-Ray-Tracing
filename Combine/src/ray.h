//ray.h
#ifndef _RAY_H_
#define _RAY_H_

#include <float.h>
#include "vector3D.h"
#include <iostream>
#include <random>
using namespace std;

class Object;

const float SMALLEST_DIST = 1e-4; //Constant used to dismiss intersections very close to previous
class Ray
{
private:
	const Vector3D origin;
	Vector3D direction;
	float t; //Distance travelled alogn the Ray
	bool hit; //has the ray hit something?
	const Object *object;//The object that has been hit
	int level;//Number of times the ray has been traced recursively
	float refractive_index;
	Vector3D normal; //Normal of the hit object

public:  
	Ray(const Vector3D& o, const Vector3D& d, int _level = 0, float _ref_idx = 1.0):
    		origin(o), direction(d), t(FLT_MAX), hit(false), level(_level), refractive_index(_ref_idx)
	{
		direction.normalize();	
	}
	Vector3D getOrigin() const  {return origin;}
	Vector3D getDirection() const  {return direction;}
	Vector3D getPosition() const {return origin + t*direction;}
	Vector3D getNormal() const {return normal;}
	float getParameter() const {return t;}
	bool setParameter(const float par, const Object *obj, const Vector3D normal);
	bool didHit() const {return hit;}
	const Object* intersected() const {return object;}
	int getLevel() const {return level;}
	double getTime() const {
		random_device generator;
		uniform_real_distribution<double> distribution(0.0,50.0);
		uniform_real_distribution<double> distribution2(0.0,1.0);
		double time1 = distribution(generator);
		double time2 = distribution(generator);
		double e = distribution2(generator);
		double time = time1 + e*(time2-time1);
		return time;}

};
#endif
