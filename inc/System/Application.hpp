#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

inline namespace neoxops {
    namespace graphics {
        class D3DGraphics;
    }

    namespace scene {
        class SceneManager;
    }
    
    namespace system {
        // 전방 선언
        class FPSLimiter;
        class Window;

        /// @brief 응용 프로그램 클래스
        class Application final {
        private:
            HINSTANCE m_hInstance;                          ///< 응용 프로그램의 인스턴스 핸들
            FPSLimiter* m_FPSLimiter;                       ///< FPSLimiter 객체
            Window* m_Window;                               ///< Window 객체
            graphics::D3DGraphics* m_D3DGraphics;           ///< D3DGraphics 객체
            scene::SceneManager* m_SceneMgr;                ///< SceneManager 객체

            void input() noexcept;
            void update() noexcept;
            void render() noexcept;

        public:
            Application() noexcept;
            Application(const Application&) noexcept = delete;
            Application(Application&&) noexcept = delete;
            ~Application() noexcept;

            [[nodiscard]] bool Initialize(HINSTANCE) noexcept;
            void Run() noexcept;

            Application& operator=(const Application&) noexcept = delete;
            Application& operator=(Application&&) noexcept = delete;
        };
    }
}