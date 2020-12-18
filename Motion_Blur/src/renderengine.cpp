#include "renderengine.h"
#define PI 3.14
#include <iostream>
#include<random>
using namespace std;

const Color RenderEngine::trace(const int i, const int j)
{
	Vector3D ray_dir = camera->get_ray_direction(i, j);
	Ray ray(camera->get_position(), ray_dir);
	ray.getTime();
	// return world->shade_ray(ray);
	return Depth(world->shade_ray(ray), ray);
}

bool RenderEngine::renderLoop()
{
	static int i = 0;
	for(int j = 0; j<camera->getHeight(); j++)
	{
		Color color = trace(i, j);
		// Color color(0,1,0);
		color.clamp();
		camera->drawPixel(i, j, color);
	}

	if(++i == camera->getWidth())
	{
		i = 0;
		return true;
	}
	return false;
}

Color RenderEngine::Depth(Color color, Ray &primaryRay){
    //fixed aperture for lens
    float aperture = 2.0f;
    //distance to get fixed focal point
    float focal_dis = 15.0f;
    Camera *cam = camera;
    Vector3D eye = cam->get_position();
    //first ray from eye with fixed focal point
    Vector3D focal_point = eye + focal_dis * primaryRay.getDirection();
    //5 rays for distribution
    int rays = 5;
    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;
    for (int i = 0; i < rays; i++) {
        //distributed rays will be scattered about the lens of the camera
        random_device generator;
        uniform_real_distribution<double> distribution(0.0,aperture);
        float radius = distribution(generator);
        // std::cout << radius;
        random_device generator2;
        uniform_real_distribution<double> distrib(0.0,360.0);
        float phi = distrib(generator2);
        // std::cout << phi << std::endl;
        float x = eye.X()+radius*cos(phi*PI/180);
        float y = eye.Y()+radius*sin(phi*PI/180);
        float z = eye.Z();
        Vector3D origin = Vector3D(x,y,z);
        //create for every ray created
        Ray DOFRay(origin, focal_point-origin);
        Color c = world->shade_ray(DOFRay);
        R += c.r;
        G += c.g;
        B += c.b;
    }
    Color finalColor(R/rays, G/rays, B/rays);
    return finalColor;
}
