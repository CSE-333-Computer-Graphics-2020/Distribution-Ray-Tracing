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
/*
Color World::colour(Ray& ray)
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

        normal.normalize();
        v.normalize();
        Vector3D N = normal;
        Vector3D I = -v;
        double R = 1.0;
        double epsilon = 0.00001;


        if(kt>0)
        {   
            
            double n1 = 1.0;
            double n2 = 1.0;

            // Refraction color
            Color refraction_color = Color(0.0, 0.0, 0.0);

            
            if (dotProduct(I,N) > 0)
            {
                // Entering transmissive object
                n2 = ray.intersected()->material->eta;
            }
            else
            {
                // Leaving transmissive object
                n1 = ray.intersected()->material->eta;
                N = -N;
            }
            
            // Compute refraction direction
            double n = n1 / n2;
            double incident_cos = dotProduct(I,N);
            double sin_square = n * n * (1.0 - incident_cos * incident_cos);
            Color refraction_color = 0;
            if (sin_square <= 1)
            {
                double trans_cos = sqrt(1.0 - sin_square);
                Vector3D refraction_direction = n * I - (n * incident_cos + trans_cos) * N;
                refraction_direction.normalize();

                Vector3D refr_origin = point + epsilon * refraction_direction;
                Ray refr_ray(refr_origin, refraction_direction);


                std::vector<Vector3D> purtubed_reflected_rays;
                int size = 10.0;

                while (purtubed_reflected_rays.size() < size) 
                {                
                    // cout<<"here"<<endl;
                    Vector3D w = -refraction_direction;
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
                    cout<<product<<endl;
                    
                    if (product > 0) 
                    {
                        // Add it to the results. It's a valid purturbedRay.

                        purtubed_reflected_rays.push_back(A2);
                    }
                    else
                    {
                        break;
                    }
                }

            

                for (auto dir : purtubed_reflected_rays) 
                    {

                        double Rs = (n1 * incident_cos - n2 * trans_cos) / (n1 * incident_cos + n2 * trans_cos);
                        double Rp = (n1 * trans_cos - n2 * incident_cos) / (n1 * trans_cos + n2 * incident_cos);
                        R = (Rs * Rs + Rp * Rp) / 2.0;

                        refraction_color = colour(refr_ray);
                        refraction_color.R((1.0 - R)*refraction_color.R());
                        refraction_color.G((1.0 - R)*refraction_color.G());
                        refraction_color.B((1.0 - R)*refraction_color.B());

                        refraction_color = refraction_color + refraction_color;
                    }

                refraction_color.R(refraction_color.R()/10.0);
                refraction_color.G(refraction_color.G()/10.0);
                refraction_color.B(refraction_color.B()/10.0);
            }

            // Add refraction
            final_color = final_color + refraction_color;
        }

        return final_color;
    }

    return background;
}
*/

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

                if (product > 0)
                {
                    n2 = eta;
                }
                else
                {
                    n1 = eta;
                    N = -N;
                }
                
                double n = n1 / n2;
                double incident_cos = dotProduct(I,N);
                double sin_square = (1.0 - (incident_cos * incident_cos));
                sin_square = n*n*sin_square;
                double incident_sin = sqrt(sin_square);

                if (incident_sin <= 1)
                {
                    double trans_cos = sqrt(1.0 - sin_square);

                    double val = (1 - (incident_cos * incident_cos));
                    val = 1 - (pow(n,2.0) * val);

                    Vector3D refraction_dir = (n * (I - (normal*incident_cos))) - (normal * sqrt(val));
                    refraction_dir.normalize();
                    Ray refr_ray(point, refraction_dir);

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
                    final_color = final_color + (refraction_color + reflected_color);

                }
            
            }  

        }          
            
        return final_color;
    
    }
    return background;


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

        // Recursive
        	
            //Reflection

            /*
           normal.normalize();
           v.normalize();
        	Vector3D N = normal;
        	Vector3D I = -v;
        	double R = 1.0;
        	double epsilon = 0.00001;


        	if(kt>0)
			{	
				
				double n1 = 1.0;
				double n2 = 1.0;

				// Refraction color
				Color refraction_color = Color(0.0, 0.0, 0.0);

				
				if (dotProduct(I,N) > 0)
				{
					// Entering transmissive object
					n2 = ray.intersected()->material->eta;
				}
				else
				{
					// Leaving transmissive object
					n1 = ray.intersected()->material->eta;
					N = -N;
				}
				
				// Compute refraction direction
				double n = n1 / n2;
				double incident_cos = dotProduct(I,N);
				double sin_square = n * n * (1.0 - incident_cos * incident_cos);
                Color refraction_color = 0;
				if (sin_square <= 1)
				{
					double trans_cos = sqrt(1.0 - sin_square);
					Vector3D refraction_direction = n * I - (n * incident_cos + trans_cos) * N;
					refraction_direction.normalize();

					Vector3D refr_origin = point + epsilon * refraction_direction;
					Ray refr_ray(refr_origin, refraction_direction);


                std::vector<Vector3D> purtubed_reflected_rays;
                int size = 10.0;

                while (purtubed_reflected_rays.size() < size) 
                {                
                    // cout<<"here"<<endl;
                    Vector3D w = -refraction_direction;
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
                    cout<<product<<endl;
                    
                    if (product > 0) 
                    {
                        // Add it to the results. It's a valid purturbedRay.

                        purtubed_reflected_rays.push_back(A2);
                    }
                    else
                    {
                        break;
                    }
                }

                

                for (auto dir : purtubed_reflected_rays) 
                    {

                        double Rs = (n1 * incident_cos - n2 * trans_cos) / (n1 * incident_cos + n2 * trans_cos);
                        double Rp = (n1 * trans_cos - n2 * incident_cos) / (n1 * trans_cos + n2 * incident_cos);
                        R = (Rs * Rs + Rp * Rp) / 2.0;

                        refraction_color = colour(refr_ray);
                        refraction_color.R((1.0 - R)*refraction_color.R());
                        refraction_color.G((1.0 - R)*refraction_color.G());
                        refraction_color.B((1.0 - R)*refraction_color.B());

                        refraction_color = refraction_color + refraction_color;
                    }

                    refraction_color.R(refraction_color.R()/10.0);
                    refraction_color.G(refraction_color.G()/10.0);
                    refraction_color.B(refraction_color.B()/10.0);


				}

				// Add refraction
				final_color = final_color + refraction_color;

			}

            

            
            
        }
        
        
	return final_color ;
	
	}
return background;
*/


            

/*

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
        double kr_mat = m->kr;
        double kt = m->kt;
        double eta = m->eta;

        Vector3D point = ray.getPosition();
        Vector3D normal = ray.getNormal();
        Vector3D v = ray.getDirection();
        Vector3D origin = ray.getOrigin();
        
        Vector3D l_pos = lightSourceList[0]->getPosition();
        Vector3D l = (l_pos - point);
        Vector3D h = l + v;
        l.normalize();
        h.normalize();

        Color diffuse = mat_color*kd*max(dotProduct(normal,l),0.0);
        Color specular = mat_color*ks*max(pow(dotProduct(h,normal),5.0), 0.0);

        final_color =  diffuse + specular;


        if(recur<3)
        {
            if(ks>0)
            {
                Vector3D reflected_dir = (2*dotProduct(v,normal)*normal) - v;
                Ray reflected = Ray(point, -reflected_dir);
                Color reflected_color = Color(-0.01) + shade_ray(reflected, recur+1);
            }
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
                final_color  = final_color  + shade_ray(reflected_ray,recur+1)*kr;

           }

            if(kt>0)
            {

                Vector3D reflected_dir = (2*dotProduct(v,normal)*normal) - v;
                Ray reflected = Ray(point, -reflected_dir);
                Color reflected_color = shade_ray(reflected, recur+1);
                double product = dotProduct(-v, normal);
                double kr = 1;
                double kg = 1;
                double kb = 1;

                Color refraction_color = 0;
                cout<<product<<endl;
                if(product<0)
                {
                    double val = 1 - ((eta*eta) * (1 - (product * product)));
                    double ar = 0.2;
                    double ag = 0.2;
                    double ab = 0.2;

                    double dist = (origin - point).length();
                    kr = exp(-ar * dist);
                    kg = exp(-ag * dist);
                    kb = exp(-ab * dist);

                    if(val>0)
                    {
                        Vector3D refraction_dir = (eta * (-v - (normal*(product)))) - (normal * sqrt(val));
                        Ray refraction = Ray(point, -refraction_dir);
                        refraction_color = shade_ray(refraction, recur+1);

                    }
                    else
                    {

                        reflected_color.R(reflected_color.R()*kr);
                        reflected_color.G(reflected_color.G()*kg);
                        reflected_color.B(reflected_color.B()*kb);
                        return reflected_color + final_color;
                    }

                    double Ro = ((eta - 1)*(eta - 1)) / ((eta + 1)*(eta + 1));
                    if(eta > 1){
                        if(Ro > 1){
                            reflected_color.R(reflected_color.R()* kr);
                            reflected_color.G(reflected_color.G() * kg);
                            reflected_color.B(reflected_color.B() * kb);

                            return reflected_color + final_color;
                        }
                        product = sqrt(1 - Ro);  
                    }
                    double R = Ro + ((1 - Ro)*pow(1-product, 5));

                    reflected_color.R(reflected_color.R()*kr*R);
                    reflected_color.G(reflected_color.G()*kg*R);
                    reflected_color.B(reflected_color.B()*kb*R);

                    refraction_color.R(refraction_color.R()*kr*(1-R));
                    refraction_color.G(refraction_color.G()*kr*(1-R));
                    refraction_color.B(refraction_color.B()*kr*(1-R));

                    return reflected_color + refraction_color + final_color;

                }


            }
        }

        return final_color;

    }

return background;
*/
}
