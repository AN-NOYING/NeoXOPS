#include "System/Window.hpp"

using namespace system;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* pWindow = nullptr;

    if (uMsg == WM_NCCREATE) {
        auto* pcs = reinterpret_cast<CREATESTRUCT*>(lParam);
        if (pcs) {
            pWindow = reinterpret_cast<Window*>(pcs->lpCreateParams);
            if (pWindow) {
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
                pWindow->SetHandle(hWnd);
            }
        }
    } else {
        pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    return pWindow ? pWindow->MessageHandler(uMsg, wParam, lParam) : DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/// @brief 기본 생성자
Window::Window() noexcept {
    m_hWnd = nullptr;
}

/// @brief 소멸자
Window::~Window() noexcept {
    if (m_hWnd) {
        DestroyWindow(m_hWnd);
        m_hWnd = nullptr;
    }
}

/// @brief 윈도우를 생성합니다.
/// @param hInstance 응용 프로그램의 인스턴스 핸들
/// @param title 타이틀
/// @param width 너비
/// @param height 높이
/// @param fullscreenEnabled 전체화면 활성화 유무
/// @return 성공(true), 실패(false)
bool Window::Create(HINSTANCE hInstance, const char* title, int32_t width, int32_t height, bool fullscreenEnabled) noexcept {
    // 윈도우 클래스 정보
    WNDCLASSEX wc       = {};
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hbrBackground    = nullptr;
    wc.hCursor          = nullptr;
    wc.hIcon            = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm          = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance        = hInstance;
    wc.lpfnWndProc      = WndProc;
    wc.lpszClassName    = "NeoXOPSWindowClass";
    wc.lpszMenuName     = nullptr;
    wc.style            = CS_HREDRAW | CS_VREDRAW;

    // 윈도우 클래스 정보 등록
    if (!RegisterClassEx(&wc)) {
        return false;
    }

    // 윈도우 스타일
    DWORD dwExStyle = 0;
    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
    if (fullscreenEnabled) {
        dwStyle = WS_POPUP;
    }

    // 윈도우 생성
    m_hWnd = CreateWindowEx(
        dwExStyle,
        wc.lpszClassName,
        title,
        dwStyle,
        0,
        0,
        width,
        height,
        nullptr,
        nullptr,
        hInstance,
        this
    );
    if (!m_hWnd) {
        DWORD errorCode = GetLastError();
        return false;
    }

    return true;
}

/// @brief 메시지 핸들러
/// @param uMsg 메시지
/// @param wParam 메시지의 정보
/// @param lParam 메시지의 정보
/// @return 메시지 처리에 따른 값
LRESULT Window::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept {
    switch (uMsg) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        } break;

        case WM_INPUT: {
            
            return 0;
        } break;

        default: return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }
}

/// @brief 윈도우의 좌표를 설정합니다.
/// @param x X 좌표
/// @param y Y 좌표
void Window::SetPosition(int32_t x, int32_t y) noexcept {
    if (m_hWnd) {
        SetWindowPos(m_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
}

/// @brief 윈도우의 크기를 설정합니다.
/// @param width 너비
/// @param height 높이
void Window::SetSize(int32_t width, int32_t height) noexcept {
    if (m_hWnd) {
        SetWindowPos(m_hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
    }
}

/// @brief 윈도우의 타이틀을 설정합니다.
/// @param title 타이틀
void Window::SetTitle(const char* title) noexcept {
    if (m_hWnd && title) {
        SetWindowText(m_hWnd, title);
    }
}

/// @brief 윈도우의 핸들을 설정합니다.
/// @param hWnd 핸들
void Window::SetHandle(HWND hWnd) noexcept {
    if (m_hWnd == nullptr && hWnd != nullptr) {
        m_hWnd = hWnd;
    }
}

/// @brief 윈도우의 핸들을 취득합니다.
/// @return 윈도우의 핸들
HWND Window::GetHandle() const noexcept {
    return m_hWnd;
}