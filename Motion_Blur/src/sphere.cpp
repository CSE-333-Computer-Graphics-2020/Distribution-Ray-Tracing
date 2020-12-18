//sphere.cpp

#include "sphere.h"
#include <iostream>
#include <random>
using namespace std;

bool Sphere::intersect(Ray& r) const
{
	double time = r.getTime();
	random_device generator;
	uniform_real_distribution<double> distribution(0.00,0.05);
	double rand1 = distribution(generator);
	double rand2 = distribution(generator);
	// double rand3 = distribution(generator);
	Vector3D shift(rand1, rand2, 0.0);
	Vector3D pos = position;
	for (double i=0.0; i<time; i++) {
		if (i<time/2) {
			pos = pos+Vector3D(-0.08,0.00,0.00);
		}
		else {
			pos = pos+Vector3D(0.16,0.00,0.00);
		}
		// Vector3D centerVector = r.getOrigin() - position;
		Vector3D centerVector = r.getOrigin() - pos;
		double a = 1.0;
		double b = 2*dotProduct(r.getDirection(), centerVector);
		double c = dotProduct(centerVector, centerVector) - radius*radius;
		double discriminant = b*b - 4.0*a*c;

		//now check if discr. is positive or zero, then only we have an intersection!
		if(discriminant >=0.0)
		{

			if(discriminant == 0)
			{
				double t;
				t = -b/(2.0*a);
				Vector3D ray = r.getOrigin() + t*r.getDirection();
	            // Vector3D normal = ray - position;
	            Vector3D normal = ray - pos;
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
	            // Vector3D normal1 =ray1 - position;
	            Vector3D normal1 =ray1 - pos;
	            normal1.normalize();
	            
	            Vector3D ray2 = r.getOrigin() + t2*r.getDirection();
	            // Vector3D normal2 =ray2 - position;
	            Vector3D normal2 =ray2 - pos;
	            normal2.normalize();
	            
				bool b1 = r.setParameter(t1,this, normal1);
				bool b2 = r.setParameter(t2,this, normal2);

				return b1||b2;    
			}
		}
	}
	return false;
}
