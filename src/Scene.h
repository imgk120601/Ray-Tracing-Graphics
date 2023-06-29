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
using namespace std;

class Ray;
class Box;
class Plane;
class Sphere;
class Pointsource;


class Scene {

public:
  Scene();

  vector<Box*> box_array;
  vector<Sphere*> sphere_array;
  vector<Plane*> plane_array; 
  vector<Pointsource*> pointsource_array;
  glm::vec3 normal;
  glm::vec3 albedo;
  //it will tell which object it the ray hitted in the scene
  int my_i;
  int my_j;
    bool metal;
  bool transparent;
  bool F0;
  float ref_index;


  float scene_hit(Ray *r);
  glm::vec3 scene_hit_normal(Ray *r);
 glm::vec3 hit_point_color(glm::vec3 hit_point, glm::vec3 normal, glm::vec3 albedo);
 glm::vec3 hit_point_color1(Ray *r, int depth, float old_ref_index);

  glm::vec3 hit_point_color_material(glm::vec3 hit_point, glm::vec3 normal, glm::vec3 albedo, glm::vec3 F0);

};
