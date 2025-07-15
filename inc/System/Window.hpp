#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Types.hpp"

inline namespace neoxops {
    namespace system {        
        /// @brief Window 클래스
        class Window final {
        private:
            HWND m_hWnd;                        ///!< 윈도우의 핸들

        public:
            Window() noexcept;
            Window(const Window&) noexcept = delete;
            Window(Window&&) noexcept = delete;
            ~Window() noexcept;

            [[nodiscard]] bool Create(HINSTANCE, const char*, int32_t, int32_t, bool) noexcept;
            LRESULT MessageHandler(UINT, WPARAM, LPARAM) noexcept;

            void SetPosition(int32_t, int32_t) noexcept;
            void SetSize(int32_t, int32_t) noexcept;
            void SetTitle(const char*) noexcept;
            void SetHandle(HWND) noexcept;
            
            [[nodiscard]] HWND GetHandle() const noexcept;

            Window& operator=(const Window&) noexcept = delete;
            Window& operator=(Window&&) noexcept = delete;
        };
    }
}