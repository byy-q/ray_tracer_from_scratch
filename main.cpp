#include "data_structure/camera.hpp"
#include "data_structure/vector.hpp"
#include "objects/objects.hpp"
#include "objects/sphere.hpp"
#include "objects/wall.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <limits>
#include "light_source/light.hpp"
#include <memory>
#include <random>
#include "random.hpp"

std::mt19937 rng(std::random_device{}());
std::uniform_real_distribution<double> random01(0.0, 1.0);

Ray_tracer::pixel_num __nx = 1280;
Ray_tracer::pixel_num __ny = 720;


Ray_tracer::camera __camera = Ray_tracer::camera(Ray_tracer::vector3(17.0,7.0,34.0),Ray_tracer::vector3(17.0,-1.0,0.0),
Ray_tracer::vector3(0.0,1.0,0.0),2,30.0*3.14159265358979323846/180.0,
static_cast<double>(__nx)/__ny);

Ray_tracer::objects* list[4];
Ray_tracer::envir_light global_light({42,42,42});
Ray_tracer::light* light_source[2];


bool check_hit(const Ray_tracer::Ray& __ray,const double& t_min,const double& t_max,Ray_tracer::objects** __world,size_t size)
{
    std::shared_ptr<hit_record> place_holder(new hit_record);
    for(size_t i = 0; i < size;i++)
    {
        if(__world[i]->hit(__ray,t_min,t_max,place_holder))
            return true;
    }
    return false;
}



Ray_tracer::color __color_of_ray(const Ray_tracer::Ray __ray,Ray_tracer::objects** __world,const uint32_t& __size,Ray_tracer::light** light_source,size_t light_num)
{
    using namespace Ray_tracer;

    std::shared_ptr<hit_record> __hitting(new hit_record);
    int index = -1;
    double closest_t = std::numeric_limits<double>::max();
    for(size_t i = 0 ;i < __size;i++)
    {
        std::shared_ptr<hit_record> candidate(new hit_record);
        if(__world[i]->hit(__ray,0.01,closest_t,candidate))
        {    
            closest_t = candidate->t;
            __hitting = candidate;
            index = i;
        }
    }
    if(index < 0)
    {
        const vector3<double> direction = make_unit(__ray.__Direction());
        const double blend = 0.5 * (direction.__Y() + 1.0);
        return color(
            static_cast<std::uint8_t>(178.0 + blend * (105.0 - 178.0)),
            static_cast<std::uint8_t>(218.0 + blend * (177.0 - 218.0)),
            static_cast<std::uint8_t>(238.0 + blend * (232.0 - 238.0))
        );
    }
    //now the hit_record __hitting contains the nearest point's infomation
    /** 
     * @param ;world[index] now points to the hit obeject
    */

    //check the hit pointer whether can be illuminate by light source

    color result = global_light.get_color() * __world[index]->Absorb();
    


    //now we continue to trace the scatter ray
    size_t depth = 50;
    switch (__world[index]->Material())
    {
    case material_types::LAMBERTIAN:
    {    //the hit object occur the diffuse
        for(size_t i = 0;i < light_num;i++)
        {
            Ray light_ray = light_source[i]->get_ray(__hitting->hit_point);
            const vector3<double> light_direction =
                make_unit(light_ray.__Direction());
            const vector3<double> normal = __hitting->normal_vector;
            const double light_distance = light_ray.__Direction().length();

            if (light_distance <= 1e-4 ||
                dot_product(normal, light_direction) <= 0.0)
            {
                continue;
            }

            constexpr double shadow_epsilon = 1e-4;
            Ray shadow_ray(
                __hitting->hit_point + normal * shadow_epsilon,
                light_direction
            );
            const double max_shadow_distance =
                light_source[i]->Light_Type() == POINT
                    ? light_distance - shadow_epsilon
                    : std::numeric_limits<double>::max();

            if(!check_hit(shadow_ray,shadow_epsilon,max_shadow_distance,__world,__size))
            {
                result += light_source[i]->get_color() * __world[index]->Absorb();
            }
        }
        Ray scattered_ray = __world[index]->pointer_to_mat()->scattered_ray(__ray,__hitting);//a placeholder here,temporary
        depth--;
        if(depth>0)
             return result + (__color_of_ray(scattered_ray,__world,__size,light_source,light_num)) * __world[index]->Absorb();   
        else
            return {0,0,0};
        
    }

    case material_types::MIRROR:
    {
        Ray reflected_ray = __world[index]->pointer_to_mat()->scattered_ray(__ray,__hitting);
        depth--;

        if(depth <= 0)
        {
            return {0,0,0};
        }

        constexpr double shadow_epsilon = 1e-4;
        constexpr double direction_tolerance = 1e-6;
        constexpr double specular_exponent = 8.0;
        const vector3<double> normal = __hitting->normal_vector;
        const vector3<double> reflected_direction =
            make_unit(reflected_ray.__Direction());
        color direct_light(0, 0, 0);

        for (size_t i = 0; i < light_num; ++i)
        {
            Ray light_ray = light_source[i]->get_ray(__hitting->hit_point);
            const vector3<double> light_direction =
                make_unit(light_ray.__Direction());

            if (light_source[i]->Light_Type() == POINT)
            {
                const double light_distance = light_ray.__Direction().length();
                if (light_distance <= shadow_epsilon ||
                    dot_product(normal, light_direction) <= 0.0)
                {
                    continue;
                }

                Ray shadow_ray(
                    __hitting->hit_point + normal * shadow_epsilon,
                    light_direction
                );
                if (check_hit(
                        shadow_ray,
                        shadow_epsilon,
                        light_distance - shadow_epsilon,
                        __world,
                        __size))
                {
                    continue;
                }

                const double alignment =
                    std::max(0.0, dot_product(reflected_direction, light_direction));
                const double highlight = std::pow(alignment, specular_exponent);
                direct_light += light_source[i]->get_color() * highlight *
                    __world[index]->Absorb();
                continue;
            }

            if (dot_product(reflected_direction, light_direction) >=
                    1.0 - direction_tolerance &&
                !check_hit(
                    reflected_ray,
                    shadow_epsilon,
                    std::numeric_limits<double>::max(),
                    __world,
                    __size))
            {
                direct_light += light_source[i]->get_color() *
                    __world[index]->Absorb();
            }
        }

        return (direct_light +
            __color_of_ray(reflected_ray,__world,__size,light_source,light_num)) *
             __world[index]->Absorb();
    }
    default:
        return result;

}
}

int main()
{

    list[0] = new Ray_tracer::sphere(
        Ray_tracer::vector3(9.0,-4.0,0.0),
        2.0,
        new Ray_tracer::lambertian({0.25,0.42,0.55})
    );
    list[1] = new Ray_tracer::sphere(
        Ray_tracer::vector3(16.0,-3.0,0.0),
        3.0,
        new Ray_tracer::lambertian({0.68,0.48,0.28})
    );
    list[2] = new Ray_tracer::sphere(
        Ray_tracer::vector3(26.0,0.0,0.0),
        6.0,
        new Ray_tracer::mirror({0.82,0.84,0.88})
    );
    list[3] = new Ray_tracer::wall(
        Ray_tracer::vector3(0.0,-6.0,0.0),
        Ray_tracer::vector3(0.0,1.0,0.0),
        new Ray_tracer::lambertian({0.68,0.68,0.68})
    );

    light_source[0] = new Ray_tracer::point_light({150,135,120},{2.0,14.0,14.0});
    light_source[1] = new Ray_tracer::parallel_light({28,32,45},{-1.0,2.0,1.0});


    std::ofstream image_file("images/P3.ppm");
    image_file<<"P3\n"<<__nx<<" "<<__ny<<"\n255\n";

    for(unsigned j = 0 ;j <__ny;j++)
    {
        for(unsigned i = 0;i <__nx;i++)
        {
            Ray_tracer::Ray __ray = __camera.get_ray(
                static_cast<double>(i) / static_cast<double>(__nx - 1),
                1.0 - static_cast<double>(j) / static_cast<double>(__ny - 1)
            );
            //、输出颜色
            Ray_tracer::color RGB = __color_of_ray(__ray,list,4,light_source,2);
            image_file<<RGB;
        }
    }

}
