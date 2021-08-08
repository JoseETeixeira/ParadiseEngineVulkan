#pragma once


class Vec3
{
public:
	Vec3()
		: x(0.0f), y(0.0f), z(0.0f)
	{}

	Vec3(float x, float y, float z)
		: x(x), y(y), z(z)
	{}

	Vec3 operator+(Vec3 const& rhs) const
	{
		return Vec3(
			x + rhs.x,
			y + rhs.y,
			z + rhs.z);
	}

	Vec3 operator+=(Vec3 const& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	Vec3 operator-(Vec3 const& rhs) const
	{
		return Vec3(
			x - rhs.x,
			y - rhs.y,
			z - rhs.z);
	}

	Vec3 operator-=(Vec3 const& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	Vec3 operator*(Vec3 const& rhs) const
	{
		return Vec3(
			x * rhs.x,
			y * rhs.y,
			z * rhs.z);
	}

	Vec3 operator*=(Vec3 const& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;

		return *this;
	}

	Vec3 operator*(float rhs) const
	{
		return Vec3(
			x * rhs,
			y * rhs,
			z * rhs);
	}

	Vec3 operator*=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;

		return *this;
	}


	float x, y, z;
};
