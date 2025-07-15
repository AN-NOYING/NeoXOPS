#include "Scene/SceneManager.hpp"
#include "System/Application.hpp"
#include "System/FPSLimiter.hpp"
#include "System/Window.hpp"
#include "Graphics/D3DGraphics.hpp"

using namespace graphics;
using namespace scene;
using namespace system;

/// @brief 기본 생성자
Application::Application() noexcept {
    m_hInstance     = nullptr;
    m_FPSLimiter    = nullptr;
    m_Window        = nullptr;
    m_D3DGraphics   = nullptr;
    m_SceneMgr      = nullptr;
}

/// @brief 소멸자
Application::~Application() noexcept {
    if (m_SceneMgr) {
        delete m_SceneMgr;
        m_SceneMgr = nullptr;
    }
    
    if (m_FPSLimiter) {
        delete m_FPSLimiter;
        m_FPSLimiter = nullptr;
    }
    
    if (m_D3DGraphics) {
        delete m_D3DGraphics;
        m_D3DGraphics = nullptr;
    }
    
    if (m_Window) {
        delete m_Window;
        m_Window = nullptr;
    }
}

void Application::input() noexcept {
    m_SceneMgr->Input();
}

void Application::update() noexcept {
    m_SceneMgr->Update(m_FPSLimiter->GetDeltaTime());
}

void Application::render() noexcept {
    float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_D3DGraphics->BeginFrame(color);
    m_SceneMgr->Render();
    m_D3DGraphics->EndFrame();
}

/// @brief 응용 프로그램을 초기화합니다.
/// @param hInstance 응용 프로그램의 인스턴스 핸들
/// @return 성공(true), 실패(false)
bool Application::Initialize(HINSTANCE hInstance) noexcept {
    m_hInstance = hInstance;

    // 윈도우 생성
    m_Window = new Window();
    if (m_Window && !m_Window->Create(hInstance, "NeoXOPS", 640, 480, false)) {
        return false;
    }

    // Direct3D 초기화
    m_D3DGraphics = new D3DGraphics();
    if (m_D3DGraphics && !m_D3DGraphics->Initialize(m_Window->GetHandle(), 640, 480, false, false)) {
        return false;
    }

    // FPSLimiter 초기화
    m_FPSLimiter = new FPSLimiter();
    if (!m_FPSLimiter) {
        return false;
    }

    // 장면 관리자 초기화
    m_SceneMgr = new SceneManager();
    if (!m_SceneMgr) {
        return false;
    }

    return true;
}

/// @brief 응용 프로그램을 구동합니다.
void Application::Run() noexcept {
    // 윈도우 표기
    HWND windowHandle = m_Window->GetHandle();
    ShowWindow(windowHandle, SW_SHOW);
    UpdateWindow(windowHandle);

    // 루프
    MSG msg = {};
    
    while (true) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // TODO:
        m_FPSLimiter->StartFrame();

        this->input();
        this->update();
        this->render();

        m_FPSLimiter->EndFrame(m_D3DGraphics->IsVSyncEnabled());
    }
}