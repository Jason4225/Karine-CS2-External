#pragma once
#include <numbers>
#include <cmath>

struct Vector3_t
{
	constexpr Vector3_t(const float x = 0.f, const float y = 0.f, const float z = 0.f) noexcept : x(x), y(y), z(z) { }

	constexpr const Vector3_t& operator-(const Vector3_t& other) const noexcept
	{
		return Vector3_t{ x - other.x, y - other.y, z - other.z };
	}

	constexpr const Vector3_t& operator+(const Vector3_t& other) const noexcept
	{
		return Vector3_t{ x + other.x, y + other.y, z + other.z };
	}

	constexpr const Vector3_t& operator/(const float factor) const noexcept
	{
		return Vector3_t{ x / factor, y / factor, z / factor };
	}

	constexpr const Vector3_t& operator*(const float factor) const noexcept
	{
		return Vector3_t{ x * factor, y * factor, z * factor };
	}

	constexpr const bool operator>(const Vector3_t& other) const noexcept 
	{
		return x > other.x && y > other.y && z > other.z;
	}

	constexpr const bool operator>=(const Vector3_t& other) const noexcept 
	{
		return x >= other.x && y >= other.y && z >= other.z;
	}

	constexpr const bool operator<(const Vector3_t& other) const noexcept 
	{
		return x < other.x && y < other.y && z < other.z;
	}

	constexpr const bool operator<=(const Vector3_t& other) const noexcept 
	{
		return x <= other.x && y <= other.y && z <= other.z;
	}

	constexpr const Vector3_t& ToAngle() const noexcept
	{
		return Vector3_t{ std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>), std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>), 0.0f };
	}

	float Length() const 
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	float Length2D() const 
	{
		return std::sqrt(x * x + y * y);
	}

	constexpr const bool IsZero() const noexcept
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	float CalculateDistance(const Vector3_t& point) const 
	{
		float dx = point.x - x;
		float dy = point.y - y;
		float dz = point.z - z;

		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	float x, y, z;
};