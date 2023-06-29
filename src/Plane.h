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

//#include "Mesh.h"
#include <vector>

class Ray;


class Plane {

public:
  Plane();

  glm::vec3 po;
  glm::vec3 normal;
  glm::vec3 albedo;
  bool metal;
  bool transparent;
   float F0;
   float ref_index=0;
  

  float plane_hit(Ray *r);
  glm::vec3 plane_hit_normal(Ray *r);

};
