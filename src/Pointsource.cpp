
#include "Pointsource.h"
#include "Ray.h"

Pointsource::Pointsource(){

//
}

float Pointsource::pointsource_hit(Ray *r){

    glm::vec3 A=r->origin;
    glm::vec3 B=r->direction;
    glm::vec3 p=this->origin;

    p= p-A;

    
    std::vector<float> v={};

    for(int i=0; i<3; i++){
        if(B[i]==0){
            if(p[0]!=0){
                return -1.0;
            }
        }else{
            v.push_back(p[i]/B[i]);
        }
    }


    
    for(int i=1; i<v.size(); i++){
        if(v[i-1]!=v[i]){
            return -1.0;
        }
    }
    
    if(v.size()==0){
        return 0;
    }

    return v[0]; 
  }
