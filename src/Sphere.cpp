
#include "Sphere.h"
#include "Ray.h"
#include <math.h>

Sphere::Sphere(){

    this->metal=false;
    this->transparent= false;
    this->F0=0.0;
    this->ref_index=0.0;


}

float my_dot1(glm::vec3 a, glm::vec3 b){
    return a[0]*b[0]+ a[1]*b[1]+ a[2]*b[2];
}

float Sphere::sphere_hit(Ray *r) {

    glm::vec3 center= this->center;
    float radius= this->radius;
    glm::vec3 oc = r->origin -center;


    float a = my_dot1(r->direction, r->direction);
    float b = 2.0 * my_dot1(oc, r->direction);
    float c = my_dot1(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;

    float d, e;
    float my_t;
    float length;


    if(discriminant >= 0){
        
        discriminant= sqrt(discriminant);

        d=( (-b+discriminant)/2*a);
        e=( (-b-discriminant)/2*a);
        if( d>=0 || e>=0){

            

            if(d<e){
                my_t=d;
            }else{
                my_t=e;
        
            }
            // //also calculate normal
            // this->normal=r->origin+ my_t*(r->direction);
            // this->normal= this->normal-this->center;
            // length= glm::length(this->normal);
            // this->normal=this->normal/length;


            return my_t;

        }else{
            return -1.0;
        }

    }

    return -1.0;

}

glm::vec3 Sphere::sphere_hit_normal(Ray *r){

    float my_t= this->sphere_hit(r);
    glm::vec3 normal={0.0,0.0,0.0};

    normal=r->origin+ my_t*(r->direction);
    normal= normal-this->center;
    float length= glm::length(normal);
    normal=normal/length;

    return normal;

}