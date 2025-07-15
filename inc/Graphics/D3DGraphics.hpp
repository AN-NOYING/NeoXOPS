#pragma once

// #define ENABLE_SOFTWARE_RENDER          ///< CPU 기반 소프트웨어 렌더링 활성화

#include <d3d11.h>
#include <directxmath.h>
#include <dxgi.h>
#include <wrl/client.h>

// If you are uisng MinGw, you'll need to add the option to your compiler.
// Example: -ld3d11 -ldxgi -ld3dcompiler
#if defined(_MSC_VER)
    #pragma comment(lib, "d3d11.lib")
    #pragma comment(lib, "dxgi.lib")
    #pragma comment(lib, "d3dcompiler.lib")
#endif

inline namespace neoxops {
    namespace graphics {
        /// @brief Direct3D 그래픽 클래스
        class D3DGraphics final {
        private:
            Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
            Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
            Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
            Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
            Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
            Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterState;

            D3D11_VIEWPORT m_ViewPort;

            bool m_VSyncEnabled;

        public:
            D3DGraphics() noexcept;
            D3DGraphics(const D3DGraphics&) noexcept = delete;
            D3DGraphics(D3DGraphics&&) noexcept = delete;
            ~D3DGraphics() noexcept;

            [[nodiscard]] bool Initialize(HWND, int32_t, int32_t, bool, bool) noexcept;
            [[nodiscard]] bool IsVSyncEnabled() const noexcept;

            void BeginFrame(float color[4]) noexcept;
            void EndFrame() noexcept;

            [[nodiscard]] bool Resize(int32_t, int32_t) noexcept;

            void SetVSync(bool) noexcept;

            [[nodiscard]] ID3D11Device* GetDevice() const noexcept;
            [[nodiscard]] ID3D11DeviceContext* GetDeviceContext() const noexcept;
            [[nodiscard]] ID3D11RenderTargetView* GetRenderTargetView() const noexcept;
            [[nodiscard]] ID3D11DepthStencilView* GetDepthStencilView() const noexcept;

            D3DGraphics& operator=(const D3DGraphics&) noexcept = delete;
            D3DGraphics& operator=(D3DGraphics&&) noexcept = delete;
        };
    }
}