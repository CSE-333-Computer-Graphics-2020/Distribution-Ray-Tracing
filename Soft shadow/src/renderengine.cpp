#include "renderengine.h"
#include "world.h"
#include<iostream>
#include<string>
#include<map>
#include<random>
#include "utils.h"
using namespace std;


const Color RenderEngine::trace(const int i, const int j)
{
	Vector3D ray_dir = camera->get_ray_direction(i, j);
	Ray ray(camera->get_position(), ray_dir);
	return world->shade_ray(ray, 0);
}

bool RenderEngine::renderLoop()
{
	static int i = 0;
	for(int j = 0; j<camera->getHeight(); j++)
	{
		//Stratified Sampling
		Color color = Color(0.0, 0.0, 0.0);
		int n=4;
		for(int p=0; p<n-1; p++)
		{
			for(int q=0; q<n-1; q++)
			{
				random_device generator;
		        uniform_real_distribution<double> distribution(0.0,1.0);

		        double rand = distribution(generator);
		        int new_i = i + (p+rand)/n;
		        int new_j = j + (q+rand)/n;

		        color = color + trace(new_i, new_j);

			}
		}
    	color.R(color.R() / (n*n));
    	color.G(color.G() / (n*n));
    	color.B(color.B() / (n*n));
		//Color color = trace(i, j);
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
