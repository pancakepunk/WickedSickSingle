
#include "PhysicsPrecompiled.h"
#include "PhysicsUtil.h"
#include "Architecture/Geometry.h"

namespace WickedSick
{
  float PointToLineDistSq(const Vector3 & line0,
                          const Vector3 & line1,
                          const Vector3 & point)
  {
    Vector3 v = line1 - line0;
    Vector3 w = point - line0;
    float c1 = w.Dot(v);
    if (c1 <= 0)
    {
      return (point - line0).LengthSq();
    }

    float c2 = v.Dot(v);
    if (c2 <= c1)
    {
      return (point - line1).LengthSq();
    }

    float b = c1 / c2;
    Vector3 Pb = line0 + (v*b);
    return (Pb - point).LengthSq();
  }

  float PointToTriSq(const Vector3 & t0,
                     const Vector3 & t1,
                     const Vector3 & t2,
                     const Vector3 & point)
  {
    //find normal of the tri.
    Vector3 v0 = t2 - t0;
    Vector3 v1 = t1 - t0;
    Vector3 v2 = point - t0;
    float dot00, dot01, dot02, dot11, dot12;
    dot00 = v0.Dot(v0);
    dot01 = v0.Dot(v1);
    dot02 = v0.Dot(v2);
    dot11 = v1.Dot(v1);
    dot12 = v1.Dot(v2);

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    if ((u >= 0) && (v >= 0) && (u + v < 1)) //we're inside
    {
      return 0.0f;
    }
    else if (u < 0.0f) // on the side with the line t0t1
    {
      return PointToLineDistSq(t0, t1, point);
    }
    else if (v < 0.0f) // on the side with the line t0t2
    {
      return PointToLineDistSq(t0, t2, point);
    }
    else // on the side with the line t1t2
    {
      return PointToLineDistSq(t1, t2, point);
    }
  }


  bool rectToCircle(Vector2 minimum, Vector2 maximum, Vector2 center, float radius)
  {
    Vector2 incircle = Vector2(clamp(center.x, minimum.x, maximum.x), clamp(center.y, minimum.y, maximum.y));
    Vector2 fromSquareToCircle = incircle - center;
    if(fromSquareToCircle.LengthSq() < (radius * radius))
    {
      return true;
    }
    return false;
  }

  bool RectToCircle(AABBox bbox, Sphere circle)
  {
    Vector3 bmax = bbox.center_ + bbox.half_sides_;
    Vector3 bmin = bbox.center_ - bbox.half_sides_;

    Vector2 incircle = Vector2(clamp(circle.center_.x, bmin.x, bmax.x), clamp(circle.center_.z, bmin.z, bmax.z));
    Vector2 fromSquareToCircle = incircle - Vector2(circle.center_.x, circle.center_.z);
    if(fromSquareToCircle.LengthSq() < (circle.radius_ * circle.radius_))
    {
      return true;
    }
    return false;
  }

  bool PointToCircle(Vector3 pt, Sphere circle)
  {
    float radSq = circle.radius_ * circle.radius_;
    float distSq = (circle.center_.x - pt.x) * (circle.center_.x - pt.x) + (circle.center_.z - pt.z) * (circle.center_.z - pt.z);

    if(distSq <= radSq) return true;
    return false;
  }

  bool CircleToCircle(Sphere circle1, Sphere circle2)
  {
    float dx = circle2.center_.x - circle1.center_.x;
    float dy = circle2.center_.y - circle1.center_.y;
    float radii = circle1.radius_ + circle2.radius_;
    return ( ( dx * dx )  + ( dy * dy ) < radii * radii ) ;
/*
    //mark your collison is broke bruh!
    float rad1Sq = circle1.radius_ * circle1.radius_;
    float rad2Sq = circle2.radius_ * circle2.radius_;
    float distSq = (circle1.center_.x - circle2.center_.x) * (circle1.center_.x - circle2.center_.x) + (circle1.center_.z - circle2.center_.z) * (circle1.center_.z - circle2.center_.z);

    return (rad1Sq + rad2Sq) <= distSq;
*/
  }
}
