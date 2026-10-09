#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../data_structure/vector.hpp"

namespace Ray_tracer
{
class objects
{
    public:
        virtual bool hit(const Ray& __ray,const double& t_min,const double& t_ma,double* result ) const = 0;
        virtual color Color_()const = 0;
};


}

#endif