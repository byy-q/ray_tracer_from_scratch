#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>
#include "../data_structure/camera.hpp"
#include "../data_structure/vector.hpp"
#include "objects.hpp"
#include <memory>
namespace Ray_tracer
{




class sphere :public objects
{
    public:
        sphere(const vector3<double>& __center,const double&__radius,material* mat_PTR)
        :center(__center),radius(__radius),mater_ptr(mat_PTR){}
        sphere():center(vector3(0.0,0.0,0.0)),radius(0),mater_ptr(nullptr){}
        bool hit(const Ray& __ray,const double& t_min,const double& t_max,std::shared_ptr<hit_record>) const override;
        vector3<double> Center__()const{return center;}
        double Radius_()const{return radius;}
        material_types Material()const override{return mater_ptr->type(); }
        vector3<double> Absorb()const override{return mater_ptr->Absorb();}
        material* pointer_to_mat()const override{return mater_ptr;}
    private:
        vector3<double> center;
        double radius;
        material* mater_ptr;
};

bool sphere::hit(const Ray& __ray,const double& t_min,const double& t_max,std::shared_ptr<hit_record> result) const
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
            result->t = root;
            result->hit_point = __ray.at_place_of(root);
            result->normal_vector = make_unit(result->hit_point - center);
            return true;
        }
    }
    result->t = root;
    result->hit_point = __ray.at_place_of(root);        
    result->normal_vector = make_unit(result->hit_point - center);
    return true;
}







}
#endif