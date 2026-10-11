#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "../data_structure/vector.hpp"
#include "../random.hpp"
#include"../hit_record.hpp"
namespace Ray_tracer
{

    enum material_types {LAMBERTIAN};
class material
{
    public:

    virtual material_types type()const = 0;
    virtual vector3<double> Absorb()const = 0;
    virtual Ray scattered_ray(const Ray& incoming,std::shared_ptr<hit_record> ptr)const = 0;

};

class lambertian : public material
{
    public:

        lambertian(const vector3<double> albedo):albedo(albedo){}
        material_types type()const override{return material_types::LAMBERTIAN;}
        vector3<double> Absorb()const override{return albedo;}
        /** 
         * @return :return the ray after scatter
        */
        Ray scattered_ray(const Ray& incoming, std::shared_ptr<hit_record> ptr)const override{
            vector3<double> normal = ptr->normal_vector;
            if (dot_product(incoming.__Direction(), normal) > 0.0)
            {
                normal *= -1.0;
            }

            vector3<double> random_in_sphere;
            do
            {
                random_in_sphere = vector3<double>(
                    2.0 * random01(rng) - 1.0,
                    2.0 * random01(rng) - 1.0,
                    2.0 * random01(rng) - 1.0
                );
            } while (dot_product(random_in_sphere, random_in_sphere) >= 1.0);

            vector3<double> direction = normal + random_in_sphere;
            if (dot_product(direction, direction) < 1e-12)
            {
                direction = normal;
            }

            return Ray(ptr->hit_point + normal * 1e-4, direction);
        }


    private:
        vector3<double> albedo;       
};


};



#endif