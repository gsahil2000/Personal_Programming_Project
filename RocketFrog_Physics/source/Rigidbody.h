#pragma once
#include "Precision.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Quaternion.h"

namespace RocketFrog
{
	class Rigidbody
	{
	public:
		Rigidbody();
		Rigidbody(const Rigidbody& a_rhs);
		~Rigidbody();

		/* Updates internal data from state data.
		   Called every frame to update the state.
		   This should be called after rigidbody's values have been altered
		   directly to see the effect that frame otherwise it'll be next frame.
		*/
		void CalculateDerivedData();


		/* Adds the given force vector in world space to
		   center of mass of this object. */
		void AddForce(const Vector3& a_vForce);

		void AddTorque(const Vector3& a_vDeltaTorque);
		void AddRotation(const Vector3& a_vDeltaRotation);
		void AddVelocity(const Vector3& a_vDeltaVelocity);

		/* Clears the accumulated forces. */
		void ClearAccumulators();

		/* Adds the given force to the given point on this rigid body.
		   Both the force and point in world space. 
		   It also updates the torque is force not applied exactly to center of mass. */
		void AddForceAtPoint(const Vector3& a_vForce, const Vector3& a_vPoint);

		/* Adds the given force in world space to point on this rigid body 
		   in local space. Usefull for spring forces, or other forces fixed to body. */
		void AddForceAtBodyPoint(const Vector3& a_vForce, const Vector3& a_vPoint);

		/* Converts and returns the given point from world space into the body's
           local space. */
        Vector3 GetPointInLocalSpace(const Vector3& point) const;

        /* Converts and returns the given point from world space into the body's
           local space. */
        Vector3 GetPointInWorldSpace(const Vector3& point) const;

        /**
         * Converts and sets the given direction from world space into the
         * body's local space. */
        Vector3 GetDirectionInLocalSpace(const Vector3& direction) const;

        /* Converts the given direction from world space into the
         * body's local space. */
        Vector3 GetDirectionInWorldSpace(const Vector3& direction) const;

		/* position accessors */
		Vector3 GetPosition() const;
		void	SetPosition(const Vector3& a_pos);

		/* velocity accessors */
		Vector3 GetVelocity() const;
		void	SetVelocity(const Vector3& a_vel);

		/// damping accessors
		number  GetDamping() const;
		void	SetDamping(const number a_damping);

		/// Mass accessors
		number  GetMass() const;
		number	GetInverseMass() const;
		void	SetMass(const number a_mass);
		void	SetInverseMass(const number a_inverseMass);

		/// returns if this particle has finite mass.
		bool	HasFiniteMass() const;

		/* angular damping accessors. */
		number GetAngularDamping() const;
		void   SetAngularDamping(const number a_nAngularDamping);		

		/* orientation accessors. */
		Quaternion GetOrientation() const;
		void       GetOrientation(Quaternion& a_outOrientation) const;
		void       SetOrientation(const Quaternion& a_qOrientation);

		/* rotation accessors. */
		Vector3 GetRotation() const;
		void    GetRotation(Vector3& a_outRotation) const;
		void    SetRotation(const Vector3& a_vRotation);

		/* transform matrix accessors. */
		Matrix3x4 GetTransformMatrix() const;
		void      GetTransformMatrix(Matrix3x4* a_mTransform) const;
		void      GetTransformMatrix(number a_mTransform[16]) const;
		void      GetGLTransform(float a_Transform[16]) const;

		/* inverse inertia tensor accessor. */
		Matrix3x3 GetInertiaTensor() const;
		void      GetInertiaTensor(Matrix3x3*  a_outInertiaTensor) const;

		/* Takes in the Inertia Tensor matrix and stores its inverse. */
		void      SetInertiaTensor(const Matrix3x3& a_inertiaTensor);

		/* inverse inertia tensor in world coordinates accessors. */
		Matrix3x3 GetInertiaTensorWorld() const;
		void      GetInertiaTensorWorld(Matrix3x3*  a_outInertiaTensor) const;
		void      SetInertiaTensorWorld(const Matrix3x3& a_inertiaTensor);

		/* Acceleration Accessors. */
		Vector3 GetAcceleration() const;
		void    SetAcceleration(const Vector3& a_vAcceleration);

		/* body is awake accessors. */
		bool GetBodyAwake() const;
		void SetAwake(const bool a_bAwake);

		/* CanSleep accessors. */
		bool GetCanSleep() const;
		void SetCanSleep(const bool a_bCanSleep);

		/// called every frame to update the particle physics state.
		void Integrate(number a_nDeltaTime);

	protected:
		/* Holds the inverse mass of this rigidbody. */
		number m_nInverseMass;

		/* Amount of damping applied to linear motion. */
		number m_nLinearDamping;

		/* Amount of damping applies to angular motion. */
		number m_nAngularDamping;

		/* Holds current position of the rigidbody. */
		Vector3 m_vPosition;

		/* Holds angular orientation of this rigidbody in worldspace. */
		Quaternion m_qOrientation;

		/* Holds linear velocity in worldspace. */
		Vector3 m_vVelocity;

		/* Holds the rotation in worldspace. */
		Vector3 m_vRotation;

		/* Holds transformation matrix to go from 
		   local space to world space and vice-versa. */
		Matrix3x4 m_mTransformMatrix;

		/* Holds the inverse of inertia tensor matrix.
		   The inertia tensor provided must not be degenerate
		   (that would mean body had zero inertia for spinning along one axis).
		   If tensor is finite, it will be invertible. */
		Matrix3x3 m_mInverseInertiaTensor;

		/* Holds inverse Inertia Tensor of this body in world space. */
		Matrix3x3 m_mInverseInertiaTensorWorld;

		/* Holds total force accumalted for that frame, and is cleared at end of each frame. */
		Vector3 m_vForceAccumulated;

		/* Holds total toque accumulated to rotate this object for that frame. */
		Vector3 m_vTorqeAccumulated;

		/* current status of this rigidbody for calcuations. */
		bool m_bIsAwake;

		/* check to see if this body can sleep. */
		bool m_bCanSleep;

		/* Holds the acceleration of the rigid body.  This value
          can be used to set acceleration due to gravity (its primary
          use), or any other constant acceleration. */
        Vector3 m_vAcceleration;
		Vector3 m_vAngularAcceleration;

        /* Holds the linear acceleration of the rigid body, for the previous frame. */
        Vector3 m_vLastFrameAcceleration;

	private:

	};
}