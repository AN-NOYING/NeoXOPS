#pragma once

#include <algorithm>
#include <cmath>
#include "Types.hpp"

inline namespace neoxops {
    /// @brief Vector2F
    struct Vector2F final {
        float X;            ///< X 좌표
        float Y;            ///< Y 좌표

        constexpr Vector2F() noexcept : X(0.0f), Y(0.0f) {}
        constexpr Vector2F(float x, float y) noexcept : X(x), Y(y) {}
        constexpr Vector2F(const Vector2F&) noexcept = default;
        Vector2F(Vector2F&&) noexcept = delete;
        
        static constexpr Vector2F Zero() { return { 0.0f, 0.0f }; }
        static constexpr Vector2F One() { return { 1.0f, 1.0f }; }
        static constexpr Vector2F Up() { return { 0.0f, 1.0f }; }
        static constexpr Vector2F Down() { return { 0.0f, -1.0f}; }
        static constexpr Vector2F Left() { return { -1.0f, 0.0f }; }
        static constexpr Vector2F Right() { return { 1.0f, 0.0f }; }

        /// @brief 근사 비교
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return 근사(true), 근사하지 않음(false)
        /// @note 소수점 오차로 인해 존재하는 함수
        static constexpr bool NearlyEquals(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            return (std::fabs(lhs.X - rhs.X) <= 1e-5) && (std::fabs(lhs.Y - rhs.Y) <= 1e-5);
        }

        /// @brief 내적
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return 내적
        static constexpr float Dot(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            return (lhs.X * rhs.X) + (lhs.Y * rhs.Y);
        }

        /// @brief 거리(길이)
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return 거리(길이)
        static constexpr float Distance(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            float dx = lhs.X - rhs.X;
            float dy = lhs.Y - rhs.Y;

            return std::sqrtf((dx * dx) + (dy * dy));
        }

        /// @brief 거리(길이) 제곱
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return 거리(길이) 제곱
        static constexpr float DistanceSquared(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            float dx = lhs.X - rhs.X;
            float dy = lhs.Y - rhs.Y;

            return ((dx * dx) + (dy * dy));
        }

        /// @brief 선형 보간
        /// @param start 시작
        /// @param end 종료
        /// @param t 보간 계수 (0.0 ~ 1.0)
        /// @return Vector2F
        static Vector2F Lerp(const Vector2F& start, const Vector2F& end, float t) noexcept {
            t = std::clamp(t, 0.0f, 1.0f);
            return { start.X + (end.X - start.X) * t, start.Y + (end.Y - start.Y) * t };
        }

        /// @brief 최솟값
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return Vector2F
        static Vector2F Min(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            return {
                lhs.X < rhs.X ? lhs.X : rhs.X,
                lhs.Y < rhs.Y ? lhs.Y : rhs.Y
            };
        }

        /// @brief 최댓값
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return Vector2F
        static Vector2F Max(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            return {
                lhs.X > rhs.X ? lhs.X : rhs.X,
                lhs.Y > rhs.Y ? lhs.Y : rhs.Y
            };
        }

		/// @brief 정규화
		/// @return Vector2F
		Vector2F Normalize() const noexcept {
			float length = std::sqrtf((X*X) + (Y*Y));
			return (length > 1e-6f) ? (*this / length) : Vector2F{ 0.0f, 0.0f };
		}

		/// @brief 길이
		/// @return 길이
		constexpr float Length() const noexcept {
			return std::sqrtf((X * X) + (Y * Y));
		}

		/// @brief 길이 제곱
		/// @return 길이 제곱
		constexpr float LengthSquared() const noexcept {
			return ((X * X) + (Y * Y));
		}

        /// @brief 회전
        /// @param rad 각도 (라디안 단위)
        /// @return Vector2F
        constexpr Vector2F Rotate(float rad) noexcept {
            float c = std::cosf(rad);        // X축은 Cos
            float s = std::sinf(rad);        // Y축은 Sin

            return { X * c - Y * s, X * s + Y * c };
        }

		/// @brief 복사 대입 연산자 오버로딩
		Vector2F& operator=(const Vector2F&) noexcept = default;

		/// @brief 이동 대입 연산자 제거
		Vector2F& operator=(Vector2F&&) noexcept = delete;

        /// @brief + 단항 연산자 오버로딩
        constexpr Vector2F operator+() const noexcept { return *this; }

        /// @brief - 단항 연산자 오버로딩
        constexpr Vector2F operator-() const noexcept { return { -X, -Y }; }

        /// @brief + 연산자 오버로딩
        /// @param vec Vector2F
        /// @return Vector2F
        constexpr Vector2F operator+(const Vector2F& vec) const noexcept {
            return { X + vec.X, Y + vec.Y };
        }

        /// @brief - 연산자 오버로딩
        /// @param vec Vector2F
        /// @return Vector2F
        constexpr Vector2F operator-(const Vector2F& vec) const noexcept {
            return { X - vec.X, Y - vec.Y };
        }

        /// @brief * 연산자 오버로딩
        /// @param vec Vector2F
        /// @return Vector2F
        constexpr Vector2F operator*(const Vector2F& vec) const noexcept {
            return { X * vec.X, Y * vec.Y };
        }

        /// @brief / 연산자 오버로딩
        /// @param vec Vector2F
        /// @return Vector2F
        /// @note 0으로 나누지 않도록 주의
        constexpr Vector2F operator/(const Vector2F& vec) const noexcept {
            float nx = std::fabs(vec.X) <= 1e-6f ? 0.0f : (X / vec.X);
            float ny = std::fabs(vec.Y) <= 1e-6f ? 0.0f : (Y / vec.Y);

            return { nx, ny };
        }

        /// @brief + 연산자 오버로딩
        /// @param value 값
        /// @return Vector2F
        constexpr Vector2F operator+(float value) const noexcept {
            return { X + value, Y + value };
        }

        /// @brief - 연산자 오버로딩
        /// @param value 값
        /// @return Vector2F
        constexpr Vector2F operator-(float value) const noexcept {
            return { X - value, Y - value };
        }

        /// @brief * 연산자 오버로딩
        /// @param value 값
        /// @return Vector2F
        constexpr Vector2F operator*(float value) const noexcept {
            return { X * value, Y * value };
        }

        /// @brief / 연산자 오버로딩
        /// @param value 값
        /// @return Vector2F
        constexpr Vector2F operator/(float value) const noexcept {
            float nx = std::fabs(value) <= 1e-6f ? 0.0f : (X / value);
            float ny = std::fabs(value) <= 1e-6f ? 0.0f : (Y / value);

            return { nx, ny };
        }

        /// @brief += 연산자 오버로딩
        /// @param vec Vector2F
        /// @return Vector2F
        Vector2F& operator+=(const Vector2F& vec) noexcept {
            X += vec.X;
            Y += vec.Y;

            return *this;
        }

        /// @brief -= 연산자 오버로딩
        /// @param vec Vector2F
        /// @return Vector2F
        Vector2F& operator-=(const Vector2F& vec) noexcept {
            X -= vec.X;
            Y -= vec.Y;

            return *this;
        }

        /// @brief *= 연산자 오버로딩
        /// @param vec Vector2F
        /// @return Vector2F
        Vector2F& operator*=(const Vector2F& vec) noexcept {
            X *= vec.X;
            Y *= vec.Y;

            return *this;
        }

        /// @brief /= 연산자 오버로딩
        /// @param vec Vector2F
        /// @return Vector2F
        Vector2F& operator/=(const Vector2F& vec) noexcept {
            if (std::fabs(vec.X) > 1e-6f) { X /= vec.X; } else { X = 0.0f; }
            if (std::fabs(vec.Y) > 1e-6f) { Y /= vec.Y; } else { Y = 0.0f; }

            return *this;
        }

        /// @brief += 연산자 오버로딩
        /// @param value 값
        /// @return Vector2F
        Vector2F& operator+=(float value) noexcept {
            X += value;
            Y += value;

            return *this;
        }

        /// @brief -= 연산자 오버로딩
        /// @param value 값
        /// @return Vector2F
        Vector2F& operator-=(float value) noexcept {
            X -= value;
            Y -= value;

            return *this;
        }

        /// @brief *= 연산자 오버로딩
        /// @param value 값
        /// @return Vector2F
        Vector2F& operator*=(float value) noexcept {
            X *= value;
            Y *= value;

            return *this;
        }

        /// @brief /= 연산자 오버로딩
        /// @param value 값
        /// @return Vector2F
        Vector2F& operator/=(float value) noexcept {
            if (std::fabs(value) > 1e-6f) { X /= value; } else { X = 0.0f; }
            if (std::fabs(value) > 1e-6f) { Y /= value; } else { Y = 0.0f; }

            return *this;
        }

        /// @brief == 연산자 오버로딩
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return 같다(true), 다르다(false)
        friend constexpr bool operator==(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            return (lhs.X == rhs.X) &&  (lhs.Y == rhs.Y);
        }

        /// @brief != 연산자 오버로딩
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return 다르다(true), 같다(false)
        friend constexpr bool operator!=(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            return !(lhs == rhs);
        }

        /// @brief < 연산자 오버로딩
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return 우측 피연산자가 더 크다(true), 작다(false)
        friend constexpr bool operator<(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            return (lhs.X < rhs.X) || (lhs.X == rhs.X && lhs.Y < rhs.Y);
        }

        /// @brief > 연산자 오버로딩
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return 좌측 피연산자가 더 크다(true), 작다(false)
        friend constexpr bool operator>(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            return (lhs.X > rhs.X) || (lhs.X == rhs.X && lhs.Y > rhs.Y);
        }

        /// @brief <= 연산자 오버로딩
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return 우측 피연산자가 더 크거나 같다(true), 작다(false)
        friend constexpr bool operator<=(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            return (lhs < rhs) || (lhs == rhs);
        }

        /// @brief >= 연산자 오버로딩
        /// @param lhs Vector2F
        /// @param rhs Vector2F
        /// @return 좌측 피연산자가 더 크거나 같다(true), 작다(false)
        friend constexpr bool operator>=(const Vector2F& lhs, const Vector2F& rhs) noexcept {
            return (lhs > rhs) || (lhs == rhs);
        }
    };
}
