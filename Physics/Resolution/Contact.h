#pragma once

namespace WickedSick
{
  class Collider;
  class ContactConstraint;
  class Contact
  {
    public:
      Contact();
      void find_basis();

      Vector3 GetMeshContactPoint1(){return model_contact_point_1_;}
      Vector3 GetMeshContactPoint2(){return model_contact_point_2_;}
      Vector3 GetWorldContactPoint1(){return world_contact_point_1_;}
      Vector3 GetWorldContactPoint2(){return world_contact_point_2_;}
      Vector3 GetNormal(){return normal_;}
      Vector3 GetTangent1(){return tangent_1_;}
      Vector3 GetTangent2(){return tangent_2_;}
      float   GetPenetrationDepth(){return penetration_depth_;}
      float   GetNormalImpulseSum(){return normal_impulse_sum_;}
      float   GetTangentImpulseSum1(){return tangent_impulse_sum_1_;}
      float   GetTangentImpulseSum2(){return tangent_impulse_sum_2_;}

      void SetModelContactPoint1(Vector3 val){model_contact_point_1_ = val;}
      void SetModelContactPoint2(Vector3 val){model_contact_point_2_ = val;}
      void SetWorldContactPoint1(Vector3 val){world_contact_point_1_ = val;}
      void SetWorldContactPoint2(Vector3 val){world_contact_point_2_ = val;}
      void SetNormal(Vector3 val){normal_ = val;}
      void SetTangent1(Vector3 val){tangent_1_ = val;}
      void SetTangent2(Vector3 val){tangent_2_ = val;}
      void SetPenetrationDepth(float val){penetration_depth_  = val;}
      void SetNormalImpulseSum(float val){normal_impulse_sum_ = val;}
      void SetTangentImpulseSum1(float val){tangent_impulse_sum_1_ = val;}
      void SetTangentImpulseSum2(float val){tangent_impulse_sum_2_ = val;}

    private:
      Vector3              model_contact_point_1_;
      Vector3              model_contact_point_2_;

      Vector3              world_contact_point_1_;
      Vector3              world_contact_point_2_;

      Vector3              normal_;
      Vector3              tangent_1_, tangent_2_;
      float                penetration_depth_;
      float                normal_impulse_sum_;
      float                tangent_impulse_sum_1_;
      float                tangent_impulse_sum_2_;
  };
}
