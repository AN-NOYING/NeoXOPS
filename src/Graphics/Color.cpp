#include "Graphics/Color.hpp"

using namespace graphics;

/// @brief 기본 생성자
Color::Color() noexcept {
    A = 0, R = 0, G = 0, B = 0;
}

/// @brief 생성자
/// @param r Red (0 ~ 255)
/// @param g Green (0 ~ 255)
/// @param b Blue (0 ~ 255)
/// @param a Alpha (0 ~ 255)
Color::Color(int32_t r, int32_t g, int32_t b, int32_t a) noexcept {
    R = r;
    G = g;
    B = b;
    A = a;
}

/// @brief 생성자
/// @param colorCode 색상 코드
Color::Color(uint32_t colorCode) noexcept {
    A = (colorCode >> 24) & 0xFF;
    R = (colorCode >> 16) & 0xFF;
    G = (colorCode >>  8) & 0xFF;
    B = (colorCode)       & 0xFF;
}

/// @brief 생성자
/// @param clr Color
Color::Color(const Color& clr) noexcept {
    A = clr.A;
    R = clr.R;
    G = clr.G;
    B = clr.B;
}

/// @brief 정수 색상 코드를 취득합니다.
/// @param r Red (0 ~ 255)
/// @param g Green (0 ~ 255)
/// @param b Blue (0 ~ 255)
/// @param a Alpha (0 ~ 255)
/// @return 32비트 정수 색상 코드
uint32_t Color::GetColorCode(int32_t r, int32_t g, int32_t b, int32_t a) noexcept {
    return ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

/// @brief 정수 색상 코드를 취득합니다.
/// @return 32비트 정수 색상 코드
uint32_t Color::GetColorCode() const noexcept {
    return ((A & 0xFF) << 24) | ((R & 0xFF) << 16) | ((G & 0xFF) << 8) | (B & 0xFF);
}
