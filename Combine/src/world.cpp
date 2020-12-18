#include "world.h"
#include<iostream>
#include<string>
#include<map>
#include<random>
#include "utils.h"
using namespace std;

float World::firstIntersection(Ray& ray)
{
	for(int i=0; i<objectList.size(); i++) {
        objectList[i]->intersect(ray);
    }
	return ray.getParameter();
}

Color World::ambi(Ray& ray, Material*& mat)
{
    firstIntersection(ray);
    if (ray.didHit()) {
        Material *m = ray.intersected()->material;
        Color mat_color =  m->color;
        double ka = m->ka;
        return mat_color*ka;
    }
    return background;
}

Color World::shade_ray(Ray& ray)
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


        //Shadow
        // Ray shadow_ray(point, l);
        // firstIntersection(shadow_ray);

        // if(shadow_ray.didHit() && shadow_ray.intersected()->material->kt==0)
        // {
        //     isShadow = true;
        //     Color shad_color = mat_color;
        //     double shad_ka = ka;
        //     return shad_color*shad_ka;

        // }

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

        if (kt==0) {
            return shadow_color;
        }
        // Recursive
        // if(recur<3)
        // { 
            // Reflection
            if(ks > 0)
            {
                Color refl_color(0);
                Vector3D d = v;
                Vector3D r = d - 2*(dotProduct(d,normal))*normal ;
                Ray reflected_ray(point,r);
                // final_color  = final_color  + shade_ray(reflected_ray,recur+1)*kr;


                std::vector<Vector3D> purtubed_reflected_rays;
                int size = 10.0;

                while (purtubed_reflected_rays.size() < size) 
                {                
                    // cout<<"here"<<endl;
                    Vector3D w = r;
                    w.normalize();
                    
                    double temp = min(w[0], w[1]);
                    double min_coord = min(temp, w[2]);

                    Vector3D t;

                    if(w[0]==min_coord && w[0]!=1)
                    {
                        t[0]=1.0;
                        t[1]=w[1];
                        t[2]=w[2];
                    }
                    else if(w[1]==min_coord && w[1]!=1)
                    {
                        t[0]=w[0];
                        t[1]=1.0;
                        t[2]=w[2];
                    }
                    else
                    {
                        t[0]=w[0];
                        t[1]=w[1];
                        t[2]=1.0;                        
                    }

                    Vector3D u = crossProduct(t,w);
                    u.normalize();

                    Vector3D v = crossProduct(w, u);


                    
                    // generate random value
                    random_device generator;
                    uniform_real_distribution<double> distribution(0.0,1.0);


                    double a = 0.15;

                    double rand = distribution(generator);
                    double rand2 = distribution(generator);

                    double t1 = -a/2 + rand*a;
                    double t2 = -a/2 + rand2*a;

                    Vector3D A2 = w + u * t1 + v * t2;

                    A2.normalize();

                    double product = dotProduct(normal, A2);
                    // cout<<product<<endl;
                    
                    if (product > 0) 
                    {
                        // Add it to the results. It's a valid purturbedRay.

                        purtubed_reflected_rays.push_back(A2);
                    }
                }


                for (auto dir : purtubed_reflected_rays) 
                    {
                
                        Ray resultant_ray(point + (0.001 * dir), dir);
                        // firstIntersection(resultant_ray);
                        Color hit_color = ambi(resultant_ray, m);
                        refl_color = refl_color + hit_color;
                        //final_color = final_color + shade_ray(resultant_ray,recur+1)*kr;
                    }

                float glossy_refl=10.0;
                refl_color.R(refl_color.R()/glossy_refl);
                refl_color.G(refl_color.G()/glossy_refl);
                refl_color.B(refl_color.B()/glossy_refl);

                final_color =final_color + refl_color*ks;



                }
            
	return final_color ;
	
	}
return background;
}


