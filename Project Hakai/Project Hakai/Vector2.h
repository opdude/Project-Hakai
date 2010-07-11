#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>
#include <iosfwd>

class Vector2
{
public:
	Vector2()
	{
		x=0;
		y=0;
	}
	Vector2(float new_x, float new_y)
	{
		set(new_x, new_y);
	}

	void set(float new_x, float new_y)
	{
		x = new_x;
		y = new_y;
	}

	void operator=(float value)
	{
		set(value, value);
	}

	void operator+=(const Vector2& other)
	{
		set(x + other.x, y + other.y);
	}
	void operator-=(const Vector2& other)
	{
		set(x - other.x, y - other.y);
	}
	void operator*=(float factor)
	{
		set(x * factor, y * factor);
	}
	void operator/=(float factor)
	{
		set(x / factor, y / factor);
	}

	float lengthSquared() const
	{
		return x*x + y*y;
	}
	float length() const
	{
		return std::sqrt(lengthSquared());
	}
	void normalize()
	{
		operator/=(length());
	}
	void invert()
	{
		x = -x;
		y = -y;
	}

	float x,y;
};

inline const Vector2 operator+(const Vector2& one, const Vector2& two)
{
	return Vector2(one.x + two.x, one.y + two.y);
}
inline const Vector2 operator-(const Vector2& one, const Vector2& two)
{
	return Vector2(one.x - two.x, one.y - two.y);
}
inline const Vector2 operator-(const Vector2& one)
{
	return Vector2(-one.x, -one.y);
}
inline const Vector2 operator*(const Vector2& one, float factor)
{
	return Vector2(one.x * factor, one.y * factor);
}
inline const Vector2 operator*(float factor, const Vector2& one)
{
	return Vector2(one.x * factor, one.y * factor);
}
inline const Vector2 operator/(const Vector2& one, float factor)
{
	return Vector2(one.x / factor, one.y / factor);
}
inline float operator*(const Vector2& one, const Vector2& two){ //dotproduct
	return one.x * two.x + one.y * two.y;
}

inline bool operator==(const Vector2& one, const Vector2& two)
{
	return one.x == two.x && one.y == two.y;
}
inline bool operator!=(const Vector2& one, const Vector2& two)
{
	return !(one == two);
}

#endif //VECTOR2_HPP 