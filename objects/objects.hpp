#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../data_structure/vector.hpp"
#include <memory>
#include "../material/material.hpp"
#include "../hit_record.hpp"
namespace Ray_tracer
{
class objects
{
    public:
    /** 
     * @param : result records the hit point inforamtion of the hit point;
    */
        virtual bool hit(const Ray& __ray,const double& t_min,const double& t_ma,std::shared_ptr<hit_record> ) const = 0;
        virtual material_types Material()const = 0;
        virtual vector3<double> Absorb()const = 0;
        virtual material* pointer_to_mat()const = 0;
    private:

};


}

#endif