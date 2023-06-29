
#include "Plane.h"
#include "Ray.h"

Plane::Plane(){
    this->metal=false;
    this->transparent= false;
     this->F0=0.0;
     this->ref_index=0.0;

}

float my_dot(glm::vec3 a, glm::vec3 b){
    return a[0]*b[0]+ a[1]*b[1]+ a[2]*b[2];
}

float Plane::plane_hit(Ray *r){

    glm::vec3 A=r->origin;
    glm::vec3 B=r->direction;
    glm::vec3 n=this->normal;
    glm::vec3 po= this->po;
    float t1= my_dot(po-A, n);
    float t2= my_dot(B,n);

    if(t2==0){
        return -1.0;
    }

    return t1/t2; 
  }


glm::vec3 Plane::plane_hit_normal(Ray *r){

    
    return this->normal;

}