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
        Ray shadow_ray(point, l);
        firstIntersection(shadow_ray);

        if(shadow_ray.didHit() && shadow_ray.intersected()->material->kt==0)
        {
            isShadow = true;
            Color shad_color = mat_color;
            double shad_ka = ka;
            return shad_color*shad_ka;

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
            
            // if(kt > 0)
            // {
            //     // Refraction
            //     Vector3D d = v;
            //     Vector3D part_a = (d - normal*(dotProduct(d,normal)));
            //     part_a = part_a/eta;

            //     double val1 = pow(dotProduct(d,normal), 2);
            //     val1 = (1-val1) / (pow(eta, 2));
            //     Vector3D part_b = normal*sqrt(1-val1);

            //     Vector3D term = part_a - part_b;
            //     Ray refracted_ray(point, term);

            //     Color refr_color(0);
            //     // Vector3D d = v;
            //     // Vector3D r = d - 2*(dotProduct(d,normal))*normal ;
            //     // Ray reflected_ray(point,r);
            //     //final_color  = final_color  + shade_ray(reflected_ray,recur+1)*kr;


            //     std::vector<Vector3D> purtubed_refracted_rays;
            //     int size = 10.0;

            //     while (purtubed_refracted_rays.size() < size) 
            //     {                
            //         cout<<"here"<<endl;
            //         Vector3D w = refracted_ray.getDirection();
            //         w.normalize();
                    
            //         double temp = min(w[0], w[1]);
            //         double min_coord = min(temp, w[2]);

            //         Vector3D t;

            //         if(w[0]==min_coord && w[0]!=1)
            //         {
            //             t[0]=1.0;
            //             t[1]=w[1];
            //             t[2]=w[2];
            //         }
            //         else if(w[1]==min_coord && w[1]!=1)
            //         {
            //             t[0]=w[0];
            //             t[1]=1.0;
            //             t[2]=w[2];
            //         }
            //         else
            //         {
            //             t[0]=w[0];
            //             t[1]=w[1];
            //             t[2]=1.0;                        
            //         }

            //         Vector3D u = crossProduct(t,w);
            //         u.normalize();

            //         Vector3D v = crossProduct(w, u);


                    
            //         // generate random value
            //         random_device generator;
            //         uniform_real_distribution<double> distribution(0.0,1.0);
            //         double exponent = 5.0;
                    
            //         double phi = distribution(generator) * 2 * glm::pi<double>();
            //         double cosine_theta = pow(distribution(generator), (double)1.0/(exponent+1));
            //         double sine_theta = sqrt(1 - cosine_theta * cosine_theta);
            //         double cosine_phi = glm::cos(phi);
            //         double sine_phi = glm::sin(phi);
            //         cout<<phi<<" "<<cosine_theta<<" "<<sine_theta<<endl;
            //         cout<<cosine_phi<<" "<<sine_phi<<endl;
                    
            //         // Vector3D A = w * cosine_theta + u * cosine_phi * sine_theta + v * sine_phi * sine_theta;
            //         Vector3D A = w + u * cosine_phi * sine_theta + v * sine_phi * sine_theta;

            //         double product = dotProduct(normal, A);
            //         cout<<product<<endl;
                    
            //         if (product > 0) 
            //         {
            //             // Add it to the results. It's a valid purturbedRay.
            //             cout<<"BOOM"<<endl;
            //             purtubed_refracted_rays.push_back(A);
            //         }
            //     }


            //     for (auto dir : purtubed_refracted_rays) 
            //         {
                
            //             Ray resultant_ray(point + (0.0001 * dir), dir);
            //             Color hit_color(0);
            //             hit_color = shade_ray(resultant_ray, recur+1);
            //             refr_color = refr_color + hit_color;
            //             //final_color = final_color + shade_ray(resultant_ray,recur+1)*kr;
            //         }

            //     float glossy_refr=10.0;
            //     final_color.R(final_color.R()/glossy_refr);
            //     final_color.G(final_color.G()/glossy_refr);
            //     final_color.B(final_color.B()/glossy_refr);
            //     //refl_color = refl_color/glossy_refl;

            //     cout<<refr_color.R()<<" "<<refr_color.G()<<endl;

            //     final_color =final_color + refr_color*0.8*0.3;

            //     // final_color = final_color  + shade_ray(refracted_ray,recur+1)*kt;
                
            // }
            
            
            
            
        // }
        
        
	return final_color ;
	
	}
return background;
}


