#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "vec.h"
#include <array>

inline float triArea(vec2 a, vec2 b, vec2 c){
	return ((c.x-a.x)*(b.y-a.y)-(b.x-a.x)*(c.y-a.y))/2;
}

inline std::array<float, 3> barycentric(vec2 p, std::array<vec2, 3> P){
    float areaTotal = triArea(P[0],P[1],P[2]);
    float alpha1 = 0,alpha2=0,alpha3=0;
    alpha1=triArea(p,P[1],P[2])/areaTotal;
    alpha2=triArea(P[0],p,P[2])/areaTotal;
    alpha3=1-alpha1-alpha2;
	return {alpha1,alpha2,alpha3};
}


#endif
