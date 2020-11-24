#include "world.h"
#include<iostream>
#include<string>
#include<map>
#include<random>
#include "utils.h"
using namespace std;

float World::firstIntersection(Ray& ray)
{
	for(int i=0; i<objectList.size(); i++)
	objectList[i]->intersect(ray);
	return ray.getParameter();
}

Color World::shade_ray(Ray& ray, int recur)
{
	firstIntersection(ray);
    bool isShadow = false;
	if(ray.didHit())
	{  
        Color final_color = 0;

        // Blinn Phong
        Color mat_color =  ray.intersected()->material->color;
        Material *m = ray.intersected()->material;
        double ka = m->ka;
        double kd = m->kd;
        double ks = m->ks;
        double kr = m->kr;
        double kt = m->kt;
        double eta = m->eta;

        Vector3D point = ray.getPosition();
        Vector3D normal = ray.getNormal();
        Vector3D v = ray.getDirection();
        
        Vector3D l_pos = lightSourceList[0]->getPosition();
        Vector3D l = (l_pos - point);
        Vector3D h = l + v;
        l.normalize();
        h.normalize();

        Color diffuse = mat_color*kd*max(dotProduct(normal,l),0.0);
        Color specular = mat_color*ks*max(pow(dotProduct(h,normal),5.0), 0.0);

        final_color =  diffuse + specular;


        Color shadow_color = Color(0.0, 0.0, 0.0);
        //Shadow

        int shadow_sample = 10;
        for(int i=0; i<shadow_sample;i++)
        {
	        Vector3D c = l_pos;
	        Vector3D a = Vector3D(-1.5, 0, 10) - c;
	        Vector3D b = Vector3D(-1, -0.5, 10) - c;

	        random_device generator;
	        uniform_real_distribution<double> distribution(0.0,1.0);

	        double rand = distribution(generator);
	        double rand2 = distribution(generator);

	        Vector3D r = c + rand*a + rand2*b;


	        Ray shadow_ray(point, r);
	        firstIntersection(shadow_ray);

	        if(shadow_ray.didHit() && shadow_ray.intersected()->material->kt==0)
	        {
	            isShadow = true;
	            Color shad_color = mat_color;
	            double shad_ka = ka;
	            shadow_color = shadow_color + shad_color*shad_ka;
        	}
        	else
        	{
        		shadow_color = shadow_color + final_color;
        	}
    	}

    	shadow_color.R(shadow_color.R()/shadow_sample);
    	shadow_color.G(shadow_color.G()/shadow_sample);
    	shadow_color.B(shadow_color.B()/shadow_sample);

    	return shadow_color;

        double nt = 1.0;
        // Recursive
        
	return final_color ;
	
	}
return background;
}


