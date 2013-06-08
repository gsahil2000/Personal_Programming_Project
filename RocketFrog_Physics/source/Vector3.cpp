#include "Vector3.h"
#include <exception>

// Disable warning message 4100(unreferenced Param), 4305(converting float to number).
#pragma warning( push )
#pragma warning( disable : 4100 4305 )

namespace RocketFrog
{
	const Vector3 Vector3::GRAVITY		 = Vector3(0, -9.81, 0);
	const Vector3 Vector3::RIGHT		 = Vector3(1, 0, 0);
	const Vector3 Vector3::UP			 = Vector3(0, 1, 0);
	const Vector3 Vector3::OUT_OF_SCREEN = Vector3(0, 0, 1);
	const Vector3 Vector3::X		     = Vector3(1, 0, 0);
	const Vector3 Vector3::Y			 = Vector3(0, 1, 0);
	const Vector3 Vector3::Z             = Vector3(0, 0, 1);

	Vector3::Vector3()
		:x(0), y(0), z(0), padding(0)
	{}

	Vector3::Vector3(const Vector3& a_rhs)
		:x(a_rhs.x), y(a_rhs.y), z(a_rhs.z)
	{}

	Vector3::Vector3(const number a_x,const number a_y,const number a_z)
		:x(a_x), y(a_y), z(a_z), padding(0)
	{}

	Vector3::Vector3(const number a_all)
		:x(a_all), y(a_all), z(a_all), padding(0)
	{}

	Vector3& Vector3::operator=(const Vector3& a_rhs)
	{
		x = a_rhs.x;
		y = a_rhs.y;
		z = a_rhs.z;

		return *this;
	}

	Vector3::~Vector3()
	{}

	number Vector3::operator[](const unsigned int i) const
	{
		switch(i)
		{
		case 0:
			return x;

		case 1:
			return y;

		case 2:
			return z;

		default:
			throw std::exception("Index out of bound.");
		}
	}

	number& Vector3::operator[](const unsigned int i)
	{
		switch (i)
		{
		case 0:
			return x;

		case 1:
			return y;

		case 2:
			return z;

		default:
			throw std::exception("Index out of bound.");
		}
	}

	bool Vector3::operator==(const Vector3& a_rhs) const
	{
		return (x==a_rhs.x && y==a_rhs.y && z==a_rhs.z);
	}

	bool Vector3::operator!=(const Vector3& a_rhs) const
	{
		return !operator==(a_rhs);
	}

	bool Vector3::operator>(const Vector3& a_rhs) const
	{
		return (x>a_rhs.x && y>a_rhs.y && z>a_rhs.z);
	}

	bool Vector3::operator>=(const Vector3& a_rhs) const
	{
		return (x>=a_rhs.x && y>=a_rhs.y && z>=a_rhs.z);
	}

	bool Vector3::operator<(const Vector3& a_rhs) const
	{
		return (x<a_rhs.x && y<a_rhs.y && z<a_rhs.z);
	}

	bool Vector3::operator<=(const Vector3& a_rhs) const
	{
		return (x<=a_rhs.x && y<=a_rhs.y && z<=a_rhs.z);
	}

	void Vector3::Invert()
	{
		/// Flips all the values
		x = -x;
		y = -y;
		z = -z;
	}

	Vector3 Vector3::operator+(const Vector3& a_rhs) const
	{
		return Vector3(x+a_rhs.x, y+a_rhs.y, z+a_rhs.z);
	}

	void Vector3::operator+=(const Vector3& a_rhs)
	{
		x += a_rhs.x;
		y += a_rhs.y;
		z += a_rhs.z;
	}

	Vector3 Vector3::operator-(const Vector3& a_rhs) const
	{
		return Vector3(x-a_rhs.x, y-a_rhs.y, z-a_rhs.z);
	}

	void Vector3::operator-=(const Vector3& a_rhs)
	{
		x -= a_rhs.x;
		y -= a_rhs.y;
		z -= a_rhs.z;
	}

	Vector3 Vector3::operator*(const number a_scalar) const
	{
		return Vector3(x*a_scalar, y*a_scalar, z*a_scalar);
	}

	Vector3 Vector3::operator*(const Vector3& a_rhs) const
	{
		return Vector3(x*a_rhs.x, y*a_rhs.y, z*a_rhs.z);
	}

	void Vector3::operator*=(const number a_scalar)
	{
		x *= a_scalar;
		y *= a_scalar;
		z *= a_scalar;
	}

	void Vector3::operator*=(const Vector3& a_rhs)
	{
		x *= a_rhs.x;
		y *= a_rhs.y;
		z *= a_rhs.z;
	}

	void Vector3::AddScaledVector(const Vector3& a_vector, const number a_scale)
	{
		x += (a_vector.x * a_scale);
		y += (a_vector.y * a_scale);
		z += (a_vector.z * a_scale);
	}

	number Vector3::Magnitude() const
	{
		return num_sqrt(x*x + y*y + z*z);
	}

	number Vector3::SquaredMagnitude() const
	{
		return (x*x + y*y + z*z);
	}

	void Vector3::Normalize()
	{
		number _mag = Magnitude();

		if (_mag > 0)
		{
			(*this) *= ( 1 / _mag);
		}
	}

	number Vector3::DotProduct(const Vector3& a_rhs) const
	{
		return (x*a_rhs.x + y*a_rhs.y + z*a_rhs.z);
	}

	Vector3 Vector3::CrossProduct(const Vector3& a_rhs) const
	{
		Vector3 _product;
		_product.x = y*a_rhs.z - z*a_rhs.y;
		_product.y = z*a_rhs.x - x*a_rhs.z;
		_product.z = x*a_rhs.y - y*a_rhs.x;

		return _product;
	}

	void Vector3::CrossProductUpdate(const Vector3& a_rhs)
	{
		*this = CrossProduct(a_rhs);
	}

	void Vector3::Clear()
	{
		x = y = z = 0;
	}
}

#pragma warning (pop)