#pragma once

#include <algorithm>
#include <cmath>
#include "Types.hpp"

inline namespace neoxops {
    /// @brief Vector3F
    struct Vector3F final {
        float X;            ///< X 좌표
        float Y;            ///< Y 좌표
        float Z;            ///< Z 좌표

        constexpr Vector3F() noexcept : X(0.0f), Y(0.0f), Z(0.0f) {}
        constexpr Vector3F(float x, float y, float z) noexcept : X(x), Y(y), Z(z) {}
        constexpr Vector3F(const Vector3F& vec) noexcept = default;
        Vector3F(Vector3F&&) noexcept = delete;
        
        /// @brief Zero
        /// @return Vector3F
        static constexpr Vector3F Zero() { return { 0.0f, 0.0f, 0.0f }; }

        /// @brief One
        /// @return Vector3F
        static constexpr Vector3F One() { return { 1.0f, 1.0f, 1.0f }; }

        /// @brief 상방
        /// @return Vector3F
        static constexpr Vector3F Up() { return { 0.0f, 1.0f, 0.0f }; }

        /// @brief 하방
        /// @return Vector3F
        static constexpr Vector3F Down() { return { 0.0f, -1.0f, 0.0f }; }

        /// @brief 좌
        /// @return Vector3F
        static constexpr Vector3F Left() { return { -1.0f, 0.0f, 0.0f }; }

        /// @brief 우
        /// @return Vector3F
        static constexpr Vector3F Right() { return { 1.0f, 0.0f, 0.0f }; }

        /// @brief 전방
        /// @return Vector3F
        static constexpr Vector3F Forward() { return { 0.0f, 0.0f, 1.0f }; }

        /// @brief 후방
        /// @return Vector3F
        static constexpr Vector3F Backward() { return { 0.0f, 0.0f, -1.0f }; }

        /// @brief 근사 비교
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return 근사(true), 비근사(false)
        static constexpr bool NearlyEquals(const Vector3F& lhs, const Vector3F& rhs) noexcept {
            return (std::fabs(lhs.X - rhs.X) <= 1e-5f) && (std::fabs(lhs.Y - rhs.Y) <= 1e-5f) && (std::fabs(lhs.Z - rhs.Z) <= 1e-5f);
        }

        /// @brief 내적
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return 내적
        static constexpr float Dot(const Vector3F& lhs, const Vector3F& rhs) noexcept {
            return (lhs.X * lhs.Y) + (lhs.Y * rhs.Y) + (lhs.Z * rhs.Z);
        }

        /// @brief 외적
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return Vector3F
        static constexpr Vector3F Cross(const Vector3F& lhs, const Vector3F& rhs) noexcept {
            return {
                lhs.Y * rhs.Z - lhs.Z * rhs.Y,
                lhs.Z * rhs.X - lhs.X * rhs.Z,
                lhs.X * rhs.Y - lhs.Y * rhs.X
            };
        }

        /// @brief 거리
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return 거리
        static constexpr float Distance(const Vector3F& lhs, const Vector3F& rhs) noexcept {
            float dx = lhs.X - rhs.X;
            float dy = lhs.Y - rhs.Y;
            float dz = lhs.Z - rhs.Z;

            return std::sqrtf((dx*dx) + (dy*dy) + (dz*dz));
        }

        /// @brief 거리 제곱
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return 거리 제곱
        static constexpr float DistanceSquared(const Vector3F& lhs, const Vector3F& rhs) noexcept {
            float dx = lhs.X - rhs.X;
            float dy = lhs.Y - rhs.Y;
            float dz = lhs.Z - rhs.Z;

            return ((dx * dx) + (dy * dy) + (dz * dz));
        }

        /// @brief 선형 보간
        /// @param start Vector3F
        /// @param end Vector3F
        /// @param t 보간 계수 (0.0 ~ 1.0)
        /// @return Vector3F
        static constexpr Vector3F Lerp(const Vector3F& start, const Vector3F& end, float t) noexcept {
            t = std::clamp(t, 0.0f, 1.0f);
            return {
                start.X + (end.X - start.X) * t,
                start.Y + (end.Y - start.Y) * t,
                start.Z + (end.Z - start.Z) * t
            };
        }

        /// @brief 최솟값
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return Vector3F
        static constexpr Vector3F Min(const Vector3F& lhs, const Vector3F& rhs) noexcept {
            return {
                lhs.X < rhs.X ? lhs.X : rhs.X,
                lhs.Y < rhs.Y ? lhs.Y : rhs.Y,
                lhs.Z < rhs.Z ? lhs.Z : rhs.Z
            };
        }

        /// @brief 최댓값
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return Vector3F
        static constexpr Vector3F Max(const Vector3F& lhs, const Vector3F& rhs) noexcept {
            return {
                lhs.X > rhs.X ? lhs.X : rhs.X,
                lhs.Y > rhs.Y ? lhs.Y : rhs.Y,
                lhs.Z > rhs.Z ? lhs.Z : rhs.Z
            };
        }

        /// @brief 정규화
        /// @return Vector3F
        constexpr Vector3F Normalize() const noexcept {
            float length = Length();
            return (length > 1e-6f) ? (*this / length) : Vector3F::Zero();
        }

        /// @brief 길이
        /// @return 길이
        constexpr float Length() const noexcept {
            return std::sqrtf((X*X) + (Y*Y) + (Z*Z));
        }

        /// @brief 길이 제곱
        /// @return 길이 제곱
        constexpr float LengthSquared() const noexcept {
            return ((X*X) + (Y*Y) + (Z*Z));
        }

        Vector3F& operator=(const Vector3F&) noexcept = default;
        Vector3F& operator=(Vector3F&&) noexcept = delete;

        /// @brief + 단항 연산자 오버로딩
        /// @return Vector3F
        const Vector3F operator+() const noexcept {
            return *this;
        }

        /// @brief - 단항 연산자 오버로딩
        /// @return Vector3F
        const Vector3F operator-() const noexcept {
            return { -X, -Y, -Z };
        }

        /// @brief + 연산자 오버로딩
        /// @param vec Vector3F
        /// @return Vector3F
        constexpr Vector3F operator+(const Vector3F& vec) const noexcept {
            return { X + vec.X, Y + vec.Y, Z + vec.Z };
        }

        /// @brief - 연산자 오버로딩
        /// @param vec Vector3F
        /// @return Vector3F
        constexpr Vector3F operator-(const Vector3F& vec) const noexcept {
            return { X - vec.X, Y - vec.Y, Z - vec.Z };
        }

        /// @brief * 연산자 오버로딩
        /// @param vec Vector3F
        /// @return Vector3F
        constexpr Vector3F operator*(const Vector3F& vec) const noexcept {
            return { X * vec.X, Y * vec.Y, Z * vec.Z };
        }

        /// @brief / 연산자 오버로딩
        /// @param vec Vector3F
        /// @return Vector3F
        constexpr Vector3F operator/(const Vector3F& vec) const noexcept {
            return {
                std::fabs(vec.X) <= 1e-6f ? 0.0f : (X / vec.X),
                std::fabs(vec.Y) <= 1e-6f ? 0.0f : (Y / vec.Y),
                std::fabs(vec.Z) <= 1e-6f ? 0.0f : (Z / vec.Z)
            };
        }

        /// @brief + 연산자 오버로딩
        /// @param value 값
        /// @return Vector3F
        constexpr Vector3F operator+(float value) const noexcept {
            return { X + value, Y + value, Z + value };
        }

        /// @brief - 연산자 오버로딩
        /// @param value 값
        /// @return Vector3F
        constexpr Vector3F operator-(float value) const noexcept {
            return { X - value, Y - value, Z - value };
        }

        /// @brief * 연산자 오버로딩
        /// @param value 값
        /// @return Vector3F
        constexpr Vector3F operator*(float value) const noexcept {
            return { X * value, Y * value, Z * value };
        }

        /// @brief / 연산자 오버로딩
        /// @param value 값
        /// @return Vector3F
        constexpr Vector3F operator/(float value) const noexcept {
            return {
                std::fabs(value) <= 1e-6f ? 0.0f : (X / value),
                std::fabs(value) <= 1e-6f ? 0.0f : (Y / value),
                std::fabs(value) <= 1e-6f ? 0.0f : (Z / value)
            };
        }

        /// @brief += 연산자 오버로딩
        /// @param vec Vector3F
        /// @return Vector3F
        Vector3F& operator+=(const Vector3F& vec) noexcept {
            X += vec.X;
            Y += vec.Y;
            Z += vec.Z;

            return *this;
        }

        /// @brief -= 연산자 오버로딩
        /// @param vec Vector3F
        /// @return Vector3F
        Vector3F& operator-=(const Vector3F& vec) noexcept {
            X -= vec.X;
            Y -= vec.Y;
            Z -= vec.Z;

            return *this;
        }

        /// @brief *= 연산자 오버로딩
        /// @param vec Vector3F
        /// @return Vector3F
        Vector3F& operator*=(const Vector3F& vec) noexcept {
            X *= vec.X;
            Y *= vec.Y;
            Z *= vec.Z;

            return *this;
        }

        /// @brief /= 연산자 오버로딩
        /// @param vec Vector3F
        /// @return Vector3F
        Vector3F& operator/=(const Vector3F& vec) noexcept {
            X = (std::fabs(vec.X) < 1e-6f) ? 0.0f : X / vec.X;
            Y = (std::fabs(vec.Y) < 1e-6f) ? 0.0f : Y / vec.Y;
            Z = (std::fabs(vec.Z) < 1e-6f) ? 0.0f : Z / vec.Z;

            return *this;
        }

        /// @brief += 연산자 오버로딩
        /// @param value 값
        /// @return Vector3F
        Vector3F& operator+=(float value) noexcept {
            X += value;
            Y += value;
            Z += value;

            return *this;
        }

        /// @brief -= 연산자 오버로딩
        /// @param value 값
        /// @return Vector3F
        Vector3F& operator-=(float value) noexcept {
            X -= value;
            Y -= value;
            Z -= value;

            return *this;
        }

        /// @brief *= 연산자 오버로딩
        /// @param value 값
        /// @return Vector3F
        Vector3F& operator*=(float value) noexcept {
            X *= value;
            Y *= value;
            Z *= value;

            return *this;
        }

        /// @brief /= 연산자 오버로딩
        /// @param value 값
        /// @return Vector3F
        Vector3F& operator/=(float value) noexcept {
            X = (std::fabs(value) < 1e-6f) ? 0.0f : X / value;
            Y = (std::fabs(value) < 1e-6f) ? 0.0f : Y / value;
            Z = (std::fabs(value) < 1e-6f) ? 0.0f : Z / value;

            return *this;
        }

        /// @brief == 연산자 오버로딩
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return 같다(true), 다르다(false)
        friend constexpr bool operator==(const Vector3F &lhs, const Vector3F &rhs) noexcept
        {
            return (lhs.X == rhs.X) && (lhs.Y == rhs.Y) && (lhs.Z == rhs.Z);
        }

        /// @brief != 연산자 오버로딩
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return 다르다(true), 같다(false)
        friend constexpr bool operator!=(const Vector3F &lhs, const Vector3F &rhs) noexcept { 
            return !(lhs == rhs); 
        }

        /// @brief < 연산자 오버로딩
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return 우측 피연산자가 더 크다(true), 작다(false)
        friend constexpr bool operator<(const Vector3F &lhs, const Vector3F &rhs) noexcept {
            return (lhs.X < rhs.X) || (lhs.X == rhs.X && (lhs.Y < rhs.Y || (lhs.Y == rhs.Y && lhs.Z < rhs.Z)));
        }

        /// @brief > 연산자 오버로딩
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return 좌측 피연산자가 더 크다(true), 작다(false)
        friend constexpr bool operator>(const Vector3F &lhs, const Vector3F &rhs) noexcept {
            return (lhs.X > rhs.X) || (lhs.X == rhs.X && (lhs.Y > rhs.Y || (lhs.Y == rhs.Y && lhs.Z > rhs.Z)));
        }

        /// @brief <= 연산자 오버로딩
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return 우측 피연산자가 더 크거나 같다(true), 작다(false)
        friend constexpr bool operator<=(const Vector3F &lhs, const Vector3F &rhs) noexcept { 
            return (lhs < rhs) || (lhs == rhs); 
        }

        /// @brief >= 연산자 오버로딩
        /// @param lhs Vector3F
        /// @param rhs Vector3F
        /// @return 좌측 피연산자가 더 크거나 같다(true), 작다(false)
        friend constexpr bool operator>=(const Vector3F &lhs, const Vector3F &rhs) noexcept { 
            return (lhs > rhs) || (lhs == rhs); 
        }
    };
}