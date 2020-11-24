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


        //Shadow
        Ray shadow_ray(point, l);
        firstIntersection(shadow_ray);

        // if(shadow_ray.didHit() && shadow_ray.intersected()->material->kt==0)
        // {
        //     isShadow = true;
        //     Color shad_color = mat_color;
        //     double shad_ka = ka;
        //     return shad_color*shad_ka;

        // }

        
        // Recursive
        if(recur<3)
        { 
            // Reflection
            if(ks > 0)
            {
                Color reflection_color(0);
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

                    //square length
                    double a = 0.15;
                    double rand = distribution(generator);
                    double rand2 = distribution(generator);

                    double t1 = -a/2 + rand*a;
                    double t2 = -a/2 + rand2*a;

                    Vector3D perturbed = w + u * t1 + v * t2;
                    perturbed.normalize();

                    double product = dotProduct(normal, perturbed);
                    // cout<<product<<endl;
                    // Check if perturbed ray below surface
                    if (product > 0) 
                    {
                        purtubed_reflected_rays.push_back(perturbed);
                    }
                }


                for( int i=0;i<size;i++)
                {
                    Vector3D perturbed_ray_dir = purtubed_reflected_rays[i];
                    Ray perturbed_reflected_ray(point + (0.001 * perturbed_ray_dir), perturbed_ray_dir);
                    // firstIntersection(perturbed_reflected_ray);
                    Color hit_color = ambi(perturbed_reflected_ray, m);
                    reflection_color = reflection_color + hit_color;
                    //final_color = final_color + shade_ray(perturbed_reflected_ray,recur+1)*kr;

                }

                float glossy_refl=10.0;
                reflection_color.R(reflection_color.R()/glossy_refl);
                reflection_color.G(reflection_color.G()/glossy_refl);
                reflection_color.B(reflection_color.B()/glossy_refl);

                final_color =final_color + reflection_color*ks;

                }

        }
        
    return final_color ;
    
    }
return background;
}

