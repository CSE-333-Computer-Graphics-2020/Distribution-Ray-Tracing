//sphere.cpp

#include "sphere.h"

bool Sphere::intersect(Ray& r) const
{
	Vector3D centerVector = r.getOrigin() - position;
	double a = 1.0;
	double b = 2*dotProduct(r.getDirection(), centerVector);
	double c = dotProduct(centerVector, centerVector) - radius*radius;
	double discriminant = b*b - 4.0*a*c;

	//now check if discr. is posivtive or zero, then only we have an intersection!
	if(discriminant >=0.0)
	{

		if(discriminant == 0)
		{
			double t;
			t = -b/(2.0*a);
			Vector3D ray = r.getOrigin() + t*r.getDirection();
            Vector3D normal = ray - position;
            normal.normalize();
			r.setParameter(t, this, normal);

			return true;
		}
		else
		{
			//Calculate both roots
			double D = sqrt(discriminant);
			double t1 = (-b +D)/(2.0*a);
			double t2 = (-b -D)/(2.0*a);
            
            Vector3D ray1 = r.getOrigin() + t1*r.getDirection();
            Vector3D normal1 =ray1 - position;
            normal1.normalize();
            
            Vector3D ray2 = r.getOrigin() + t2*r.getDirection();
            Vector3D normal2 =ray2 - position;
            normal2.normalize();
            
			bool b1 = r.setParameter(t1,this, normal1);
			bool b2 = r.setParameter(t2,this, normal2);

			return b1||b2;    
		}
	}
	return false;

}
