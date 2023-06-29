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


class SoftwareRasterization{
public:    
    int frameWidth = 400;
    int frameHeight = 400;
    int displayScale = 1;

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
    
    // check if framebuffer coordinates (i, j) are inside the triangle  
    bool insideTriangle(glm::vec4 *vertices, float x, float y) {       

        //to go from NDC(ON PAPER COORDINATE) to screen coordinate(pixel position)--conversion                     
        float xa = (vertices[0][0] + 1.0f)*0.5*frameWidth, ya = (1.0f-vertices[0][1])*0.5*frameHeight;
        float xb = (vertices[1][0] + 1.0f)*0.5*frameWidth, yb = (1.0f-vertices[1][1])*0.5*frameHeight;
        float xc = (vertices[2][0] + 1.0f)*0.5*frameWidth, yc = (1.0f-vertices[2][1])*0.5*frameHeight;
        
        // float ab = (ya-yb)*x + (xb-xa)*y + xa*yb - xb*ya;
        // float cab = (ya-yb)*x + (xb-xa)*y + xa*yb - xb*ya;

        float bc = (yb-yc)*x + (xc-xb)*y + xb*yc - xc*yb;
        float abc = (yb-yc)*xa + (xc-xb)*ya + xb*yc - xc*yb;
        
        float ca = (yc-ya)*x + (xa-xc)*y + xc*ya - xa*yc;
        float bca = (yc-ya)*xb + (xa-xc)*yb + xc*ya - xa*yc;

        float alpha = bc/abc, beta = ca/bca, gamma = 1-alpha-beta;                
        return alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1;
    }


    // check if framebuffer coordinates (i, j) are inside the triangle  
    //part1_3_supersampleing
    int insideTriangle1(glm::vec4 *vertices, float x, float y ,int n) {       

        float radius=0.25;
        float mydegree= 360/n;

        float currad=0.0174532925*mydegree;

        float mycount=0;
       

        float myx,myy;

        float currdegree=0.0;
        
        for(int i=0; i<n; i++){

                
                currad=currdegree*0.0174532925;

                myx=x+radius*cos(currad);
                myy=y+ radius*sin(currad);

                currdegree=currdegree+mydegree;
                if(insideTriangle(vertices,myx,myy)){
                    mycount++;
                }

        }

        return mycount;


    }

    // rasterize single triangle onto framebuffer from NDC
    // assuming coordinates are in anticlockwise order
    //brute force method to  resterize triangle
    //part1-1
    void rasterizeSingleTriangle(glm::vec4 *vertices, glm::vec4 color) {        
        Uint32 *pixels = (Uint32*)framebuffer->pixels;
        SDL_PixelFormat *format = framebuffer->format;
        Uint32 triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);
        Uint32 bgColor  = SDL_MapRGBA(format, 255, 255, 255, 1);    

        float x,y;    
        for(int i=0; i<frameWidth; i++) {
            for(int j=0; j<frameHeight; j++) {        
                
                x=i+0.5;
                y=j+0.5;

                if(insideTriangle(vertices,x,y)){
                    pixels[i + j*frameWidth]=triangleColor;
                }
                        
                
            }
        }        
    }
    
    //part1-2
    void rasterizeMultipleTriangle(glm::vec4 *vertices, glm::vec4 color, glm::ivec3 *indices) {        
       
         int a,b,c;
          glm::vec4 tempvertices[]={ glm::vec4(-0.4, -0.8, 0.0, 1.0),
         glm::vec4( 0.8,  0.8, 0.0, 1.0),
        glm::vec4(-0.4, -0.4, 0.0, 1.0)};
        int noofindices=sizeof(indices)/4;

       // std::cout<<noofindices<<std::endl;
        for(int i=0; i<noofindices; i++){

            a=indices[i][0];
            b=indices[i][1];
            c=indices[i][2];

            tempvertices[0]=vertices[a];
            tempvertices[1]=vertices[b];
            tempvertices[2]=vertices[c];


            this->rasterizeSingleTriangle(tempvertices, color);
           
        }


    }
    
    //part-1 3_super sampleing
   void supersampling_singletriangle(glm::vec4 *vertices, glm::vec4 color ,int n ) {        
        Uint32 *pixels = (Uint32*)framebuffer->pixels;
        SDL_PixelFormat *format = framebuffer->format;
        Uint32 triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);
        Uint32 bgColor  = SDL_MapRGBA(format, 255, 255, 255, 1);    

        float x,y;    

        float temp=0;
        for(int i=0; i<frameWidth; i++) {
            for(int j=0; j<frameHeight; j++) {        
                
                x=i+0.5;
                y=j+0.5;


                  temp=insideTriangle1(vertices, x, y,n);
                  temp= (float)temp/9;
                  temp=temp*255;
                  triangleColor=SDL_MapRGBA(format, color[0]*temp, color[1]*temp, color[2]*temp, 1);
                  if(temp>0){
                      pixels[i + j*frameWidth]=triangleColor;
                  }
               
                //std::cout<<i<<" "<<j<<" "<< temp<<std::endl;
            }
        }        
    }

        //part1-2
    void supersampling_multipletriangle(glm::vec4 *vertices, glm::vec4 color, glm::ivec3 *indices,int n) {        
       
         int a,b,c;
          glm::vec4 tempvertices[]={ glm::vec4(-0.4, -0.8, 0.0, 1.0),
         glm::vec4( 0.8,  0.8, 0.0, 1.0),
        glm::vec4(-0.4, -0.4, 0.0, 1.0)};
        int noofindices=sizeof(indices)/4;

       // std::cout<<noofindices<<std::endl;
        for(int i=0; i<noofindices; i++){

            a=indices[i][0];
            b=indices[i][1];
            c=indices[i][2];

            tempvertices[0]=vertices[a];
            tempvertices[1]=vertices[b];
            tempvertices[2]=vertices[c];


            this->supersampling_singletriangle(tempvertices, color,n);
           
        }


    }

    
    void drawrectangle(int x, int y, int width, int height){

        int x1=x;
        int x2=x+width;
        int y1=y;
        int y2= y+height;

        glm::vec4 color(1.00, 1.00, 1.00, 1.0);

        Uint32 *pixels = (Uint32*)framebuffer->pixels;
        SDL_PixelFormat *format = framebuffer->format;
        Uint32 triangleColor = SDL_MapRGBA(format, color[0]*255, color[1]*255, color[2]*255, 1);
        

        for(int i=x1; i<=x2; i++){

            for(int j=y1; j<=y2; j++){
                pixels[i + j*frameWidth]=triangleColor;
            }
        }


    }

    void drawclock(){

        //12
        drawrectangle(190,10,5,20);
        drawrectangle(210,10,5,20);
        drawrectangle(205,10,5,5);
        drawrectangle(215,25,5,5);


        //3
        drawrectangle(0+380,0+180,10,5);
        drawrectangle(0+380,10+180,10,5);
        drawrectangle(0+380,20+180,10,5);
        drawrectangle(10+380,0+180,5,25);

        //6
        drawrectangle(370-180,185+180,5,25);
        drawrectangle(370-180,205+180,10,5);
        drawrectangle(380-180,195+180,5,15);
        drawrectangle(370-180,195+180,10,5);

        
        //9
        drawrectangle(0+10,0+180,5,15);
        drawrectangle(5+10,0+180,10,5);
        drawrectangle(5+10,10+180,10,5);
        drawrectangle(15+10,0+180,5,25);


       






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



void runclock(){
    SoftwareRasterization* pipe = new SoftwareRasterization();
    //glm::vec4 color(0.25, 0.73, 0.60, 1.0);
    glm::vec4 color(1.00, 0.00, 0.00, 1.0);
    glm::vec4 color1(0.00, 1.00, 0.00, 1.0);
 glm::vec4 color2(0.00, 0.00, 1.00, 1.0);
  glm::vec4 color3(1.00, 1.00, 1.00, 1.0);

    glm::vec4 vertices[] = {
        glm::vec4(0.01,0,0,0),
        glm::vec4(0.01,1.0,0,0),
        
        glm::vec4(-0.01,1.0,0,0),
        glm::vec4(-0.01,1.0,0,0)

};

    glm::vec4 vertices1[] = {
   glm::vec4(0.01,0,0,0),
        glm::vec4(0.01,1.0,0,0),
        
        glm::vec4(-0.01,1.0,0,0),
        glm::vec4(-0.01,1.0,0,0)

};

    glm::vec4 vertices2[] = {
     glm::vec4(0.02,0,0,0),
        glm::vec4(0.02,0.5,0,0),
        
        glm::vec4(-0.02,0.5,0,0),
        glm::vec4(-0.02,0,0,0)

};


    glm::ivec3 indices[] = {
        glm::ivec3(0, 1, 2),
        glm::ivec3(0,2,3)
    };

	glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 tempmodel= glm::mat4(1.0f);

    glm::vec4 tempvertices[] = {
      glm::vec4(0.01,0,0,0),
        glm::vec4(0.01,1.0,0,0),
        
        glm::vec4(-0.01,1.0,0,0),
        glm::vec4(-0.01,0.0,0,0)
};

    glm::vec4 tempvertices1[] = {
  glm::vec4(0.01,0,0,0),
        glm::vec4(0.01,1.0,0,0),
        
        glm::vec4(-0.01,1.0,0,0),
        glm::vec4(-0.01,0.0,0,0)

};
    glm::vec4 tempvertices2[] = {
     glm::vec4(0.02,0,0,0),
        glm::vec4(0.02,0.5,0,0),
        
        glm::vec4(-0.02,0.5,0,0),
        glm::vec4(-0.02,0,0,0)

};



    float crntTime=0;
   float prevTime=SDL_GetTicks64();
   float rotation = 0.0f;

      float rotationhr=0.0f;
    float rotationmin=0.0f;
    float rotationsec=0.0f;
   
    int second=0;
    int min=0;
    int hr=0;

    pipe->rasterizeMultipleTriangle(vertices,color,indices);

    pipe->rasterizeMultipleTriangle(vertices1,color1,indices);
    pipe->rasterizeMultipleTriangle(vertices2,color2,indices);
	pipe->drawclock();
    pipe->updateScreen();


    while(!pipe->quit) {
        pipe->handleEvents();


    //    // Simple timer
       prevTime = SDL_GetTicks64();
       crntTime = SDL_GetTicks64();
		
		while (crntTime - prevTime <= 1000 )
		{
            crntTime = SDL_GetTicks64();   
		}

        second++;

        rotationsec = 0.0174532925*second*6*59;
        
        pipe->clearFrameBuffer();

            model = glm::rotate(tempmodel, glm::radians(-rotationsec), glm::vec3(0.0f, 0.0f, 1.0f));

            for(int i=0; i<3; i++){
                tempvertices[i]=model*vertices[i];
            }
                 //pipe->supersampling(vertices,color,9);
           pipe->rasterizeMultipleTriangle(tempvertices,color,indices);

           if(second==58){
               min++;
               rotationmin = 0.0174532925*min*6*59;
                model1 = glm::rotate(tempmodel, glm::radians(-rotationmin), glm::vec3(0.0f, 0.0f, 1.0f));

                for(int i=0; i<3; i++){
                    tempvertices1[i]=model1*vertices1[i];
                }

                second=0;
                //rotationsec=0;

           }

            if(min==58){
                hr++;
               rotationhr =  0.0174532925*hr*6*59;
                model2 = glm::rotate(tempmodel, glm::radians(-rotationhr), glm::vec3(0.0f, 0.0f, 1.0f));

                for(int i=0; i<3; i++){
                    tempvertices2[i]=model2*vertices2[i];
                }

                min=0;
                //rotationmin=0;
           }

             pipe->rasterizeMultipleTriangle(tempvertices1,color1,indices);
             pipe->rasterizeMultipleTriangle(tempvertices2,color2,indices);
           // pipe->rasterizeMultipleTriangle(vertices3,color3,indices1);

            pipe->drawclock();

            pipe->updateScreen();
   
   
       //pipe->clearFrameBuffer();
        //pipe->rasterizeSingleTriangle(vertices, color);
        //pipe->updateScreen();
        //pipe->clearFrameBuffer();
    }

    pipe->terminate();
}


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
        pipe->rasterizeSingleTriangle(vertices, color);
        pipe->updateScreen();
        pipe->clearFrameBuffer();
    }

     pipe->terminate();
}


void muiltipletriangle(){

  SoftwareRasterization* pipe = new SoftwareRasterization();
    glm::vec4 color(0.25, 0.73, 0.60, 1.0);
glm::vec4 vertices[] = {
    glm::vec4(-0.8,  0.0, 0.0, 1.0),
    glm::vec4(-0.4, -0.8, 0.0, 1.0),
    glm::vec4( 0.8,  0.8, 0.0, 1.0),
    glm::vec4(-0.4, -0.4, 0.0, 1.0)
};
glm::ivec3 indices[] = {
    glm::ivec3(0, 1, 3),
    glm::ivec3(1, 2, 3)
};

    while(!pipe->quit) {
        pipe->handleEvents();


   
        pipe->clearFrameBuffer();
        pipe->supersampling_multipletriangle(vertices, color,indices,4);
        pipe->updateScreen();
        pipe->clearFrameBuffer();
    }

     pipe->terminate();
    


}


void supersampling(){


    SoftwareRasterization* pipe = new SoftwareRasterization();
    glm::vec4 color(0.25, 0.73, 0.60, 1.0);
glm::vec4 vertices[] = {
    glm::vec4(-0.8,  0.0, 0.0, 1.0),
    glm::vec4(-0.4, -0.8, 0.0, 1.0),
    glm::vec4( 0.8,  0.8, 0.0, 1.0)
};
glm::ivec3 indices[] = {
    glm::ivec3(0, 1, 3),
    glm::ivec3(1, 2, 3)
};

    while(!pipe->quit) {
        pipe->handleEvents();


   
        pipe->clearFrameBuffer();
        //pipe->rasterizeMultipleTriangle(vertices, color,indices);
        pipe->supersampling_singletriangle(vertices,color,4);
        pipe->updateScreen();
        pipe->clearFrameBuffer();
    }

     pipe->terminate();
    



}

int main(int argc, char* args[]) {


    //singletringle();
    //muiltipletriangle();
    //supersampling();
    runclock();

 
    




    return 0;
}
