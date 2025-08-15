#pragma once

#include "../Type/Types.hpp"

inline namespace neoxops {
    namespace graphics {
        /// @brief 색상 구조체
        struct Color final {
            byte_t A;            ///< Alpha
            byte_t R;            ///< Red
            byte_t G;            ///< Green
            byte_t B;            ///< Blue

            Color() noexcept;
            Color(int32_t, int32_t, int32_t, int32_t a = 255) noexcept;
            Color(uint32_t) noexcept;
            Color(const Color&) noexcept;
            Color(Color&&) noexcept = delete;

            [[nodiscard]] static uint32_t GetColorCode(int32_t, int32_t, int32_t, int32_t a = 255) noexcept;
            [[nodiscard]] uint32_t GetColorCode() const noexcept;

            /// @brief == 연산자 오버로딩
            /// @param lhs Color
            /// @param rhs Color
            /// @return 같음(true), 다름(false)
            friend constexpr bool operator==(const Color& lhs, const Color& rhs) noexcept {
                return (lhs.A == rhs.A) && (lhs.R == rhs.R) && (lhs.G == rhs.G) && (lhs.B == rhs.B);
            }

            /// @brief != 연산자 오버로딩
            /// @param lhs Color
            /// @param rhs Color
            /// @return 다름(true), 같음(false)
            friend constexpr bool operator!=(const Color& lhs, const Color& rhs) noexcept {
                return !(lhs == rhs);
            }

            static constexpr uint32_t Transparent           = 0x00000000;
            static constexpr uint32_t AliceBlue             = 0xFFF0F8FF;
            static constexpr uint32_t AntiqueWhite          = 0xFFFAEBD7;
            static constexpr uint32_t Aqua                  = 0xFF00FFFF;
            static constexpr uint32_t Aquamarine            = 0xFF7FFFD4;
            static constexpr uint32_t Azure                 = 0xFFF0FFFF;
            static constexpr uint32_t Beige                 = 0xFFF5F5DC;
            static constexpr uint32_t Bisque                = 0xFFFFE4C4;
            static constexpr uint32_t Black                 = 0xFF000000;
            static constexpr uint32_t BlanchedAlmond        = 0xFFFFEBCD;
            static constexpr uint32_t Blue                  = 0xFF0000FF;
            static constexpr uint32_t BlueViolet            = 0xFF8A2BE2;
            static constexpr uint32_t Brown                 = 0xFFA52A2A;
            static constexpr uint32_t BurlyWood             = 0xFFDEB887;
            static constexpr uint32_t CadetBlue             = 0xFF5F9EA0;
            static constexpr uint32_t Chartreuse            = 0xFF7FFF00;
            static constexpr uint32_t Chocolate             = 0xFFD2691E;
            static constexpr uint32_t Coral                 = 0xFFFF7F50;
            static constexpr uint32_t CornflowerBlue        = 0xFF6495ED;
            static constexpr uint32_t Cornsilk              = 0xFFFFF8DC;
            static constexpr uint32_t Crimson               = 0xFFDC143C;
            static constexpr uint32_t Cyan                  = 0xFF00FFFF;
            static constexpr uint32_t DarkBlue              = 0xFF00008B;
            static constexpr uint32_t DarkCyan              = 0xFF008B8B;
            static constexpr uint32_t DarkGoldenrod         = 0xFFB8860B;
            static constexpr uint32_t DarkGray              = 0xFFA9A9A9;
            static constexpr uint32_t DarkGreen             = 0xFF006400;
            static constexpr uint32_t DarkKhaki             = 0xFFBDB76B;
            static constexpr uint32_t DarkMagenta           = 0xFF8B008B;
            static constexpr uint32_t DarkOliveGreen        = 0xFF556B2F;
            static constexpr uint32_t DarkOrange            = 0xFFFF8C00;
            static constexpr uint32_t DarkOrchid            = 0xFF9932CC;
            static constexpr uint32_t DarkRed               = 0xFF8B0000;
            static constexpr uint32_t DarkSalmon            = 0xFFE9967A;
            static constexpr uint32_t DarkSeaGreen          = 0xFF8FBC8B;
            static constexpr uint32_t DarkSlateBlue         = 0xFF483D8B;
            static constexpr uint32_t DarkSlateGray         = 0xFF2F4F4F;
            static constexpr uint32_t DarkTurquoise         = 0xFF00CED1;
            static constexpr uint32_t DarkViolet            = 0xFF9400D3;
            static constexpr uint32_t DeepPink              = 0xFFFF1493;
            static constexpr uint32_t DeepSkyBlue           = 0xFF00BFFF;
            static constexpr uint32_t DimGray               = 0xFF696969;
            static constexpr uint32_t DodgerBlue            = 0xFF1E90FF;
            static constexpr uint32_t Firebrick             = 0xFFB22222;
            static constexpr uint32_t FloralWhite           = 0xFFFFFAF0;
            static constexpr uint32_t ForestGreen           = 0xFF228B22;
            static constexpr uint32_t Fuchsia               = 0xFFFF00FF;
            static constexpr uint32_t Gainsboro             = 0xFFDCDCDC;
            static constexpr uint32_t GhostWhite            = 0xFFF8F8FF;
            static constexpr uint32_t Gold                  = 0xFFFFD700;
            static constexpr uint32_t Goldenrod             = 0xFFDAA520;
            static constexpr uint32_t Gray                  = 0xFF808080;
            static constexpr uint32_t Green                 = 0xFF008000;
            static constexpr uint32_t GreenYellow           = 0xFFADFF2F;
            static constexpr uint32_t Honeydew              = 0xFFF0FFF0;
            static constexpr uint32_t HotPink               = 0xFFFF69B4;
            static constexpr uint32_t IndianRed             = 0xFFCD5C5C;
            static constexpr uint32_t Indigo                = 0xFF4B0082;
            static constexpr uint32_t Ivory                 = 0xFFFFFFF0;
            static constexpr uint32_t Khaki                 = 0xFFF0E68C;
            static constexpr uint32_t Lavender              = 0xFFE6E6FA;
            static constexpr uint32_t LavenderBlush         = 0xFFFFF0F5;
            static constexpr uint32_t LawnGreen             = 0xFF7CFC00;
            static constexpr uint32_t LemonChiffon          = 0xFFFFFACD;
            static constexpr uint32_t LightBlue             = 0xFFADD8E6;
            static constexpr uint32_t LightCoral            = 0xFFF08080;
            static constexpr uint32_t LightCyan             = 0xFFE0FFFF;
            static constexpr uint32_t LightGoldenrodYellow  = 0xFFFAFAD2;
            static constexpr uint32_t LightGray             = 0xFFD3D3D3;
            static constexpr uint32_t LightGreen            = 0xFF90EE90;
            static constexpr uint32_t LightPink             = 0xFFFFB6C1;
            static constexpr uint32_t LightSalmon           = 0xFFFFA07A;
            static constexpr uint32_t LightSeaGreen         = 0xFF20B2AA;
            static constexpr uint32_t LightSkyBlue          = 0xFF87CEFA;
            static constexpr uint32_t LightSlateGray        = 0xFF778899;
            static constexpr uint32_t LightSteelBlue        = 0xFFB0C4DE;
            static constexpr uint32_t LightYellow           = 0xFFFFFFE0;
            static constexpr uint32_t Lime                  = 0xFF00FF00;
            static constexpr uint32_t LimeGreen             = 0xFF32CD32;
            static constexpr uint32_t Linen                 = 0xFFFAF0E6;
            static constexpr uint32_t Magenta               = 0xFFFF00FF;
            static constexpr uint32_t Maroon                = 0xFF800000;
            static constexpr uint32_t MediumAquamarine      = 0xFF66CDAA;
            static constexpr uint32_t MediumBlue            = 0xFF0000CD;
            static constexpr uint32_t MediumOrchid          = 0xFFBA55D3;
            static constexpr uint32_t MediumPurple          = 0xFF9370DB;
            static constexpr uint32_t MediumSeaGreen        = 0xFF3CB371;
            static constexpr uint32_t MediumSlateBlue       = 0xFF7B68EE;
            static constexpr uint32_t MediumSpringGreen     = 0xFF00FA9A;
            static constexpr uint32_t MediumTurquoise       = 0xFF48D1CC;
            static constexpr uint32_t MediumVioletRed       = 0xFFC71585;
            static constexpr uint32_t MidnightBlue          = 0xFF191970;
            static constexpr uint32_t MintCream             = 0xFFF5FFFA;
            static constexpr uint32_t MistyRose             = 0xFFFFE4E1;
            static constexpr uint32_t Moccasin              = 0xFFFFE4B5;
            static constexpr uint32_t NavahoWhite           = 0xFFFFDEAD;
            static constexpr uint32_t Navy                  = 0xFF000080;
            static constexpr uint32_t OldLace               = 0xFFFDF5E6;
            static constexpr uint32_t Olive                 = 0xFF808000;
            static constexpr uint32_t OliveDrab             = 0xFF6B8E23;
            static constexpr uint32_t Orange                = 0xFFFFA500;
            static constexpr uint32_t OrangeRed             = 0xFFFF4500;
            static constexpr uint32_t Orchid                = 0xFFDA70D6;
            static constexpr uint32_t PaleGoldenrod         = 0xFFEEE8AA;
            static constexpr uint32_t PaleGreen             = 0xFF98FB98;
            static constexpr uint32_t PaleTurquoise         = 0xFFAFEEEE;
            static constexpr uint32_t PaleVioletRed         = 0xFFDB7093;
            static constexpr uint32_t PapayaWhip            = 0xFFFFEFD5;
            static constexpr uint32_t PeachPuff             = 0xFFFFDAB9;
            static constexpr uint32_t Peru                  = 0xFFCD853F;
            static constexpr uint32_t Pink                  = 0xFFFFC0CB;
            static constexpr uint32_t Plum                  = 0xFFDDA0DD;
            static constexpr uint32_t PowderBlue            = 0xFFB0E0E6;
            static constexpr uint32_t Purple                = 0xFF800080;
            static constexpr uint32_t RebeccaPurple         = 0xFF663399;
            static constexpr uint32_t Red                   = 0xFFFF0000;
            static constexpr uint32_t RosyBrown             = 0xFFBC8F8F;
            static constexpr uint32_t RoyalBlue             = 0xFF4169E1;
            static constexpr uint32_t SaddleBrown           = 0xFF8B4513;
            static constexpr uint32_t Salmon                = 0xFFFA8072;
            static constexpr uint32_t SandyBrown            = 0xFFF4A460;
            static constexpr uint32_t SeaGreen              = 0xFF2E8B57;
            static constexpr uint32_t SeaShell              = 0xFFFFF5EE;
            static constexpr uint32_t Sienna                = 0xFFA0522D;
            static constexpr uint32_t Silver                = 0xFFC0C0C0;
            static constexpr uint32_t SkyBlue               = 0xFF87CEEB;
            static constexpr uint32_t SlateBlue             = 0xFF6A5ACD;
            static constexpr uint32_t SlateGray             = 0xFF708090;
            static constexpr uint32_t Snow                  = 0xFFFFFAFA;
            static constexpr uint32_t SpringGreen           = 0xFF00FF7F;
            static constexpr uint32_t SteelBlue             = 0xFF4682B4;
            static constexpr uint32_t Tan                   = 0xFFD2B48C;
            static constexpr uint32_t Teal                  = 0xFF008080;
            static constexpr uint32_t Thistle               = 0xFFD8BFD8;
            static constexpr uint32_t Tomato                = 0xFFFF6347;
            static constexpr uint32_t Turquoise             = 0xFF40E0D0;
            static constexpr uint32_t Violet                = 0xFFEE82EE;
            static constexpr uint32_t Wheat                 = 0xFFF5DEB3;
            static constexpr uint32_t White                 = 0xFFFFFFFF;
            static constexpr uint32_t WhiteSmoke            = 0xFFF5F5F5;
            static constexpr uint32_t Yellow                = 0xFFFFFF00;
            static constexpr uint32_t YellowGreen           = 0xFF9ACD32;
        };
    }
}
