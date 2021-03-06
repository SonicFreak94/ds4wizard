#include "pch.h"
#include "Vector2.h"
#include "gmath.h"

const Vector2 Vector2::zero = { 0.0f, 0.0f };
const Vector2 Vector2::one  = { 1.0f, 1.0f };

Vector2::Vector2(float f)  // NOLINT(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
	: x(f),
	  y(f)
{
}

Vector2::Vector2(float x, float y)  // NOLINT(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
	: x(x),
	  y(y)
{
}

float Vector2::operator[](size_t i) const
{
	if (i >= 2)
	{
		throw;
	}

	return array[i];
}

Vector2 Vector2::operator+() const
{
	const Vector2 result = { +x, +y };
	return result;
}

Vector2 Vector2::operator-() const
{
	const Vector2 result = { -x, -y };
	return result;
}

Vector2 Vector2::operator+(const Vector2& rhs) const
{
	const Vector2 result = {
		x + rhs.x,
		y + rhs.y
	};

	return result;
}

Vector2 Vector2::operator-(const Vector2& rhs) const
{
	const Vector2 result = {
		x - rhs.x,
		y - rhs.y
	};

	return result;
}

Vector2 Vector2::operator*(const Vector2& rhs) const
{
	const Vector2 result = {
		x * rhs.x,
		y * rhs.y
	};

	return result;
}

Vector2 Vector2::operator/(const Vector2& rhs) const
{
	const Vector2 result = {
		x / rhs.x,
		y / rhs.y
	};

	return result;
}

void Vector2::operator+=(const Vector2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vector2::operator-=(const Vector2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vector2::operator/=(const Vector2& rhs)
{
	x /= rhs.x;
	y /= rhs.y;
}

void Vector2::operator*=(const Vector2& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
}

Vector2 Vector2::operator+(const float rhs) const
{
	const Vector2 result = {
		x + rhs,
		y + rhs
	};

	return result;
}

Vector2 Vector2::operator-(const float rhs) const
{
	const Vector2 result = {
		x - rhs,
		y - rhs
	};

	return result;
}

Vector2 Vector2::operator*(const float rhs) const
{
	const Vector2 result = {
		x * rhs,
		y * rhs
	};

	return result;
}

Vector2 Vector2::operator/(const float rhs) const
{
	const Vector2 result = {
		x / rhs,
		y / rhs
	};

	return result;
}

void Vector2::operator+=(const float rhs)
{
	x += rhs;
	y += rhs;
}

void Vector2::operator-=(const float rhs)
{
	x -= rhs;
	y -= rhs;
}

void Vector2::operator/=(const float rhs)
{
	x /= rhs;
	y /= rhs;
}

void Vector2::operator*=(const float rhs)
{
	x *= rhs;
	y *= rhs;
}

Vector2::operator float*()
{
	return array;
}

Vector2::operator const float*() const
{
	return array;
}

float Vector2::lengthSquared() const
{
	return x * x + y * y;
}

float Vector2::length() const
{
	return sqrt(lengthSquared());
}

void Vector2::normalize()
{
	const auto l = length();

	if (l == 0.0f)
	{
		x = y = 0.0f;
	}
	else
	{
		x = x / l;
		y = y / l;
	}
}

Vector2 Vector2::normalized() const
{
	Vector2 result = *this;
	result.normalize();
	return result;
}

bool Vector2::operator==(const Vector2& rhs) const
{
	return nearEqual(rhs);
}

bool Vector2::operator!=(const Vector2& rhs) const
{
	return !(*this == rhs);
}

bool Vector2::isNormalized() const
{
	return gmath::is_one(lengthSquared());
}

float Vector2::dot(const Vector2& rhs) const
{
	return (x * rhs.x) + (y * rhs.y);
}

bool Vector2::nearEqual(const Vector2& rhs) const
{
	return gmath::near_equal(x, rhs.x) &&
	       gmath::near_equal(y, rhs.y);
}

Vector2 Vector2::lerp(const Vector2& start, const Vector2& end, float amount)
{
	return
	{
		gmath::lerp(start.x, end.x, amount),
		gmath::lerp(start.y, end.y, amount)
	};
}

Vector2 Vector2::clamp(const Vector2& v, float lower, float upper)
{
	return
	{
		std::clamp(v.x, lower, upper),
		std::clamp(v.y, lower, upper)
	};
}

Vector2 operator*(float lhs, const Vector2& rhs)
{
	return rhs * lhs;
}
