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

#include <vector>

class Ray;

class Box {

public:
  Box();

  glm::vec3 min_bound;
  glm::vec3 max_bound;
  glm::vec3 albedo;
    bool metal;
  bool transparent;
  float F0;
  float ref_index;
  

  float box_hit(Ray *r);
  glm::vec3 box_hit_normal(Ray *r);

};
