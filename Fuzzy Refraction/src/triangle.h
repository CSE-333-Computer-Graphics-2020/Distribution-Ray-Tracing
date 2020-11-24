//triangle.h
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"


class Triangle : public Object
{
public:
	Vector3D ver0;
	Vector3D ver1;
	Vector3D ver2;

	Triangle(const Vector3D& _a, const Vector3D& _b, const Vector3D& _c,  Material* mat):
		Object(mat), ver0(_a), ver1(_b), ver2(_c)
	{
		isSolid = true;
	}
	
	virtual bool intersect(Ray& r) const;
};
#endif
