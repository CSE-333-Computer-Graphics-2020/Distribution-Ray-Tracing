//object.h
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "ray.h"
#include "vector3D.h"
#include "color.h"
#include "material.h"
#include <istream>

class Object
{

protected:
	bool isSolid;

public:
	Material *material;
	Object(Material *mat): material(mat) {}
	virtual bool intersect(Ray& ray) const = 0;

	virtual void resetPosition(){};
	Vector3D getPosition(){};
	//	
	virtual Color shade(const Ray& ray) const
	{
		return material->shade(ray, isSolid);
	}
};

#endif
