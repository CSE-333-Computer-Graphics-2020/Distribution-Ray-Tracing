//sphere.cpp

#include "triangle.h"
#include <iostream>
#include "utils.h"

bool Triangle::intersect(Ray& r) const
{

	glm::vec3 v0 = glm::vec3(this->ver0[0], this->ver0[1], this->ver0[2]);
	glm::vec3 v1 = glm::vec3(this->ver1[0], this->ver1[1], this->ver1[2]);
	glm::vec3 v2 = glm::vec3(this->ver2[0], this->ver2[1], this->ver2[2]);

	Vector3D normal = crossProduct((ver1 - ver0), (ver2-ver0));
	normal.normalize();

	Vector3D e = r.getOrigin();
	Vector3D d = r.getDirection();
	

	glm::mat3 A(1.0f);
	A[0] = glm::vec3(v0[0] - v1[0], v0[1] - v1[1], v0[2] - v1[2]);
	A[1] = glm::vec3(v0[0] - v2[0], v0[1] - v2[1], v0[2] - v2[2]);
	A[2] = glm::vec3(d[0], d[1], d[2]);


	glm::mat3 T(1.0f);
	T[0] = glm::vec3(v0[0] - v1[0], v0[1] - v1[1], v0[2] - v1[2]);
	T[1] = glm::vec3(v0[0] - v2[0], v0[1] - v2[1], v0[2] - v2[2]);
	T[2] = glm::vec3(v0[0] - e[0], v0[1] - e[1], v0[2] - e[2]);


	glm::mat3 B(1.0f);
	B[0] = glm::vec3(v0[0] - e[0], v0[1] - e[1], v0[2] - e[2]);
	B[1] = glm::vec3(v0[0] - v2[0], v0[1] - v2[1], v0[2] - v2[2]);
	B[2] = glm::vec3(d[0], d[1], d[2]);


	glm::mat3 G(1.0f);
	G[0] = glm::vec3(v0[0] - v1[0], v0[1] - v1[1], v0[2] - v1[2]);
	G[1] = glm::vec3(v0[0] - e[0], v0[1] - e[1], v0[2] - e[2]);
	G[2] = glm::vec3(d[0], d[1], d[2]);


	double detA = glm::determinant(A);
	double detT = glm::determinant(T);
	double detB = glm::determinant(B);
	double detG = glm::determinant(G);

	double beta = detB/detA;
	double gamma = detG/detA;
	double t = detT/detA;

    if(beta > 0.0 && gamma > 0.0 && (beta+gamma) < 1.0 )
    	{r.setParameter(t, this, normal);
    	return true;
    }
    else
    	{
    	return false;
    }

}
