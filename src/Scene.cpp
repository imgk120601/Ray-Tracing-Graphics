
#include "Scene.h"
#include "Box.h"
#include "Ray.h"
#include "Plane.h"
#include "Sphere.h"
#include "Pointsource.h"
#include <math.h>

Scene::Scene(){


    this->box_array={};
    this->plane_array={};
    this->pointsource_array={};
    this->sphere_array={};
       this->metal=false;
    this->transparent= false;
    this->ref_index=0.0;
}

float my_dot5(glm::vec3 a, glm::vec3 b){
    return a[0]*b[0]+ a[1]*b[1]+ a[2]*b[2];
}


float my_max5(float a, float b){
    if(a<b){
        return b;
    }

    return a;
}

void my_swap1(float &a, float &b){
    float t=a;
    a=b;
    b=t;

}


float Scene::scene_hit(Ray *r){

    float t=999999;
    this->my_i=-1;
    this->my_j=-1;
    this->normal={0.0,0.0,0.0};
    this->albedo={0.0,0.0,0.0};

    float tempt=0;
    for(int i=0; i<this->box_array.size(); i++){
        tempt=box_array[i]->box_hit(r);

        if(tempt>0){
            if(tempt<t){
                t= tempt;
                this->normal= box_array[i]->box_hit_normal(r);
                this->albedo=box_array[i]->albedo;
                this->metal= box_array[i]->metal;
                this->transparent= box_array[i]->transparent;
                this->F0= box_array[i]->F0;
                this->ref_index= box_array[i]->ref_index;
                my_i=0;
                my_j=i;
            }
    
         

        }

    }

    for(int i=0; i<this->sphere_array.size(); i++){
        tempt=sphere_array[i]->sphere_hit(r);

        if(tempt>0){
              if(tempt<t){
                t= tempt;
                this->normal= sphere_array[i]->sphere_hit_normal(r);
                this->albedo=sphere_array[i]->albedo;
                 this->metal= sphere_array[i]->metal;
                this->transparent= sphere_array[i]->transparent;
                this->F0= sphere_array[i]->F0;
                this->ref_index= sphere_array[i]->ref_index;
                my_i=1;
                my_j=i;
            }
        }

    }

    for(int i=0; i<this->plane_array.size(); i++){
        tempt=plane_array[i]->plane_hit(r);

        if(tempt>0){
            if(tempt<t){
                t= tempt;
                this->normal= plane_array[i]->plane_hit_normal(r);
                this->albedo=plane_array[i]->albedo;
                 this->metal= plane_array[i]->metal;
                this->transparent= plane_array[i]->transparent;
                 this->F0= plane_array[i]->F0;
                 this->ref_index= plane_array[i]->ref_index;
                 my_i=2;
                my_j=i;
            }
        }

    }

    // for(int i=0; i<this->pointsource_array.size(); i++){
    //     tempt=pointsource_array[i]->pointsource_hit(r);

    //     if(tempt>0){
    //         if(tempt<t){
    //             t= tempt;
    //             this->normal= {1.0,1.0,1.0};
    //             this->albedo={1.0,1.0,1.0};
    //              //my_i=2;
    //             //my_j=i;
    //         }
    //     }

    // }


    return t;
 
  }


float cos_theta(glm::vec3 a, glm::vec3 b){
    float temp= glm::length(a);
    a=a/temp;
    temp= glm::length(b);
    b=b/temp;
     return a[0]*b[0]+ a[1]*b[1]+ a[2]*b[2];
 }






//for light source colors
glm::vec3 Scene::hit_point_color(glm::vec3 hit_point, glm::vec3 normal, glm::vec3 albedo){



    
    float t=0;
    float temp=0;
    float temp1=0;
    float length=0;

    Ray *light_ray= new Ray();


    light_ray->origin=hit_point;
    float count=0;

    glm::vec3 color={0.0,0.0,0.0};
    glm::vec3 tempcolor={0.0,0.0,0.0};
    glm::vec3 incident_light= {0.0,0.0,0.0};
  
    for(int i=0; i<this->pointsource_array.size(); i++){
        light_ray->direction= this->pointsource_array[i]->origin-hit_point;

        length= glm::length(light_ray->direction);
       light_ray->direction= light_ray->direction/length;

        t=this->scene_hit(light_ray);

        //part 3-

        if(t==999999||t >0.0000001 && t <0.000001 ){
             
            incident_light= this->pointsource_array[i]->origin-hit_point;
            temp= glm::length(incident_light);
            incident_light=incident_light/temp;
            temp= glm::length(normal);

            normal =normal/temp;
            temp1= my_max5(0.0, cos_theta(incident_light,normal));

            tempcolor=this->pointsource_array[i]->color;
            tempcolor= tempcolor*temp1;
            //color= color*tempintensity;

            for(int j=0; j<3; j++){
                tempcolor[j]= albedo[j]*tempcolor[j];
             }

             color=color+tempcolor;
             count++;
        }else{
            //tempcolor= {0.0,0.0,1.0};
            //color= color+tempcolor;

        }

    }

    //color= color/count;

    return color;
}



glm::vec3 Scene::hit_point_color1(Ray* r , int depth , float old_ref_index){

    glm::vec3 hit_point={0.0,0.0,0.0};
    glm::vec3 normal= {0.0,0.0,0.0};
    glm::vec3 albedo= {0.0,0.0,0.0};

    glm::vec3 color= {0.0,0.0,0.0};

    bool metal= false;
    bool transparent=false;

    float length= glm::length(r->direction);
    r->direction= r->direction/length;
    float my_t= this->scene_hit(r);

    glm::vec3 reflected_ray_direction={0.0,0.0,0.0};
    glm::vec3 transmitted_ray_direction={0.0,0.0,0.0};

    Ray* reflected_ray= new Ray();
    Ray* transmitted_ray= new Ray();
    float temp= 0.0;

    float F=1.0;
    float F0=0.0;
    float ref_index=0.0;

    glm::vec3 tempcolor={0.0,0.0,0.0};

    glm::vec3 oldalbedo= {0.0,0.0,0.0};
    float temp1,temp2;


    if(my_t!=999999  ){
        
        F0= this->F0;
        metal= this->metal;
        transparent= this->transparent;
        normal= this->normal;
        temp= glm::length(normal);
        normal =normal/temp;
        
        albedo= this->albedo;
        oldalbedo= albedo;
        hit_point= r->origin + my_t*r->direction;

       color= color+ hit_point_color(hit_point, normal,albedo);

    

        if(metal==true && depth <2){
            
            F= F0 + (1.0-F0)*pow(1-cos_theta(r->direction,normal),5 );
            reflected_ray->origin= hit_point;
           reflected_ray->direction= (r->direction - 2*(cos_theta(r->direction,normal))*normal);



            tempcolor=F*hit_point_color1(reflected_ray,depth+1, ref_index);

            for(int i=0; i<3; i++){
                tempcolor[i]= oldalbedo[i]*tempcolor[i];
            }
            color= color+tempcolor;
        }


        if(transparent==true && depth<2){

            F0= pow((old_ref_index-ref_index)/ (old_ref_index+ref_index), 2);
            F= F0 + (1.0-F0)*pow(1-cos_theta(r->direction,normal),5 );

            reflected_ray->origin= hit_point;
            reflected_ray->direction= (r->direction - 2*(cos_theta(r->direction,normal))*normal);
            
            transmitted_ray->origin=hit_point;


            temp= cos_theta(r->direction,normal);
            temp1= old_ref_index/ref_index;
            temp2= sqrt(  pow(1/temp1,2)+ pow(temp,2)-1) - temp;


            transmitted_ray->direction= temp1*(temp*normal + r->direction);

            
        

            tempcolor= F*hit_point_color1(reflected_ray,depth+1,ref_index)+(1-F)*hit_point_color1(transmitted_ray,depth+1, ref_index);
            
            for(int i=0; i<3; i++){
                tempcolor[i]= oldalbedo[i]*tempcolor[i];
            }
            color= color+tempcolor;


        }

        

        return color;
    }

    
    return {0.53, 0.80, 0.92};

}


glm::vec3 Scene::scene_hit_normal(Ray *r){


    return this->normal;

  }