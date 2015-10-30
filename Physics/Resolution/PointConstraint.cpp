
#include "PhysicsPrecompiled.h"
#include "PointConstraint.h"
#include "Architecture/Collider.h"
namespace WickedSick
{
  PointConstraint::PointConstraint(RigidBody* first, RigidBody* second) : first_(first),
                                                                          second_(second)
  {
  }

  void PointConstraint::solve_constraint()
  {
    //Vector3 ra =  constraint_point_ - first_->State.Position; //our vector from first to the constraint point
    //Vector3 rb =  constraint_point_ - second_->State.Position; //our vector from second to the constraint point
    // 
    //float ram[3][1] = { ra.x, 
    //                    ra.y, 
    //                    ra.z };
    //
    //float rbm[3][1] = { rb.x, 
    //                    rb.y, 
    //                    rb.z };
    //
    //float racp[3][3] = { 0.0f, -ra.z,  ra.y,
    //                     ra.z,  0.0f, -ra.x,
    //                    -ra.y,  ra.x,  0.0f };
    //
    //float rbcp[3][3] = { 0.0f, -rb.z,  rb.y,
    //                     rb.z,  0.0f, -rb.x,
    //                    -rb.y,  rb.x,  0.0f };
    //
    //float racpTranspose[3][3] = { 0.0f,  ra.z, -ra.y,
    //                             -ra.z,  0.0f,  ra.x,
    //                              ra.y, -ra.x,  0.0f };
    //
    //float rbcpTranspose[3][3] = { 0.0f,  rb.z, -rb.y,
    //                             -rb.z,  0.0f,  rb.x,
    //                              rb.y, -rb.x,  0.0f };
    //
    //float Jacobian[3][12];
    //
    //ram = racp * ram;
    //rbm = rbcp * rbm;
    //
    //Jacobian[0][0] = 1.0f;  Jacobian[0][1] = ram[0][0];  Jacobian[0][2] = -1.0f;  Jacobian[0][1] = rbm[0][0];
    //Jacobian[0][0] = 1.0f;  Jacobian[1][1] = ram[1][0];  Jacobian[1][2] = -1.0f;  Jacobian[1][1] = rbm[1][0];
    //Jacobian[2][0] = 1.0f;  Jacobian[2][1] = ram[2][0];  Jacobian[2][2] = -1.0f;  Jacobian[2][1] = rbm[2][0];
    //
    //Matrix JacobianTranspose(Jacobian.Transpose());
    //
    ////Jacobian.
    //
    //Matrix velocities(12, 1);
    //velocities[0][0] = first_->State.Velocity.x; velocities[0][1] = first_->State.AngularVelocity.x; velocities[0][2] = second_->State.Velocity.x; velocities[0][3] = second_->State.AngularVelocity.x;
    //velocities[1][0] = first_->State.Velocity.y; velocities[1][1] = first_->State.AngularVelocity.y; velocities[1][2] = second_->State.Velocity.y; velocities[1][3] = second_->State.AngularVelocity.y;
    //velocities[2][0] = first_->State.Velocity.z; velocities[2][1] = first_->State.AngularVelocity.z; velocities[2][2] = second_->State.Velocity.z; velocities[2][3] = second_->State.AngularVelocity.z;
    //
    //
    ////note on inertia tensors, they can be represented as a scalar or an identity matrix multiplied by a scalar
    //Matrix massMatrix(4, 4);
    //massMatrix[0][0] = first_->State.Mass; massMatrix[0][1] = 0.0f;                        massMatrix[0][2] = 0.0f;                massMatrix[0][3] = 0.0f;
    //massMatrix[1][0] = 0.0f;               massMatrix[1][1] = first_->State.InertiaTensor; massMatrix[1][2] = 0.0f;                massMatrix[1][3] = 0.0f;
    //massMatrix[2][0] = 0.0f;               massMatrix[2][1] = 0.0f;                        massMatrix[2][2] = second_->State.Mass; massMatrix[2][3] = 0.0f;
    //massMatrix[3][0] = 0.0f;               massMatrix[3][1] = 0.0f;                        massMatrix[3][2] = 0.0f;                massMatrix[3][3] = second_->State.InertiaTensor;
    //
    //Matrix InverseMassMatrix(4, 4);
    //InverseMassMatrix[0][0] = first_->State.InverseMass;  InverseMassMatrix[0][1] = 0.0f;                               InverseMassMatrix[0][2] = 0.0f;                       InverseMassMatrix[0][3] = 0.0f;
    //InverseMassMatrix[1][0] = 0.0f;                       InverseMassMatrix[1][1] = first_->State.InverseInertiaTensor; InverseMassMatrix[1][2] = 0.0f;                       InverseMassMatrix[1][3] = 0.0f;
    //InverseMassMatrix[2][0] = 0.0f;                       InverseMassMatrix[2][1] = 0.0f;                               InverseMassMatrix[2][2] = second_->State.InverseMass; InverseMassMatrix[2][3] = 0.0f;
    //InverseMassMatrix[3][0] = 0.0f;                       InverseMassMatrix[3][1] = 0.0f;                               InverseMassMatrix[3][2] = 0.0f;                       InverseMassMatrix[3][3] = second_->State.InverseInertiaTensor;
    //
    //Matrix b(3,3);
    //
    //b = -Jacobian * velocities.Transpose();
    //
    //Matrix A(Jacobian * InverseMassMatrix * JacobianTranspose);
    //
    //Matrix lambda(A.Invert() * b);
    //
    //Matrix dVelocities = lambda * Jacobian * InverseMassMatrix;

    
  }

}
