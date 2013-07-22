#pragma once
#include "RigidBody.h"
#include <vector>

namespace RocketFrog
{
	class ForceGenerator
	{
	public:
		/* Pure virutal function so that all the derived classes have to 
		   implement this function. */
		virtual void UpdateForce(Rigidbody* a_pBody, const number a_nDeltaTime) = 0;
	};

	class GravityForce : public ForceGenerator
	{
	public:
		GravityForce(const Vector3& a_vGravity);

	private:
		/* Holds acceleration due to gravity. */
		Vector3 m_vGravity;

		/* Multilpied by gravity vector to modify components. */
		number m_nGravityModifier;

		/* Applies the gravitational force to the given body. */
		virtual void UpdateForce(Rigidbody* a_pBody, const number a_nDeltaTime);
	};

	class Spring : public ForceGenerator
	{
	public:
		Spring();
		Spring(const Vector3& a_vLocalConnectionPt,
			Rigidbody* a_pOtherBody,
			const Vector3& a_vOtherConnectionPt,
			number a_nSpringConstant,
			number a_nRestLength);

		virtual void UpdateForce(Rigidbody* a_pBody, const number a_nDeltaTime);

	private:
		Vector3 m_vConnectionPoint;
		Vector3 m_vOtherConnectionPoint;

		Rigidbody* m_pOtherBody;

		number m_nSpringConstant;
		number m_nRestLength;
	};

	/* Force Generator that applies an aerodyanmic force. */
	class Aero : public ForceGenerator
	{
	public:
		Aero(const Matrix3x3& a_mTensor, const Vector3& a_vPosition, const Vector3* a_pWindSpeed);

		virtual void UpdateForce(Rigidbody* a_pBody, const number a_nDeltaTime);

	protected:
		/* Uses an explicit tensor matrix to update force on given body. */
		void UpdateForceFromTensor(Rigidbody* a_pBody, const number a_nDeltaTime, const Matrix3x3& a_mTensor);

		/* Holds the aerodynamic tensor for the suface in local space. */
		Matrix3x3 m_mTensor;

		/* relative position of aerodynamic surface in local space. */
		Vector3 m_vPosition;

		/* Pointer to vector containing the wind speed of the environment. */
		const Vector3* m_vWindSpeed;

	private:

	};

	class AeroControl : public Aero
	{
	public:
		AeroControl(const Matrix3x3& a_mBase, const Matrix3x3& a_mMin, const Matrix3x3& a_mMax,
				const Vector3& a_vPosition, const Vector3* a_pWindSpeed);

		void SetControl(const number a_nValue);

		virtual void UpdateForce(Rigidbody* a_pBody, const number a_nDeltaTime);

	protected:
		/* Aero dynamic tensor for the surface when the control is at its maximum. */
		Matrix3x3 m_mMaxTensor;

		/* Aerodynamic tensor for the surface when control is at its max. */
		Matrix3x3 m_mMinTensor;

		/* Current position of control for this surface. Goes from -1 to +1.
		   -1 = MinTensor value, 0 = base-class Tensor value, +1 - MaxTensor value. */
		number m_nControlSettings;

	private:
		/* calcualte final tensor for current settings. */
		Matrix3x3 GetTensor();
	};

	/* Holds all the force generators and the bodies they apply to. */
    class ForceRegistry
    {
	public:
        /* Registers the given force generator to apply to the given body. */
        void Add(Rigidbody* a_pBody, ForceGenerator *a_pFG);

        /* Removes the given registered pair from the registry(if present, nothing otherwise). */
        void Remove(Rigidbody* a_pBody, ForceGenerator *a_pFG);

        /* Clears all registrations from registry. Does not delete though. */
        void Clear();

        /* Calls all the force generators to update the forces of their corresponding bodies. */
        void UpdateForces(const number a_nDeltaTime);

    protected:

        /* Keeps track of one force generator and the body it applies to. */
        struct ForceRegistration
        {
			Rigidbody*       m_pBody;
            ForceGenerator * m_pFG;
        };

        /* Holds the list of registrations. */
        typedef std::vector<ForceRegistration> RegistryList;
        
		RegistryList m_Registrations;
    };
}