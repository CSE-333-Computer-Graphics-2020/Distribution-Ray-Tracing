#include "world.h"
#include "material.h"

/*
Color Material::shade(const Ray& incident,const bool isSolid) const
{
	
	glm::vec3 temp_v = glm::vec3(incident.getDirection()[0],incident.getDirection()[1],incident.getDirection()[2]);
	glm::vec3 v = -glm::normalize(temp_v);

	glm::vec3 temp_l = glm::vec3(incident.getPosition()[0],incident.getPosition()[1],incident.getPosition()[2]);
	// glm::vec3 o = glm::vec3(incident.getOrigin());
	// float t = incident.getParameter();
	// glm::vec3 d = glm::vec3(incident.getDirection());
	// glm::vec3 lig = glm::vec3(o+t*d);

	glm::vec3 l = glm::normalize(glm::vec3(1.0f,1.0f,-10.0f) - temp_l);
	
	glm::vec3 h = glm::normalize(-v+l);
	
	//glm::vec3 nor = glm::vec3(incident.getNormal()[0],incident.getNormal()[1],incident.getNormal()[2]);
	glm::vec3 a = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 b = glm::vec3(0.0f,3.0f,0.0f);
	glm::vec3 c = glm::vec3(4.0f,0.0f,0.0f);

	glm::vec3 nor = glm::normalize(glm::cross(a-b,a-c));

	glm::vec3 color_main = glm::vec3(0.1f,0.7f,0.0f);
	glm::vec3 color_main2 = glm::vec3(1.0f,1.0f,1.0f);

	glm::vec3 L = (float)kd*color_main*std::max(0.0f,glm::dot(nor,l)) + (float)ks*color_main2*(float)pow(std::max(0.0f,glm::dot(nor,h)),100);

	Color *co = new Color((double)L.x,(double)L.y,(double)L.z);
	//this->color = co;

	//std::cout<<"L.x "<<(float)pow(std::max(0.0f,glm::dot(nor,h)),1)<<std::endl;
	//std::cout<<"L.y "<<L.y<<std::endl;
	//std::cout<<"L.z "<<L.z<<std::endl;

	return *co;
}

*/


Color Material::shade(const Ray& incident, const bool isSolid) const
{
    
	return color;

}




