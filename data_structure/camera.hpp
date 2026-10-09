#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "vector.hpp"
#include <math.h>

namespace Ray_tracer
{
class camera 
{
    public:
        camera(const vector3<double>& __look_f,const vector3<double>& __look_a,const vector3<double>& view_up,const int& distance,const double& ang,const double& ratio)
        :look_from(__look_f),look_at(__look_a),view_up(view_up),distance_from_camera(distance),half_angle(ang),w_h_ratio(ratio)
        {
            half_height = distance_from_camera * tan(half_angle);
            half_width = half_height * w_h_ratio;

            vector3<double> forward = make_unit(look_at - look_from);
            vector3<double> verti_partion = view_up - forward*dot_product(view_up,forward);

            left_down_corner = look_from + forward*distance + make_unit(cross_product(verti_partion,forward))*half_width-make_unit(verti_partion)*half_height;
            vertical = make_unit(verti_partion)*2*half_height;
            horizontal = make_unit(cross_product(forward,verti_partion))*2*half_width;
            
        }
        camera() =default;
        
        Ray get_ray(const double& u,const double& v)const{return Ray(look_from,left_down_corner + vertical*u + horizontal*v);}
        vector3<double> look_FR()const{return look_from;}
  
    private:    
        vector3<double> look_from;
        vector3<double> look_at;

        vector3<double> view_up;

        int distance_from_camera;
        double half_angle;
        double w_h_ratio;

        double half_height;
        double half_width;

        vector3<double> left_down_corner;
        vector3<double> vertical ;
        vector3<double> horizontal; 
};

using pixel_num = std::uint32_t;


}
#endif