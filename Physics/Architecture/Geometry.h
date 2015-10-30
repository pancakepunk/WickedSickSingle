#pragma once
#include "PhysicsPrecompiled.h"

namespace WickedSick
{

  struct Line 
  {
    Line(const Vector3 &one, 
         const Vector3 &two);
    
    Vector3 Start;
    Vector3 End;
  };
  
  struct Triangle 
  {
    Triangle(void);
    Triangle(const Vector3 &one, 
             const Vector3 &two, 
             const Vector3 &three);
    
    Vector3 one_;
    Vector3 two_;
    Vector3 three_;
  };
  
  struct Rectangle 
  {
    Rectangle(void);
    Rectangle(const Vector3 &one, 
              const Vector3 &two, 
              const Vector3 &three, 
              const Vector3 &four);
    
    Vector3 one_;
    Vector3 two_;
    Vector3 three_;
    Vector3 four_;
  };
  
  struct Sphere 
  {
    Sphere(void);
    Sphere(const Vector3 &center, const float &radius);
    
    Vector3 center_;
    float   radius_;
  };

  struct AABBox
  {
    AABBox(void);
    AABBox(const Vector3 &center, 
           const Vector3 &halfSides);

    Vector3 center_;
    Vector3 half_sides_;
  };

}

