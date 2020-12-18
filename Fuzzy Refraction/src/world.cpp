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

// Color World::shade_ray(Ray& ray, int recur)
// {
    
// 	firstIntersection(ray);
//     bool isShadow = false;
// 	if(ray.didHit())
// 	{  
//         Color final_color = 0;

//         // Blinn Phong
//         Color mat_color =  ray.intersected()->material->color;
//         Material *m = ray.intersected()->material;
//         double ka = m->ka;
//         double kd = m->kd;
//         double ks = m->ks;
//         double kr = m->kr;
//         double kt = m->kt;
//         double eta = m->eta;

//         Vector3D point = ray.getPosition();
//         Vector3D normal = ray.getNormal();
//         Vector3D v = ray.getDirection();
        
//         Vector3D l_pos = lightSourceList[0]->getPosition();
//         Vector3D l = (l_pos - point);
//         Vector3D h = l + v;
//         l.normalize();
//         h.normalize();

//         Color diffuse = mat_color*kd*max(dotProduct(normal,l),0.0);
//         Color specular = mat_color*ks*max(pow(dotProduct(h,normal),5.0), 0.0);

//         final_color =  diffuse + specular;

//         double nt = 1.0;
//         if(recur<3)
//         {
//             normal.normalize();
//             v.normalize();
//             Vector3D N = normal;
//             Vector3D I = -v;
//             double epsilon = 0.001;
//             Color reflected_color = 0;

//             if(ks > 0)
//             {
//                 Color refl_color(0);
//                 Vector3D d = v;
//                 Vector3D r = d - 2*(dotProduct(d,normal))*normal ;
//                 Ray reflected_ray(point,r);
//                 firstIntersection(reflected_ray);
//                 if(reflected_ray.didHit())
//                 {
//                     nt = reflected_ray.intersected()->material->eta;
//                 }
//                 reflected_color = shade_ray(reflected_ray,recur+1);
//                 final_color  = final_color  + reflected_color*kr;
//             }

//             if(kt>0)
//             {   
                
//                 double n1 = 1.0;
//                 double n2 = 1.0;

//                 Color refraction_color = Color(0.0, 0.0, 0.0);
//                 double product = dotProduct(I,N);

//                 if (product > 0.0)
//                 {
//                     n2 = eta;
//                 }
//                 else
//                 {
//                     n1 = eta;
//                     N = -N;
//                 }
                
//                 double n = n1 / n2;
//                 double incident_cos = min(dotProduct(I,N), 1.0);
//                 double sin_square = (1.0 - (incident_cos * incident_cos));
//                 sin_square = n*n*sin_square;
//                 double incident_sin = sqrt(sin_square);

//                 if (incident_sin <= 1.0)
//                 {

//                     double cos_theta = min(dotProduct(I, N), 1.0);
//                     // Change
//                     Vector3D refract_perp = n*(v + cos_theta*N);
//                     Vector3D refract_par = -sqrt(abs(1.0 - (refract_perp[0]*refract_perp[0] + refract_perp[1]*refract_perp[1] + refract_perp[2]*refract_perp[2])))*N;
//                     Vector3D refraction_dir = refract_perp + refract_par;
//                     Ray refr_ray(point, refraction_dir);

//                     double trans_cos = sqrt(1.0 - sin_square);

//                     // double val = (1 - (incident_cos * incident_cos));
//                     // val = 1 - (pow(n,2.0) * val);
//                     // Vector3D refraction_dir = (n * (I - (normal*incident_cos))) - (normal * sqrt(val));
//                     // refraction_dir.normalize();
//                     // Ray refr_ray(point, refraction_dir);

//                     double Rs = pow((n1*incident_cos - n2*trans_cos) / (n1*incident_cos + n2*trans_cos), 2.0);
//                     double Rp = pow((n1*trans_cos - n2*incident_cos) / (n1*trans_cos + n2*incident_cos), 2.0);
//                     double fresnal = (Rs+Rp) / 2.0;

//                     refraction_color = shade_ray(refr_ray, recur+1);
//                     refraction_color.R((1.0 - fresnal)*refraction_color.R());
//                     refraction_color.G((1.0 - fresnal)*refraction_color.G());
//                     refraction_color.B((1.0 - fresnal)*refraction_color.B());

//                     reflected_color.R(fresnal*reflected_color.R());
//                     reflected_color.G(fresnal*reflected_color.G());
//                     reflected_color.B(fresnal*reflected_color.B());
//                     final_color = final_color + (refraction_color + reflected_color);

//                 }
            
//             }  

//         }          
            
//         return final_color;
    
//     }
//     return background;

// }


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

        double nt = 1.0;
        if(recur<3)
        {
            normal.normalize();
            v.normalize();
            Vector3D N = normal;
            Vector3D I = -v;
            double epsilon = 0.001;
            Color reflected_color = 0;

            if(ks > 0)
            {
                Color refl_color(0);
                Vector3D d = v;
                Vector3D r = d - 2*(dotProduct(d,normal))*normal ;
                Ray reflected_ray(point,r);
                firstIntersection(reflected_ray);
                if(reflected_ray.didHit())
                {
                    nt = reflected_ray.intersected()->material->eta;
                }
                reflected_color = shade_ray(reflected_ray,recur+1);
                final_color  = final_color  + reflected_color*kr;
            }

            if(kt>0)
            {   
                
                double n1 = 1.0;
                double n2 = 1.0;

                Color refraction_color = Color(0.0, 0.0, 0.0);
                double product = dotProduct(I,N);

                if (product > 0.0)
                {
                    n2 = eta;
                }
                else
                {
                    n1 = eta;
                    N = -N;
                }
                
                double n = n1 / n2;
                double incident_cos = min(dotProduct(I,N), 1.0);
                double sin_square = (1.0 - (incident_cos * incident_cos));
                sin_square = n*n*sin_square;
                double incident_sin = sqrt(sin_square);

                if (incident_sin <= 1.0)
                {

                    double cos_theta = min(dotProduct(I, N), 1.0);
                    
                    // reference 
                    Vector3D perpendicular = n*(v + cos_theta*N);
                    double val1 = (perpendicular[0]*perpendicular[0] + perpendicular[1]*perpendicular[1] + perpendicular[2]*perpendicular[2]);
                    Vector3D parallel = -sqrt(abs(1.0 - val1))*N;
                    Vector3D refraction_dir = perpendicular + parallel;

                    std::vector<Vector3D> purtubed_refracted_rays;
                int size = 5.0;

                while (purtubed_refracted_rays.size() < size) 
                {                
                    // cout<<"here"<<endl;
                    Vector3D w = refraction_dir;
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
                    purtubed_refracted_rays.push_back(perturbed);

                }
                    Color final_refraction_color = 0;
                    Color final_reflection_color = 0;

                    for( int i=0;i<size;i++)
                    {
                        Vector3D perturbed_ray_dir = purtubed_refracted_rays[i];
                        Ray refr_ray(point, perturbed_ray_dir);

                        double trans_cos = sqrt(1.0 - sin_square);

                        // double val = (1 - (incident_cos * incident_cos));
                        // val = 1 - (pow(n,2.0) * val);
                        // Vector3D refraction_dir = (n * (I - (normal*incident_cos))) - (normal * sqrt(val));
                        // refraction_dir.normalize();
                        // Ray refr_ray(point, refraction_dir);

                        double Rs = pow((n1*incident_cos - n2*trans_cos) / (n1*incident_cos + n2*trans_cos), 2.0);
                        double Rp = pow((n1*trans_cos - n2*incident_cos) / (n1*trans_cos + n2*incident_cos), 2.0);
                        double fresnal = (Rs+Rp) / 2.0;

                        refraction_color = shade_ray(refr_ray, recur+1);
                        refraction_color.R((1.0 - fresnal)*refraction_color.R());
                        refraction_color.G((1.0 - fresnal)*refraction_color.G());
                        refraction_color.B((1.0 - fresnal)*refraction_color.B());

                        reflected_color.R(fresnal*reflected_color.R());
                        reflected_color.G(fresnal*reflected_color.G());
                        reflected_color.B(fresnal*reflected_color.B());

                        final_refraction_color = final_refraction_color + refraction_color;
                        final_reflection_color = final_reflection_color + reflected_color;
                        
                    }

                    float glossy_refl=5.0;
                    final_refraction_color.R(final_refraction_color.R()/glossy_refl);
                    final_refraction_color.G(final_refraction_color.G()/glossy_refl);
                    final_refraction_color.B(final_refraction_color.B()/glossy_refl);

                    final_reflection_color.R(final_reflection_color.R()/glossy_refl);
                    final_reflection_color.G(final_reflection_color.G()/glossy_refl);
                    final_reflection_color.B(final_reflection_color.B()/glossy_refl);

                    final_color = final_color + (final_refraction_color + final_reflection_color);

                }
            
            }  

        }          
            
        return final_color;
    
    }
    return background;

}