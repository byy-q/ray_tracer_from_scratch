#ifndef WALL_HPP
#define WALL_HPP

#include "objects.hpp"
#include <cmath>
#include <memory>

namespace Ray_tracer
{

class wall : public objects
{
public:
    wall(const vector3<double>& point, const vector3<double>& normal, material* mat_ptr)
        : point_(point), normal_(make_unit(normal)), mat_ptr_(mat_ptr) {}

    bool hit(
        const Ray& ray,
        const double& t_min,
        const double& t_max,
        std::shared_ptr<hit_record> result
    ) const override
    {
        const double denominator = dot_product(ray.__Direction(), normal_);
        if (std::abs(denominator) < 1e-8)
        {
            return false;
        }

        const double t = dot_product(point_ - ray.__Origin(), normal_) / denominator;
        if (t < t_min || t > t_max)
        {
            return false;
        }

        result->t = t;
        result->hit_point = ray.at_place_of(t);
        result->normal_vector = normal_;
        return true;
    }

    material_types Material() const override { return mat_ptr_->type(); }
    vector3<double> Absorb() const override { return mat_ptr_->Absorb(); }
    material* pointer_to_mat() const override { return mat_ptr_; }

private:
    vector3<double> point_;
    vector3<double> normal_;
    material* mat_ptr_;
};

}

#endif
