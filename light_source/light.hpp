#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../data_structure/vector.hpp"
namespace Ray_tracer
{
class light
{
    public:
        light()=default;
        virtual ~light() = default;
        virtual color get_color()const = 0;
        virtual Ray get_ray(const vector3<double>& p)const = 0;

};


class point_light : public light
{
    public:
        point_light(const color& lig_color,const vector3<double>& origin):__light_color(lig_color),light_point(origin){}
        point_light(const vector3<double>& origin):light_point(origin){}
        ~point_light()=default;
        Ray get_ray(const vector3<double>& __coord){return Ray(light_point,__coord);}
        color get_color()const override{return __light_color;}
        virtual Ray get_ray(const vector3<double>& p)const override{
            return Ray(p,light_point-p);
        }
    private:
        color __light_color = color(128,128,128);
        vector3<double> light_point = {0.0,0.0,0.0};
};

class parallel_light : public light
{
    public:
        parallel_light():__light_color(0,0,0),direction({1,1,1}){};
        parallel_light(const color& _co,const vector3<double>& dir):__light_color(_co),direction(dir){}
        ~parallel_light()=default;
        color get_color()const override{return __light_color;}
        Ray get_ray(const vector3<double>& coord)const override{return Ray(coord,direction);}
    private:
        color __light_color;
        vector3<double> direction;
};


class envir_light 
{
    public:
        envir_light():__color({128,128,128}){}
        envir_light(const color& color):__color(color){}
        color get_color()const {return __color;}
    private:
        color __color;
};
}


#endif