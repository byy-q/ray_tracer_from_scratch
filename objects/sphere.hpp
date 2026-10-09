#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>
#include "../data_structure/camera.hpp"
#include "../data_structure/vector.hpp"
#include "objects.hpp"

namespace Ray_tracer
{




class sphere :public objects
{
    public:
        sphere(const vector3<double>& __center,const double&__radius,const color& __color)
        :center(__center),radius(__radius),surface_color(__color){}
        sphere():center(vector3(0.0,0.0,0.0)),radius(0),surface_color(color(255,255,255)){}
        bool hit(const Ray& __ray,const double& t_min,const double& t_max,double* result) const override;
        vector3<double> Center__()const{return center;}
        double Radius_()const{return radius;}
        color Color_()const override{return surface_color;}
    private:
        vector3<double> center;
        double radius;
        color surface_color;        
};

bool sphere::hit(const Ray& __ray,const double& t_min,const double& t_max,double* result) const
{
    const vector3<double> oc = __ray.__Origin() - center;
    const double a = dot_product(__ray.__Direction(), __ray.__Direction());
    const double half_b = dot_product(oc, __ray.__Direction());
    const double c = dot_product(oc, oc) - static_cast<double>(radius * radius);
    const double discriminant = half_b * half_b - a * c;

    if (discriminant < 0.0)
    {
        return false;
    }

    const double sqrt_discriminant = std::sqrt(discriminant);
    double root = (-half_b - sqrt_discriminant) / a;

    if (root < t_min || root > t_max)
    {
        root = (-half_b + sqrt_discriminant) / a;
        if (root < t_min || root > t_max)
        {
            return false;
        }
        else
        {
            *result = root;
            return true;
        }
    }
    *result = root;
    return true;
}







}
#endif