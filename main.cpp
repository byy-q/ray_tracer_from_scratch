#include "data_structure/camera.hpp"
#include "data_structure/vector.hpp"
#include "objects/objects.hpp"
#include "objects/sphere.hpp"
#include <fstream>
#include <limits>

Ray_tracer::pixel_num __nx = 2560;
Ray_tracer::pixel_num __ny = 2560;


Ray_tracer::camera __camera = Ray_tracer::camera(Ray_tracer::vector3(0.0,0.0,0.0),Ray_tracer::vector3(2.0,0.0,0.0),
Ray_tracer::vector3(0.0,1.0,0.0),2,30.0*3.14159265358979323846/180.0,
static_cast<double>(__nx)/__ny);

Ray_tracer::objects* list[3];


Ray_tracer::color __color_of_ray(const Ray_tracer::Ray __ray,Ray_tracer::objects** __world,const uint32_t& __size)
{
    bool hitted = false;
    Ray_tracer::color result = Ray_tracer::color(0,0,0);
    for(std::uint32_t i = 0; i < __size;i++)
    {
        double* position = new double(std::numeric_limits<double>::max());
        if(__world[i]->hit(__ray,0,*position,position))
        {
            hitted = true;
            result = __world[i]->Color_();
        }
    }
    if(hitted)
        return result;

    const Ray_tracer::vector3<double> direction = make_unit(__ray.__Direction());
    const double blend = 0.5 * (direction.__Y() + 1.0);
    return Ray_tracer::color(
        static_cast<std::uint8_t>(255.0 - 175.0 * blend),
        static_cast<std::uint8_t>(255.0 - 105.0 * blend),
        static_cast<std::uint8_t>(255.0 - 15.0 * blend));
}

int main()
{

    list[0] = new Ray_tracer::sphere(Ray_tracer::vector3(20.0,0.0,-10.0),4,Ray_tracer::color(255,0,0));
    list[1] = new Ray_tracer::sphere(Ray_tracer::vector3(7.0,0.0,0.0),3,Ray_tracer::color(0,255,0));
    list[2] = new Ray_tracer::sphere(Ray_tracer::vector3(14.0,0.0,10.0),4,Ray_tracer::color(0,0,255));
    std::ofstream image_file("images/P3.ppm");
    image_file<<"P3\n"<<__nx<<" "<<__ny<<"\n255\n";

    for(unsigned i = 0 ;i <__nx;i++)
    {
        for(unsigned j = 0;j <__ny;j++)
        {
            Ray_tracer::Ray __ray = __camera.get_ray(static_cast<double>(i)/static_cast<double>(__nx),static_cast<double>(j)/static_cast<double>(__ny));
            //、输出颜色
            Ray_tracer::color RGB = __color_of_ray(__ray,list,3);
            image_file<<RGB;
        }
    }

}
