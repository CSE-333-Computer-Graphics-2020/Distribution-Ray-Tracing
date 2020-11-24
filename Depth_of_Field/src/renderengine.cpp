#include "renderengine.h"
#define PI 3.14

const Color RenderEngine::trace(const int i, const int j)
{
	Vector3D ray_dir = camera->get_ray_direction(i, j);
	Ray ray(camera->get_position(), ray_dir);
	// return world->shade_ray(ray, 0);
	return Depth(world->shade_ray(ray), ray);
}

bool RenderEngine::renderLoop()
{
	static int i = 0;
	for(int j = 0; j<camera->getHeight(); j++)
	{
		Color color = trace(i, j);
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
        float radius = (float)(rand())/(float)(RAND_MAX/aperture);
        float phi = (float)(rand())/(float)(RAND_MAX/360);
        Vector3D origin = Vector3D(eye.X()+radius*cos(phi*PI/180),eye.Y()+radius*sin(phi*PI/180),eye.Z());
        //create for every ray created
        Ray DOFRay(origin, focal_point-origin);
        Color c = world->shade_ray(DOFRay);
        c.clamp();
        R += c.r;
        G += c.g;
        B += c.b;
    }
    Color finalColor(R/rays, G/rays, B/rays);
    return finalColor;
}
