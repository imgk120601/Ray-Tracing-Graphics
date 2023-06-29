
#include "Box.h"
#include "Ray.h"
#include <math.h>

Box::Box(){

    this->metal=false;
    this->transparent= false;
    this->F0=0.0;
    this->ref_index=0.0;


}

float my_dot2(glm::vec3 a, glm::vec3 b){
    return a[0]*b[0]+ a[1]*b[1]+ a[2]*b[2];
}

void my_swap(float &a, float &b){
    float t=a;
    a=b;
    b=t;

}

float Box::box_hit(Ray *r){

    glm::vec3 A= r->origin;
    
    glm::vec3 B=r->direction;
    glm::vec3 min_bound= this->min_bound;
    glm::vec3 max_bound =this->max_bound;

    float t0x = (min_bound[0] - A[0]) / B[0];
float t0y = (min_bound[0] - A[0]) / B[1]; 
float tmin = (t0x > t0y) ? t0x : t0y;

    float t1x = (max_bound[1] - A[1]) / B[0]; 
    float t1y = (max_bound[1] - A[1]) / B[1]; 
    float tmax = (t1x < t1y) ? t1x : t1y;

    if (t0x > t1y || t0y > t1x) return -1.0;

   // std::cout<<"jkl"<<std::endl;


    float t0z = (min_bound[2] - A[2]) / B[2]; 
    float t1z = (max_bound[2] - A[2]) / B[2]; 
     if (tmin > t1z || t0z > tmax) 
         return  -1.0; 
    if (t0z > tmin) tmin = t0z ;
    if (t1z < tmax) tmax = t1z ;



    return tmin;
 
  }



glm::vec3 Box::box_hit_normal(Ray *r){

    glm::vec3 normal={0.0,0.0,0.0};
    glm::vec3 A= r->origin;
    
    glm::vec3 B=r->direction;
    glm::vec3 min_bound= this->min_bound;
    glm::vec3 max_bound =this->max_bound;

    float t0x = (min_bound[0] - A[0]) / B[0];
float t0y = (min_bound[0] - A[0]) / B[1]; 

float tmin = (t0x > t0y) ? t0x : t0y;

    if(tmin==t0x){
        normal= {0.0,1.0,0.0};
    }else{
        normal= {1.0,0.0,0.0};
    }

    float t1x = (max_bound[1] - A[1]) / B[0]; 
    float t1y = (max_bound[1] - A[1]) / B[1]; 
    float tmax = (t1x < t1y) ? t1x : t1y;

    if (t0x > t1y || t0y > t1x) return {-1.0,-1.0,-1.0};

   // std::cout<<"jkl"<<std::endl;


    float t0z = (min_bound[2] - A[2]) / B[2]; 
    float t1z = (max_bound[2] - A[2]) / B[2]; 
   
     if (tmin > t1z || t0z > tmax) 
         return  {-1.0,-1.0,-1.0}; 
    if (t0z > tmin) tmin = t0z ;

    if(tmin==t0z){
        normal= {0.0,0.0,1.0};
    }
    if (t1z < tmax) tmax = t1z ;

    

    return normal;

  }