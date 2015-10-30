
#include "PhysicsPrecompiled.h"
#include "ContactConstraint.h"
#include "Architecture/RigidBody.h"
#include "Components\PhysicsComponent.h"
#include "Contact.h"
#include "Architecture/PhysicsMaterial.h"
#include "Architecture/Collider.h"

namespace WickedSick
{
  ContactConstraint::ContactConstraint(Collider* first, 
                                       Collider* second,
                                       Contact* contact) :  Constraint(),
                                                            first_collider_(first),
                                                            second_collider_(second),
                                                            first_body_(first->GetBase()),
                                                            second_body_(second->GetBase()),
                                                            restitution_((first_collider_->GetMaterial()->Restitution + second_collider_->GetMaterial()->Restitution)*0.5f),
                                                            restitution_slop_(0.5f),
                                                            penetration_slop_(0.005f),
                                                            baumgarte_term_(-0.000005f),
                                                            contact_(contact),
                                                            restitution_term_(0.0f)
  {
  }

  ContactConstraint::~ContactConstraint()
  {
  }

  void ContactConstraint::Initialize()
  {
    Vector3 rA = contact_->GetWorldContactPoint1() - (first_body_->GetState().Position + first_body_->GetState().CenterOfMass);
    Vector3 rB = contact_->GetWorldContactPoint2() - (second_body_->GetState().Position + second_body_->GetState().CenterOfMass); 

    float relativeVelocity;
    Vector3 crossA, crossB;
    crossA = first_body_->GetState().AngularVelocity.Cross(rA);
    crossB = second_body_->GetState().AngularVelocity.Cross(rB);
    relativeVelocity = (-first_body_->GetState().Velocity - crossA + second_body_->GetState().Velocity + crossB).Dot(contact_->GetNormal());
    
    if(relativeVelocity < -0.25f)
    {
      restitution_term_ = relativeVelocity * (restitution_);
    }

  }

  void ContactConstraint::Iterate(const float& dt)
  {
    Vector3 posA = (first_body_->GetState().Position + first_body_->GetState().CenterOfMass);
    Vector3 posB = (second_body_->GetState().Position + second_body_->GetState().CenterOfMass);

    //our vector from first to the constraint point, and from the second to the contraint point
    Vector3 rA = contact_->GetWorldContactPoint1() - posA;
    Vector3 rB = contact_->GetWorldContactPoint2() - posB; 

    // cross ra and rb with the normal
    Vector3 rAcrossNormal, rBcrossNormal;
    rAcrossNormal = (-rA).Cross(contact_->GetNormal());
    rBcrossNormal = (-rB).Cross(contact_->GetNormal());

    //state references for easier access
    MotionState& firstBodyState = first_body_->GetState();
    MotionState& secondBodyState = second_body_->GetState();

    //setup the jacobian matrix
    float Jacobian[1][12];
    Jacobian[0][ 0] = -contact_->GetNormal().x;  
    Jacobian[0][ 1] = -contact_->GetNormal().y;  
    Jacobian[0][ 2] = -contact_->GetNormal().z;  
    Jacobian[0][ 3] = rAcrossNormal.x;  
    Jacobian[0][ 4] = rAcrossNormal.y;  
    Jacobian[0][ 5] = rAcrossNormal.z;
    Jacobian[0][ 6] = contact_->GetNormal().x;  
    Jacobian[0][ 7] = contact_->GetNormal().y;  
    Jacobian[0][ 8] = contact_->GetNormal().z;  
    Jacobian[0][ 9] = rBcrossNormal.x;
    Jacobian[0][10] = rBcrossNormal.y;
    Jacobian[0][11] = rBcrossNormal.z;

    //transpose it
    float JacobianTranspose[12][1];
    JacobianTranspose[ 0][0] = Jacobian[0][ 0];  
    JacobianTranspose[ 1][0] = Jacobian[0][ 1];  
    JacobianTranspose[ 2][0] = Jacobian[0][ 2];  
    JacobianTranspose[ 3][0] = Jacobian[0][ 3];
    JacobianTranspose[ 4][0] = Jacobian[0][ 4];
    JacobianTranspose[ 5][0] = Jacobian[0][ 5];
    JacobianTranspose[ 6][0] = Jacobian[0][ 6]; 
    JacobianTranspose[ 7][0] = Jacobian[0][ 7]; 
    JacobianTranspose[ 8][0] = Jacobian[0][ 8]; 
    JacobianTranspose[ 9][0] = Jacobian[0][ 9];
    JacobianTranspose[10][0] = Jacobian[0][10];
    JacobianTranspose[11][0] = Jacobian[0][11];

    //velocity vector.
    float velocities[12][1];
    velocities[ 0][0] = firstBodyState.Velocity.x;
    velocities[ 1][0] = firstBodyState.Velocity.y;
    velocities[ 2][0] = firstBodyState.Velocity.z;
    velocities[ 3][0] = firstBodyState.AngularVelocity.x;
    velocities[ 4][0] = firstBodyState.AngularVelocity.y;
    velocities[ 5][0] = firstBodyState.AngularVelocity.z;
    velocities[ 6][0] = secondBodyState.Velocity.x;
    velocities[ 7][0] = secondBodyState.Velocity.y;
    velocities[ 8][0] = secondBodyState.Velocity.z;
    velocities[ 9][0] = secondBodyState.AngularVelocity.x;
    velocities[10][0] = secondBodyState.AngularVelocity.y;
    velocities[11][0] = secondBodyState.AngularVelocity.z;

    //multiplying jacobian by velocity vector
    //         V
    //         V
    // J J J J V
    //         V

    float JV = 
      Jacobian[0][ 0] * velocities[ 0][0] +
      Jacobian[0][ 1] * velocities[ 1][0] +
      Jacobian[0][ 2] * velocities[ 2][0] +
      Jacobian[0][ 3] * velocities[ 3][0] +  
      Jacobian[0][ 4] * velocities[ 4][0] +  
      Jacobian[0][ 5] * velocities[ 5][0] +
      Jacobian[0][ 6] * velocities[ 6][0] + 
      Jacobian[0][ 7] * velocities[ 7][0] + 
      Jacobian[0][ 8] * velocities[ 8][0] + 
      Jacobian[0][ 9] * velocities[ 9][0] +
      Jacobian[0][10] * velocities[10][0] +
      Jacobian[0][11] * velocities[11][0];

    //multiplying mass matrix by jacobian transpose
    //          M M M M
    //          M M M M
    //          M M M M
    // J J J J  M M M M

    float InvMassxJacobianTranspose[12][1];
    
    InvMassxJacobianTranspose[ 0][0] = firstBodyState.InverseMass           * JacobianTranspose[ 0][0];
    InvMassxJacobianTranspose[ 1][0] = firstBodyState.InverseMass           * JacobianTranspose[ 1][0];
    InvMassxJacobianTranspose[ 2][0] = firstBodyState.InverseMass           * JacobianTranspose[ 2][0];
    InvMassxJacobianTranspose[ 3][0] = firstBodyState.InverseInertiaTensor  * JacobianTranspose[ 3][0];
    InvMassxJacobianTranspose[ 4][0] = firstBodyState.InverseInertiaTensor  * JacobianTranspose[ 4][0];
    InvMassxJacobianTranspose[ 5][0] = firstBodyState.InverseInertiaTensor  * JacobianTranspose[ 5][0];
    InvMassxJacobianTranspose[ 6][0] = secondBodyState.InverseMass          * JacobianTranspose[ 6][0];
    InvMassxJacobianTranspose[ 7][0] = secondBodyState.InverseMass          * JacobianTranspose[ 7][0];
    InvMassxJacobianTranspose[ 8][0] = secondBodyState.InverseMass          * JacobianTranspose[ 8][0];
    InvMassxJacobianTranspose[ 9][0] = secondBodyState.InverseInertiaTensor * JacobianTranspose[ 9][0];
    InvMassxJacobianTranspose[10][0] = secondBodyState.InverseInertiaTensor * JacobianTranspose[10][0];
    InvMassxJacobianTranspose[11][0] = secondBodyState.InverseInertiaTensor * JacobianTranspose[11][0];

    //found our effective mass via jacobian * mass * jacobian transpose
    float effectiveMass = 
      Jacobian[0][ 0] * InvMassxJacobianTranspose[ 0][0] +
      Jacobian[0][ 1] * InvMassxJacobianTranspose[ 1][0] +
      Jacobian[0][ 2] * InvMassxJacobianTranspose[ 2][0] +
      Jacobian[0][ 3] * InvMassxJacobianTranspose[ 3][0] +
      Jacobian[0][ 4] * InvMassxJacobianTranspose[ 4][0] +
      Jacobian[0][ 5] * InvMassxJacobianTranspose[ 5][0] +
      Jacobian[0][ 6] * InvMassxJacobianTranspose[ 6][0] +
      Jacobian[0][ 7] * InvMassxJacobianTranspose[ 7][0] +
      Jacobian[0][ 8] * InvMassxJacobianTranspose[ 8][0] +
      Jacobian[0][ 9] * InvMassxJacobianTranspose[ 9][0] +
      Jacobian[0][10] * InvMassxJacobianTranspose[10][0] +
      Jacobian[0][11] * InvMassxJacobianTranspose[11][0];

    //tweakable. keep here. should be low.
    float beta = 0.5f;//075f;
    
    Vector3 crossA, crossB;
    crossA = firstBodyState.AngularVelocity.Cross(rA);
    crossB = secondBodyState.AngularVelocity.Cross(rB);
    
    Vector3 relativeVelVec = (-firstBodyState.Velocity - crossA) + (secondBodyState.Velocity + crossB);
    float relativeVelocity;
    relativeVelocity = relativeVelVec.Dot(contact_->GetNormal());
    
    float bias = ((beta/dt) * std::max(abs(contact_->GetPenetrationDepth()) - penetration_slop_, 0.0f)); 

    bias += std::max(abs(restitution_term_) - restitution_slop_, 0.0f);

    float lambda = -(relativeVelocity - bias) / effectiveMass;

    float newimpulse = std::max(contact_->GetNormalImpulseSum() + lambda, 0.0f);

    lambda = abs(newimpulse - contact_->GetNormalImpulseSum());

    contact_->SetNormalImpulseSum(newimpulse);


    float dVelocities[12][1];

    dVelocities[ 0][0] = InvMassxJacobianTranspose[ 0][0] * lambda;
    dVelocities[ 1][0] = InvMassxJacobianTranspose[ 1][0] * lambda;
    dVelocities[ 2][0] = InvMassxJacobianTranspose[ 2][0] * lambda;
    dVelocities[ 3][0] = InvMassxJacobianTranspose[ 3][0] * lambda;
    dVelocities[ 4][0] = InvMassxJacobianTranspose[ 4][0] * lambda;
    dVelocities[ 5][0] = InvMassxJacobianTranspose[ 5][0] * lambda;
    dVelocities[ 6][0] = InvMassxJacobianTranspose[ 6][0] * lambda;
    dVelocities[ 7][0] = InvMassxJacobianTranspose[ 7][0] * lambda;
    dVelocities[ 8][0] = InvMassxJacobianTranspose[ 8][0] * lambda;
    dVelocities[ 9][0] = InvMassxJacobianTranspose[ 9][0] * lambda;
    dVelocities[10][0] = InvMassxJacobianTranspose[10][0] * lambda;
    dVelocities[11][0] = InvMassxJacobianTranspose[11][0] * lambda;

    if(!first_body_->IsStatic())
    {
      firstBodyState.Velocity.x         += dVelocities[ 0][0];
      firstBodyState.Velocity.y         += dVelocities[ 1][0];
      firstBodyState.Velocity.z         += dVelocities[ 2][0];
      firstBodyState.AngularVelocity.x  += dVelocities[ 3][0];
      firstBodyState.AngularVelocity.y  += dVelocities[ 4][0];
      firstBodyState.AngularVelocity.z  += dVelocities[ 5][0];
    }                                                           

    if(!second_body_->IsStatic())                                     
    {                                                           
      secondBodyState.Velocity.x        += (dVelocities[ 6][0]);
      secondBodyState.Velocity.y        += (dVelocities[ 7][0]);
      secondBodyState.Velocity.z        += (dVelocities[ 8][0]);
      secondBodyState.AngularVelocity.x += (dVelocities[ 9][0]);
      secondBodyState.AngularVelocity.y += (dVelocities[10][0]);
      secondBodyState.AngularVelocity.z += (dVelocities[11][0]);
    }
  }
}
