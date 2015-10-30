#include "PhysicsPrecompiled.h"
#include "Geometry.h"
namespace WickedSick
{
  AABBox::AABBox(void)
  {
  }

  AABBox::AABBox(const Vector3 & center, 
                 const Vector3 & halfSides) 
                  : center_(center), 
                    half_sides_(halfSides) 
  {
  }

  Sphere::Sphere(void) 
  {
  }

  Sphere::Sphere(const Vector3 & center, 
                 const float & radius) 
                  : center_(center), 
                    radius_(radius) 
  {
  }

  Rectangle::Rectangle(void) 
  {
  }

  Rectangle::Rectangle(const Vector3 & one, 
                       const Vector3 & two, 
                       const Vector3 & three, 
                       const Vector3 & four) 
                        : one_(one), 
                          two_(two), 
                          three_(three), 
                          four_(four) 
  {
  }
  
  Triangle::Triangle(void) 
  {
  }
  
  Triangle::Triangle(const Vector3 & one, 
                     const Vector3 & two, 
                     const Vector3 & three) 
                      : one_(one), 
                        two_(two), 
                        three_(three) 
  {
  }
  
  Line::Line(const Vector3 & one, 
             const Vector3 & two) 
              : Start(one), 
                End(two) 
  {
  }
}

