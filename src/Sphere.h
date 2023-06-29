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

class Sphere {

public:
  Sphere();

  glm::vec3 center;
  float radius;
  glm::vec3 albedo;
  glm::vec3 normal;
  bool metal;
  bool transparent;
  float F0;
  float ref_index;

  float sphere_hit(Ray *r);
  glm::vec3 sphere_hit_normal(Ray *r);

};
