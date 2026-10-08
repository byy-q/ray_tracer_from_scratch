/** 
 * @brief : this is the vector for the color and the coordinates used in render
 * @details : we use 24 bits RGB color model,so the (255,0,0) represented for red,(255,255,255) represented for white

*/
#ifndef RAY_VECTOR
#define RAY_VECTOR

#include <cstdint>
#include <math.h>
#include <type_traits>
namespace Ray_tracer
{

template<typename T = float>
class vector3
{
    template<typename > friend class vector3;
    public:
        vector3() = default;
        vector3(const T& _X,const T& _Y,const T& _Z):x(_X),y(_Y),z(_Z){}
        ~vector3()=default;
        
        template<typename U>
        vector3(const vector3<U>& copy)
        {
            x = static_cast<T>(copy.x);
            y = static_cast<T>(copy.y);
            z = static_cast<T>(copy.z);
        }

        
        template<typename U>
        vector3<T>& operator=(const vector3<U>& copy)
        {
            x = static_cast<T>(copy.x);
            y = static_cast<T>(copy.y);
            z = static_cast<T>(copy.z);
            return *this;
        }

        
        double length() const
        {
            return static_cast<double>(sqrt(x*x + y*y + z*z));
        }

        template<typename U>
        friend std::common_type_t<T,U> dot_product(const vector3<T>& _v1 ,const vector3<U>& _v2);

        template<typename U>
        friend vector3<std::common_type_t<T,U>> cross_product(const vector3<T>& _v1,const vector3<U>& _v2);

        template<typename U>
        vector3<T>& operator+=(const vector3<U>& _v2)
        {
            x += static_cast<T>(_v2.x); y += static_cast<T>(_v2.y); z += static_cast<T>(_v2.z);
            return *this;
        }

        template<typename U>
        friend auto operator+(const vector3<T>& _v1,const vector3<U>& _v2)-> vector3<std::common_type_t<T,U>>
        {
            using result = std::common_type_t<T,U>;
            return vector3<result>(static_cast<result>(_v1.x)+static_cast<result>(_v2.x),static_cast<result>(_v1.y)+
            static_cast<result>(_v2.y),static_cast<result>(_v1.z)+static_cast<result>(_v2.z));
        }

        template<typename U>
        vector3<T>& operator-=(const vector3<U>& _v2)
        {
            x -= static_cast<T>(_v2.x); y -= static_cast<T>(_v2.y); z -= static_cast<T>(_v2.z);
            return *this;
        }
        template<typename U>
        friend auto operator-(const vector3<T>& _v1,const vector3<U>& _v2) -> vector3<std::common_type_t<T,U>>
        {
            using result = std::common_type_t<T,U>;
            return vector3<result>(static_cast<result>(_v1.x)-static_cast<result>(_v2.x),static_cast<result>(_v1.y)-
            static_cast<result>(_v2.y),static_cast<result>(_v1.z)-static_cast<result>(_v2.z));
        }       
        template<typename U>
        vector3<T>& operator*=(const U& m)
        {
            x*=static_cast<T>(m);y*=static_cast<T>(m);z*=static_cast<T>(m);
            return *this;
        }
        template<typename U>
        friend auto operator*(const vector3& _v,const U& m) ->vector3<std::common_type_t<T,U>>
        {
            using result = std::common_type_t<T,U>;
            return vector3<result>(static_cast<result>(_v.x)*static_cast<result>(m),
        static_cast<result>(_v.y)*static_cast<result>(m),static_cast<result>(_v.z)*static_cast<result>(m));
        }
        template<typename U>
        vector3<T>& operator/=(const U &m)
        {
            x/=static_cast<T>(m);y/=static_cast<T>(m);z/=static_cast<T>(m);
        }
        template<typename U>
        friend auto operator/(const vector3<T>&_v,const U& m) -> vector3<std::common_type_t<T,U>>
        {
            using result = std::common_type_t<T,U>;
            return vector3<result>(static_cast<result>(_v.x)/static_cast<result>(m),
        static_cast<result>(_v.y)/static_cast<result>(m),static_cast<result>(_v.y)/static_cast<result>(m));
        }

        friend vector3<T> make_unit(const vector3<T>& __v)
        {
            double Length = __v.length();
            return vector3<T>(__v.x/Length,__v.y/Length,__v.z/Length);
        }
    private:
        T x = 0;
        T y = 0;
        T z = 0;

};

template<typename U,typename T>
std::common_type_t<T,U> dot_product(const vector3<T>& _v1,const vector3<U>& _v2)
{
    using result = std::common_type_t<T,U>;
    return static_cast<result>(_v1.x)*static_cast<result>(_v2.x)+ static_cast<result>(_v1.y)*static_cast<result>(_v2.y)+ 
    static_cast<result>(_v1.z)*static_cast<result>(_v2.z);
}

template<typename U,typename T>       
vector3<std::common_type_t<T,U>> cross_product(const vector3<T>& _v1,const vector3<U>& _v2)
{
    using result = std::common_type_t<T,U>;
    return vector3<result>(
        static_cast<result>(_v1.y) * static_cast<result>(_v2.z) - static_cast<result>(_v1.z) * static_cast<result>(_v2.y),
        static_cast<result>(_v1.z) * static_cast<result>(_v2.x) - static_cast<result>(_v1.x) * static_cast<result>(_v2.z),
        static_cast<result>(_v1.x) * static_cast<result>(_v2.y) - static_cast<result>(_v1.y) * static_cast<result>(_v2.x));
}

class color 
{
    public:
        template<typename U,std::enable_if_t<
        std::is_same<U,int>::value || std::is_same<U,long>::value ||std::is_same<U,short>::value ||
        std::is_same<U,unsigned>::value ||std::is_same<U,unsigned short>::value ||std::is_same<U,unsigned long>::value,int> = 0>
        explicit color(const vector3<U>& value)
        {
            if(value.x < 0) red = 0;
            else if(value.x > 255) red = 255;
            else red = value.x;
            
            if(value.y < 0) green = 0;
            else if(value.y > 255) green = 255;
            else green = value.y;           
            
            if(value.z < 0) blue = 0;
            else if(value.z > 255) blue = 255;
            else blue = value.x;
        }

        color(const uint8_t& r,const uint8_t& g,const uint8_t& b):red(r),green(g),blue(b){}
        ~color()=default;

        color(const color& copy){red = copy.red;green = copy.green;blue = copy.blue;}
        color& operator=(const color& copy)
        {
            red = copy.red;
            green = copy.green;
            blue = copy.blue;
            return *this;
        }

        color& operator+=(const color& copy)
        {
            red = red+copy.red>255?255:red+copy.red;
            blue = blue+copy.blue>255?255:blue+copy.blue; 
            green = green+copy.green>255?255:green+copy.green;
            return *this;
        }

        friend color operator+(const color&__c1,const color&__c2)
        {
            color result = __c1;
            result+=__c2;
            return result;
        }

        color& operator-=(const color& __c2)
        {
            red = red-__c2.red > red ? 0 : red-__c2.red;
            green = green-__c2.green > green ? 0 : green-__c2.green;
            blue = blue-__c2.blue > blue ? 0 : blue-__c2.blue;
            return *this;
        }

        friend color operator-(const color&__c1,const color&__c2)
        {
            color result = __c1;
            result-=__c2;
            return result;
        }

        color& operator*=(const std::uint8_t& m)
        {
            red = red*m>255?255:red*m;
            green = green*m>255?255:green*m;
            blue = blue*m>255?255:blue*m;
            return *this;
        }
        friend color operator*(const color&__c1,const std::uint8_t& m)
        {
            color result = __c1;
            result *= m;
            return result;
        }

    private:
        std::uint16_t red;
        std::uint16_t green;
        std::uint16_t blue;
};


class Ray
{
    public:
        Ray(const vector3<double>& __origin,const vector3<double>& __direction):origin(__origin),direction(__direction){}
        vector3<double> __Origin()const{return origin;}
        vector3<double> __Direction()const{return direction;}
        vector3<double> at_place_of(const double& t){
            return origin + direction*t;
        }


    private:
        vector3<double> origin = vector3<double>(0.0,0.0,0.0);
        vector3<double> direction = vector3<double>(0.0,0.0,0.0);
};


}
#endif