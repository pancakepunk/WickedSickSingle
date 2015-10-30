#pragma once

namespace WickedSick
{
  struct AABBox;
  struct Sphere;

  float PointToLineDistSq(const Vector3 & line0,
                          const Vector3 & line1,
                          const Vector3 & point);
  float PointToTriSq(const Vector3 & t0,
                     const Vector3 & t1,
                     const Vector3 & t2,
                     const Vector3 & point);

  bool rectToCircle(Vector2 minimum, 
                    Vector2 maximum, 
                    Vector2 center, 
                    float radius);

  bool RectToCircle(AABBox bbox, Sphere circle);
  bool PointToCircle(Vector3 pt, Sphere circle);
  bool CircleToCircle(Sphere circle1, Sphere circle2);

}
