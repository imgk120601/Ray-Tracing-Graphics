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

//#include "HalfEdge.h"
#include <vector>

class Ray;
class Pointsource {

public:
  Pointsource();
  glm::vec3 origin;
  glm::vec4 color;
  float intensity;

  float pointsource_hit(Ray *r);

};
