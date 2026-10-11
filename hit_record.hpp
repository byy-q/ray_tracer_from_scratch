#ifndef HIT_RECORD_HPP
#define HIT_RECORD_HPP

#include "data_structure/vector.hpp"

typedef struct
{
    
    Ray_tracer::vector3<double> hit_point;
    double t;
    Ray_tracer::vector3<double> normal_vector;
} hit_record;


#endif