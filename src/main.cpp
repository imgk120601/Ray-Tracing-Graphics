// #pragma once
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <math.h>

#include <glm/geometric.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
//#include "Ray.h"
//#include "Sphere.h"
//#include "Plane.h"
//#include "Box.h"
//#include "Scene.h"
#include "headers.h"
//#include "Pointsource.h"


class SoftwareRasterization{
public:    
    int frameWidth = 300;
    int frameHeight = 300;
    int displayScale = 2;

    //for supersampling-> framewidth 20 , frame height-20 , display scace-19
    SDL_Surface *framebuffer = NULL;
    SDL_Surface *windowSurface = NULL;
    SDL_Window  *window = NULL;
    bool quit = false;


    SoftwareRasterization() {
        if(!initialize()) std::cout << "failed to initialize SDL" << std::endl;
    }
    ~SoftwareRasterization() {
        terminate();
    }
   

    // clear the framebuffer with some color
    void clearFrameBuffer() {        
        // set the pixels in the frambuffer 

        glm::vec4 color(0.00, 0.00, 0.00, 1.0);

        Uint32 *pixels = (Uint32*)framebuffer->pixels;
        SDL_PixelFormat *format = framebuffer->format;        
        Uint32 rgba = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);
        for(int i=0; i<frameWidth; i++) {
            for(int j=0; j<frameHeight; j++) {
                pixels[i + frameWidth*j] = rgba; // set new color
            }
        }        
    }


   float my_dotproduct(glm::vec3 a, glm::vec3 b){
     return a[0]*b[0]+ a[1]*b[1]+ a[2]*b[2];
   }

// color ray_color(const ray& r) {
//     if (hit_sphere(point3(0,0,-1), 0.5, r))
//         return color(1, 0, 0);
//     vec3 unit_direction = unit_vector(r.direction());
//     auto t = 0.5*(unit_direction.y() + 1.0);
//     return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
// }

float my_min(float a, float b){
    if(a<=b){
        return a;
    }

    return b;
}

float my_max(float a, float b){
    if(a<b){
        return b;
    }

    return a;
}

float Convert_sRGB_FromLinear (float theLinearValue) {
  return theLinearValue <= 0.0031308f
       ? theLinearValue * 12.92f
       : powf (theLinearValue, 1.0f/2.4f) * 1.055f - 0.055f;
}

void gamma_correction(glm::vec3 &color){

    float gamma = 2.2;
    color = pow(color, glm::vec3(1.0/gamma));
}

void sRGB_gamma_correction(glm::vec3 &color){

    for(int i=0;i<3; i++){
        color[i]= Convert_sRGB_FromLinear(color[i]);
    }

    //gamma_correction(color);
}

    void raytracing1() { 
        glm::vec4 color= {0.0,0.0,0.0,1.0};       
        Uint32 *pixels = (Uint32*)framebuffer->pixels;
        SDL_PixelFormat *format = framebuffer->format;
        Uint32 triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);
        Uint32 bgColor  = SDL_MapRGBA(format, 255, 255, 255, 1);    

        
        Ray *r= new Ray();
        
        Sphere *s1= new Sphere();
        s1->center= {0.0,0.0,-2.0};
        s1->radius=1.0;

        Sphere *s2= new Sphere();
        s2->center={0.0,-101.0,-2.0};
        s2->radius=100;

        float x,y;    
        glm::vec3 vec3origin={0.0,0.0,0.0};
        glm::vec3 tempvec3= {0.0,0.0,0.0};
        float tempx , tempy;

        float my_t , my_t1, my_t2;

        glm::vec3 normal={0.0,0.0,0.0};
        float length=0.0;
        float two=2.0;
        

        
        for(int i=0; i<frameWidth; i++) {
            for(int j=0; j<frameHeight; j++) {        
                
                x=i+0.5;
                y=j+0.5;

                tempx= (-1 + x*2/frameWidth)*1;
                tempy= (-1 + y*2/frameHeight)*1;
                r->origin=vec3origin;
                tempvec3[0]=tempx;
                tempvec3[1]=tempy;
                tempvec3[2]=-1.0;
                r->direction= tempvec3;
                //hit_sphere(centre,100.0, r) || hit_sphere(centre1, 1, r)
                my_t1=s1->sphere_hit(r);
                my_t2=s2->sphere_hit(r);

                if(my_t1>=0 && my_t2>=0){
                    my_t= my_min(my_t1,my_t2);
                }else{
                    my_t=my_max(my_t1,my_t2);
                }
                if(my_t>=0 ){
                   // std::cout<<"yes"<<std::endl;
                  
                    normal={0.0,0.0,0.0};
                   if(my_t== my_t1){
                     normal=r->origin+ my_t*(r->direction);
                    normal= normal-s1->center;
                   }else if(my_t==my_t2){
                    normal= normal-s2->center;
                   }
                   length= glm::length(normal);
                   normal=normal/length;

                   normal= (normal+glm::vec3(1.0,1.0,1.0));
                   normal=normal/two;
                   color= glm::vec4(normal,1.0);
                  triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);

                     pixels[i + (frameHeight-j-1)*frameWidth]=triangleColor;
                }else{
                    //std::cout<<"NO"<<std::endl;
                }
                
            }
        }        
    

    }
    


    void raytracing2() { 
        glm::vec4 color= {1.0,0.5,0.7,1.0};       
        Uint32 *pixels = (Uint32*)framebuffer->pixels;
        SDL_PixelFormat *format = framebuffer->format;
        Uint32 triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);
        Uint32 bgColor  = SDL_MapRGBA(format, 255, 255, 255, 1);    

        Ray *r= new Ray();
        
        Sphere *s1= new Sphere();
        s1->center= {0.0,0.0,-2.0};
        s1->radius=1.0;

        Plane *p1= new Plane();
        p1->normal= {0.0,1.0,0.0};
        p1->po= {0.0,-1.4,0.0};

        Box *b1= new Box();
        b1->min_bound= {2.0,2.0,-4.0};
        b1->max_bound= {2.0,3.0,-5.0};


        float x,y;    
        glm::vec3 vec3origin={0.0,0.0,0.0};
        glm::vec3 tempvec3= {0.0,0.0,0.0};
        float tempx , tempy;

        float my_t , my_t1, my_t2,my_t3;

        my_t =-1.0;
        glm::vec3 normal={0.0,0.0,0.0};
        float length=0.0;
        float two=2.0;

        

        
        for(int i=0; i<frameWidth; i++) {
            for(int j=0; j<frameHeight; j++) {        
                
                x=i+0.5;
                y=j+0.5;

                tempx= (-1 + x*2/frameWidth)*1;
                tempy= (-1 + y*2/frameHeight)*1;
                r->origin=vec3origin;
                tempvec3[0]=tempx;
                tempvec3[1]=tempy;
                tempvec3[2]=-1.0;
                r->direction= tempvec3;
                //hit_sphere(centre,100.0, r) || hit_sphere(centre1, 1, r)
                 my_t1=s1->sphere_hit(r);
                 my_t2=p1->plane_hit(r);
                my_t3= b1->box_hit(r);

                my_t= -1.0;

                if(my_t1>=0){
                    my_t=my_t1;
                }

                if(my_t2>=0){
                    if(my_t==-1.0){
                        my_t= my_t2;
                    }else{
                         //std::cout<<my_t2<<" "<<my_t1<<std::endl;
                           my_t= my_min(my_t,my_t2);
                    }
                 
                }

                if(my_t3>=0){
                    if(my_t==-1.0){
                        my_t= my_t3;
                    }else{
                           my_t= my_min(my_t,my_t3);
                    }
                 
                }
            

               // std::cout<<my_t<<std::endl;
                if(my_t>=0 ){
                   // std::cout<<"yes"<<std::endl;
                    
                    normal = {0.0,0.0,0.0};

                    if(my_t== my_t1){
                    normal=r->origin+ my_t*(r->direction);
                     normal= normal-s1->center;
                    }else if(my_t==my_t2){

                        //if(my_t1!=-1){
                         //     std::cout<<my_t2<<" "<<my_t1<<std::endl;
                        //}
                      
                             //std::cout<<"kjhljk"<<std::endl;

                        
                        normal= p1->normal;
                     
                    }else if(my_t== my_t3){
                        //std::cout<<my_t<<" "<<my_t3<<std::endl;
                        normal= b1->box_hit_normal(r);
                    }

                    length= glm::length(normal);
                    normal=normal/length;

                    normal= (normal+glm::vec3(1.0,1.0,1.0));
                    normal=normal/two;
                    color= glm::vec4(normal,1.0);
                   triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);

                     pixels[i + (frameHeight-j-1)*frameWidth]=triangleColor;
                }else{
                    //std::cout<<"NO"<<std::endl;
                }
                
            }
        }        
    

    }
    
 

    
    void raytracing3(){

         glm::vec3 color= {1.0,0.5,0.7};       
        Uint32 *pixels = (Uint32*)framebuffer->pixels;
        SDL_PixelFormat *format = framebuffer->format;
        Uint32 triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);
        Uint32 bgColor  = SDL_MapRGBA(format, 255, 255, 255, 1);    

        Ray *r= new Ray();
        
        Sphere *s1= new Sphere();
        s1->center= {0.0,0.0,-2.0};
        s1->radius=0.9;
        s1->albedo={0.8,0.7,0.8};
        Plane *p1= new Plane();
        p1->normal= {0.0,1.0,0.0};
        p1->po= {0.0,-1.4,0.0};
        p1->albedo= {1.0,0.7,0.8};

      
        Box *b1= new Box();
        b1->min_bound= {2.0,3.0,-5.0};
        b1->max_bound= {3.0,4.0,-6.0};

        b1->albedo= {0.5,0.5,0.5};

        Pointsource *ps1=new Pointsource();
        ps1->origin={10.0,10.0,5.0};
        ps1->color={1.0,1.0,1.0,1.0};
        ps1->intensity=100;


        Pointsource *ps2=new Pointsource();
        ps2->origin={10.0,10.0,20.0};
        ps2->color={0.0,0.0,1.0,1.0};
        ps2->intensity=100;

        Scene *scene1 = new Scene();
        scene1->box_array={};
        scene1->box_array.push_back(b1);
        scene1->plane_array={};
        scene1->plane_array.push_back(p1);
        scene1->pointsource_array={};
        scene1->pointsource_array.push_back(ps1);
        //scene1->pointsource_array.push_back(ps2);
        scene1->sphere_array={};
        scene1->sphere_array.push_back(s1);
        

        float x,y;    
        glm::vec3 vec3origin={0.0,0.0,0.0};
        glm::vec3 tempvec3= {0.0,0.0,0.0};
        float tempx , tempy;

        float my_t =-1.0;
        float length=0.0;
        float two=2.0;


        

        std::vector<float> vec={0.0,0.0,0.0,0.0};
        glm::vec3 hit_point={0.0,0.0,0.0};

        float temp=0.0;
        float temp1=0.0;
        float tempintensity=0.0;
        Ray *light_ray = new Ray();

        int oldi=0;
        int oldj=0;

        float my_t1=0;
        glm::vec3 incident_light={0.0,0.0,0.0};
        glm::vec3 normal={0.0,0.0,0.0};
        glm::vec3 albedo={0.0,0.0,0.0};
        
    
       // int closest_shape=0;
        for(int i=0; i<frameWidth; i++) {
            for(int j=0; j<frameHeight; j++) {        
                
                x=i+0.5;
                y=j+0.5;

                tempx= (-1 + x*2/frameWidth)*1;
                tempy= (-1 + y*2/frameHeight)*1;
                r->origin=vec3origin;
                tempvec3[0]=tempx;
                tempvec3[1]=tempy;
                tempvec3[2]=-1.0;
                r->direction= tempvec3;
                
                my_t= scene1->scene_hit(r);

                if(my_t!=999999){
                    //oldi=scene1->my_i;
                    //oldj= scene1->my_j;
                    normal= scene1->normal;
                    albedo= scene1->albedo;
                    hit_point= r->origin + my_t*r->direction;

                    color=scene1->hit_point_color(hit_point, normal, albedo);

                   

                    //gamma correction
                    //sRGB_gamma_correction(color);



                     //  std::cout<<"hello"<<std::endl;
                    triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);

                    pixels[i + (frameHeight-j-1)*frameWidth]=triangleColor;
                    


                }else{  
                    
                    //   color= {0.0,0.0,0.0,1.0};

                    //     // //  //  std::cout<<"hello"<<std::endl;
                    //       triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);

                    //       pixels[i + (frameHeight-j-1)*frameWidth]=triangleColor;


                }


                
            }
                
            }
              
    

    }
 
    
  void raytracing5(){

         glm::vec3 color= {1.0,0.5,0.7};       
        Uint32 *pixels = (Uint32*)framebuffer->pixels;
        SDL_PixelFormat *format = framebuffer->format;
        Uint32 triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);
        Uint32 bgColor  = SDL_MapRGBA(format, 255, 255, 255, 1);    

        Ray *r= new Ray();
        
        Sphere *s1= new Sphere();
        s1->center= {0.0,0.0,-2.0};
        s1->radius=0.9;
        s1->albedo={0.8,0.7,0.8};
        Plane *p1= new Plane();
        p1->normal= {0.0,1.0,0.0};
        p1->po= {0.0,-1.4,0.0};
        p1->albedo= {1.0,0.7,0.8};

      
        Box *b1= new Box();
        b1->min_bound= {2.0,3.0,-5.0};
        b1->max_bound= {3.0,4.0,-6.0};

        b1->albedo= {0.5,0.5,0.5};

        Pointsource *ps1=new Pointsource();
        ps1->origin={10.0,10.0,5.0};
        ps1->color={1.0,1.0,1.0,1.0};
        ps1->intensity=100;


        Pointsource *ps2=new Pointsource();
        ps2->origin={10.0,10.0,20.0};
        ps2->color={0.0,0.0,1.0,1.0};
        ps2->intensity=100;

        Scene *scene1 = new Scene();
        scene1->box_array={};
        scene1->box_array.push_back(b1);
        scene1->plane_array={};
        scene1->plane_array.push_back(p1);
        scene1->pointsource_array={};
        scene1->pointsource_array.push_back(ps1);
        //scene1->pointsource_array.push_back(ps2);
        scene1->sphere_array={};
        scene1->sphere_array.push_back(s1);
        

        float x,y;    
        glm::vec3 vec3origin={0.0,0.0,0.0};
        glm::vec3 tempvec3= {0.0,0.0,0.0};
        float tempx , tempy;

        float my_t =-1.0;
        float length=0.0;
        float two=2.0;


        

        std::vector<float> vec={0.0,0.0,0.0,0.0};
        glm::vec3 hit_point={0.0,0.0,0.0};

        float temp=0.0;
        float temp1=0.0;
        float tempintensity=0.0;
        Ray *light_ray = new Ray();

        int oldi=0;
        int oldj=0;

        float my_t1=0;
        glm::vec3 incident_light={0.0,0.0,0.0};
        glm::vec3 normal={0.0,0.0,0.0};
        glm::vec3 albedo={0.0,0.0,0.0};
        
    
       // int closest_shape=0;
        for(int i=0; i<frameWidth; i++) {
            for(int j=0; j<frameHeight; j++) {        
                
                x=i+0.5;
                y=j+0.5;

                tempx= (-1 + x*2/frameWidth)*1;
                tempy= (-1 + y*2/frameHeight)*1;
                r->origin=vec3origin;
                tempvec3[0]=tempx;
                tempvec3[1]=tempy;
                tempvec3[2]=-1.0;
                r->direction= tempvec3;
                
                my_t= scene1->scene_hit(r);

                if(my_t!=999999){
                    //oldi=scene1->my_i;
                    //oldj= scene1->my_j;
                    normal= scene1->normal;
                    albedo= scene1->albedo;
                    hit_point= r->origin + my_t*r->direction;

                    color=scene1->hit_point_color(hit_point, normal, albedo);

                   

                    //gamma correction
                    sRGB_gamma_correction(color);



                     //  std::cout<<"hello"<<std::endl;
                    triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);

                    pixels[i + (frameHeight-j-1)*frameWidth]=triangleColor;
                    


                }else{  
                    
                    //   color= {0.0,0.0,0.0,1.0};

                    //     // //  //  std::cout<<"hello"<<std::endl;
                    //       triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);

                    //       pixels[i + (frameHeight-j-1)*frameWidth]=triangleColor;


                }


                
            }
                
            }
              
    

    }
 

  void raytracing6(){

         glm::vec3 color= {1.0,0.5,0.7};       
        Uint32 *pixels = (Uint32*)framebuffer->pixels;
        SDL_PixelFormat *format = framebuffer->format;
        Uint32 triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);
        Uint32 bgColor  = SDL_MapRGBA(format, 255, 255, 255, 1);    

        Ray *r= new Ray();
        
        Sphere *s1= new Sphere();
        s1->center= {0.0,0.0,-2.0};
        s1->radius=0.5;
        s1->albedo={0.8,0.7,0.8};
        s1->metal=true;
        s1->F0=0.8;

        Sphere *s2= new Sphere();
        s2->center= {0.4,0.0,-1.2};
        s2->radius=0.2;
        s2->albedo={0.8,0.7,0.8};
        s2->metal=false;
        s2->transparent=false;
        s2->F0=0.3;

        Sphere *s3= new Sphere();
        s3->center= {-0.5,0.0,-1.5};
        s3->radius=0.2;
        s3->albedo={0.3,1.0,1.0};
        s3->metal=false;
        s3->transparent=false;
        s3->F0=0.3;

        Sphere *s4= new Sphere();
        s4->center= {-0.5,0.0,-1.1};
        s4->radius=0.17;
        s4->albedo={0.8,0.7,0.8};
        s4->metal=false;
        s4->transparent=true;
        s4->ref_index=1.5;
        s4->F0=0.3;

        Plane *p1= new Plane();
        p1->normal= {0.0,1.0,0.0};
        p1->po= {0.0,-1.4,0.0};
        p1->albedo= {1.0,0.7,0.8};
        p1->metal=false;

      
        Box *b1= new Box();
        b1->min_bound= {2.0,3.0,-5.0};
        b1->max_bound= {3.0,4.0,-6.0};

        b1->albedo= {0.5,0.5,0.5};

        Pointsource *ps1=new Pointsource();
        ps1->origin={10.0,10.0,5.0};
        ps1->color={1.0,1.0,1.0,1.0};
        ps1->intensity=100;


        Pointsource *ps2=new Pointsource();
        ps2->origin={10.0,10.0,10.0};
        ps2->color={0.0,0.0,1.0,1.0};
        ps2->intensity=100;

        Scene *scene1 = new Scene();
        //scene1->box_array={};
       // scene1->box_array.push_back(b1);
        scene1->plane_array={};
        scene1->plane_array.push_back(p1);
        scene1->pointsource_array={};
        scene1->pointsource_array.push_back(ps1);
        //scene1->pointsource_array.push_back(ps2);
        scene1->sphere_array={};
        scene1->sphere_array.push_back(s1);
        scene1->sphere_array.push_back(s2);
        scene1->sphere_array.push_back(s3);
        scene1->sphere_array.push_back(s4);
        




        Scene *scene6 = new Scene();

        scene6->sphere_array={};
        scene6->sphere_array.push_back(s1);
        

        float x,y;    
        glm::vec3 vec3origin={0.0,0.0,0.0};
        glm::vec3 tempvec3= {0.0,0.0,0.0};
        float tempx , tempy;

        float my_t =-1.0;
        float length=0.0;
        float two=2.0;


        

        std::vector<float> vec={0.0,0.0,0.0,0.0};
        glm::vec3 hit_point={0.0,0.0,0.0};

        float temp=0.0;
        float temp1=0.0;
        float tempintensity=0.0;
        Ray *light_ray = new Ray();

        int oldi=0;
        int oldj=0;

        float my_t1=0;
        glm::vec3 incident_light={0.0,0.0,0.0};
        glm::vec3 normal={0.0,0.0,0.0};
        glm::vec3 albedo={0.0,0.0,0.0};
        
    
       // int closest_shape=0;
        for(int i=0; i<frameWidth; i++) {
            for(int j=0; j<frameHeight; j++) {        
                
                x=i+0.5;
                y=j+0.5;

                tempx= (-1 + x*2/frameWidth)*1;
                tempy= (-1 + y*2/frameHeight)*1;
                r->origin=vec3origin;
                tempvec3[0]=tempx;
                tempvec3[1]=tempy;
                tempvec3[2]=-1.0;
                r->direction= tempvec3;
                
                

                color= scene1->hit_point_color1(r,1,1.0);

                       //  std::cout<<"hello"<<std::endl;
                triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);

                pixels[i + (frameHeight-j-1)*frameWidth]=triangleColor;
                    

                
            }
                
            }
              
    

    }
 

    //call this function after updating framebuffer to apply changes to screen
    void updateScreen() { 
        SDL_BlitScaled(framebuffer, NULL, windowSurface, NULL);
        SDL_UpdateWindowSurface(window);
    }

    // initialize SDL2
    bool initialize() {
        bool success = true;
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            printf("SDL could not initialize! SDL_Error: %s", SDL_GetError());
            success = false;
        } else {
            int screenWidth = frameWidth * displayScale;
            int screenHeight = frameHeight * displayScale;
            window = SDL_CreateWindow("COL781", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
            if (window == NULL) {
                printf("Window could not be created! SDL_Error: %s", SDL_GetError());
                success = false;
            } else {
                windowSurface = SDL_GetWindowSurface(window);
                framebuffer = SDL_CreateRGBSurface(0, frameWidth, frameHeight, 32, 0, 0, 0, 0);
            }
        }
        return success;
    }

    //handle events 
    void handleEvents() {        
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // free resources
    void terminate() {
        // Free resources and close SDL
        SDL_FreeSurface(framebuffer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }


};



void singletringle(){
      //runclock();
    SoftwareRasterization* pipe = new SoftwareRasterization();
    glm::vec4 color(0.25, 0.73, 0.60, 1.0);
     glm::vec4 vertices[] = {
         glm::vec4( 1.0,0,0,0),
         glm::vec4(0.0,1.0,0,0),
        
         glm::vec4(-0.5,0.5,0,0)

      };

    while(!pipe->quit) {
        pipe->handleEvents();

        pipe->clearFrameBuffer();
       

       //pipe->raytracing1();

       // pipe->raytracing2();
       // pipe->raytracing3();
        pipe->raytracing6();
     
       // std::cout<<"hi"<<std::endl;
        pipe->updateScreen();

        //std::cout<<"hi"<<std::endl;
        pipe->clearFrameBuffer();
        
       //  break;
        
    }

     pipe->terminate();
}



int main(int argc, char* args[]) {


    singletringle();


    return 0;
}
