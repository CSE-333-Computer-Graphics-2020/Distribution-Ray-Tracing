#include "ray.h"
#include <iostream>
#include<random>

bool Ray::setParameter(const float par, const Object *obj, const Vector3D n)
{
    if(par < t && par > SMALLEST_DIST)
    {
        hit = true;
        t = par;
        normal = n;
        object = obj;
        return true;
    }
    return false;
}
