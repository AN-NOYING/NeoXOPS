#include "Graphics/D3DGraphics.hpp"

using namespace graphics;

/// @brief 기본 생성자
D3DGraphics::D3DGraphics() noexcept {
    m_ViewPort      = {};
    m_VSyncEnabled  = true;
}

/// @brief 소멸자
D3DGraphics::~D3DGraphics() noexcept {
    if (m_SwapChain) {
        m_SwapChain->SetFullscreenState(FALSE, nullptr);
    }
}

/// @brief Direct3D 초기화를 수행합니다.
/// @param hWnd 윈도우의 핸들
/// @param width 너비
/// @param height 높이
/// @param fullscreenEnabled 전체화면 활성화 유무
/// @param vsyncEnabled 수직 동기화 활성화 유무
/// @return 성공(true), 실패(false)
bool D3DGraphics::Initialize(HWND hWnd, int32_t width, int32_t height, bool fullscreenEnabled, bool vsyncEnabled) noexcept {
    // V-Sync
    m_VSyncEnabled = vsyncEnabled;

    // 디스플레이 정보 취득
    Microsoft::WRL::ComPtr<IDXGIFactory> factory;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf())))) {
        return false;
    }

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    if (FAILED(factory->EnumAdapters(0, adapter.GetAddressOf()))) {
        return false;
    }

    Microsoft::WRL::ComPtr<IDXGIOutput> adapterOutput;
    if (FAILED(adapter->EnumOutputs(0, adapterOutput.GetAddressOf()))) {
        return false;
    }

    // 출력 디스플레이의 모드 목록 취득
    UINT numModes = 0U;
    if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, nullptr))) {
        return false;
    }

    DXGI_MODE_DESC* modeList = new DXGI_MODE_DESC[numModes];
    if (!modeList) {
        return false;
    }
    if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, modeList))) {
        return false;
    }

    DXGI_RATIONAL refreshRate = { 0, 1 };
    for (int i = 0; i < (int)numModes; ++i) {
        if (modeList[i].Width == width && modeList[i].Height == height) {
            refreshRate = modeList[i].RefreshRate;
            break;
        }
    }

    delete[] modeList;
    modeList = nullptr;

    // 스왑 체인 설명
    DXGI_SWAP_CHAIN_DESC swapChainDesc                  = {};
    swapChainDesc.BufferCount                           = 2;
    swapChainDesc.BufferDesc.Width                      = width;
    swapChainDesc.BufferDesc.Height                     = height;
    swapChainDesc.BufferDesc.Format                     = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator      = vsyncEnabled ? refreshRate.Numerator : 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator    = vsyncEnabled ? refreshRate.Denominator : 1;
    swapChainDesc.BufferUsage                           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow                          = hWnd;
    swapChainDesc.SampleDesc.Count                      = 1;
    swapChainDesc.SampleDesc.Quality                    = 0;
    swapChainDesc.Windowed                              = !fullscreenEnabled;
    swapChainDesc.SwapEffect                            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags                                 = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;      // 일부 드라이버 또는 OS 환경에 따라 강제로 프레임을 제한할 수 있어 방지하기 위해 이 플래그를 사용.

    // 디바이스 플래그
    UINT createDeviceFlags = 0U;
#if defined(_DEBUG) || defined(DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // 피쳐 레벨
    const D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };
    D3D_FEATURE_LEVEL featureLevel;

    // 디바이스 생성    
    if (FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        &swapChainDesc,
        m_SwapChain.GetAddressOf(),
        m_Device.GetAddressOf(),
        &featureLevel,
        m_DeviceContext.GetAddressOf()
    ))) {
#if defined(ENABLE_SOFTWARE_RENDER)
        if (FAILED(D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_WARP,
            nullptr,
            createDeviceFlags,
            featureLevels,
            _countof(featureLevels),
            D3D11_SDK_VERSION,
            &swapChainDesc,
            m_SwapChain.ReleaseAndGetAddressOf(),
            m_Device.ReleaseAndGetAddressOf(),
            &featureLevel,
            m_DeviceContext.ReleaseAndGetAddressOf()
        ))) {
            return false;
        }
#else
        return false;
#endif
    }

    // 백 버퍼로 부터 렌더 타겟 뷰 생성
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    if (FAILED(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)))) {
        return false;
    }
    if (FAILED(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_RenderTargetView.GetAddressOf()))) {
        return false;
    }

    // 깊이 스텐실 버퍼와 뷰 생성
    D3D11_TEXTURE2D_DESC depthBufferDesc    = {};
    depthBufferDesc.Width                   = width;
    depthBufferDesc.Height                  = height;
    depthBufferDesc.MipLevels               = 1;
    depthBufferDesc.ArraySize               = 1;
    depthBufferDesc.Format                  = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count        = 1;
    depthBufferDesc.SampleDesc.Quality      = 0;
    depthBufferDesc.Usage                   = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags               = D3D11_BIND_DEPTH_STENCIL;

    if (FAILED(m_Device->CreateTexture2D(&depthBufferDesc, nullptr, m_DepthStencilBuffer.GetAddressOf()))) {
        return false;
    }

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc       = {};
    depthStencilDesc.DepthEnable                    = TRUE;
    depthStencilDesc.DepthWriteMask                 = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc                      = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable                  = TRUE;
    depthStencilDesc.StencilReadMask                = 0xFF;
    depthStencilDesc.StencilWriteMask               = 0xFF;
    depthStencilDesc.FrontFace.StencilFailOp        = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp   = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp        = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc          = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp         = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp    = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp         = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc           = D3D11_COMPARISON_ALWAYS;

    if (FAILED(m_Device->CreateDepthStencilState(&depthStencilDesc, m_DepthStencilState.GetAddressOf()))) {
        return false;
    }
    m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc  = {};
    depthStencilViewDesc.Format                         = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension                  = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice             = 0;

    if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &depthStencilViewDesc, m_DepthStencilView.GetAddressOf()))) {
        return false;
    }
    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

    // 래스터라이저 상태
    D3D11_RASTERIZER_DESC rasterDesc    = {};
    rasterDesc.AntialiasedLineEnable    = FALSE;
    rasterDesc.CullMode                 = D3D11_CULL_BACK;
    rasterDesc.DepthBias                = 0;
    rasterDesc.DepthBiasClamp           = 0.0f;
    rasterDesc.DepthClipEnable          = TRUE;
    rasterDesc.FillMode                 = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise    = FALSE;
    rasterDesc.MultisampleEnable        = FALSE;
    rasterDesc.ScissorEnable            = FALSE;
    rasterDesc.SlopeScaledDepthBias     = 0.0f;
    
    if (FAILED(m_Device->CreateRasterizerState(&rasterDesc, m_RasterState.GetAddressOf()))) {
        return false;
    }
    m_DeviceContext->RSSetState(m_RasterState.Get());

    // 뷰포트
    m_ViewPort.Width    = static_cast<float>(width);
    m_ViewPort.Height   = static_cast<float>(height);
    m_ViewPort.MinDepth = 0.0f;
    m_ViewPort.MaxDepth = 1.0f;
    m_ViewPort.TopLeftX = 0.0f;
    m_ViewPort.TopLeftY = 0.0f;

    m_DeviceContext->RSSetViewports(1, &m_ViewPort);
    
    return true;
}

/// @brief V-Sync 활성화 유무를 취득합니다.
/// @return 활성화(true), 비활성화(false)
bool D3DGraphics::IsVSyncEnabled() const noexcept {
    return m_VSyncEnabled;
}

/// @brief 렌더 타겟과 깊이 버퍼를 초기화합니다.
/// @param color 색상
void D3DGraphics::BeginFrame(float color[4]) noexcept {
    // 렌더 타겟을 설정한 색상으로 클리어
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);

    // 깊이 스텐실 버퍼 초기화
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

/// @brief 화면에 출력합니다.
void D3DGraphics::EndFrame() noexcept {
    m_SwapChain->Present(m_VSyncEnabled ? 1 : 0, 0);
}

/// @brief 화면의 크기를 변경합니다.
/// @param width 너비
/// @param height 높이
bool D3DGraphics::Resize(int32_t width, int32_t height) noexcept {
    if (!m_Device || !m_DeviceContext || !m_SwapChain) {
        return false;
    }

    // 렌더 타겟과 깊이 스텐실 뷰의 바인딩을 해제
    m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

    // 기존의 리소스 해제
    m_RenderTargetView.Reset();
    m_DepthStencilView.Reset();
    m_DepthStencilBuffer.Reset();

    // 스왑 체인의 버퍼 크기 조정
    if (FAILED(m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0))) {
        return false;
    }

    // 백 버퍼로부터 렌더 타겟 뷰 생성
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    if (FAILED(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)))) {
        return false;
    }
    if (FAILED(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_RenderTargetView.GetAddressOf()))) {
        return false;
    }

    // 깊이 스텐실 버퍼 생성
    D3D11_TEXTURE2D_DESC depthBufferDesc    = {};
    depthBufferDesc.Width                   = width;
    depthBufferDesc.Height                  = height;
    depthBufferDesc.MipLevels               = 1;
    depthBufferDesc.ArraySize               = 1;
    depthBufferDesc.Format                  = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count        = 1;
    depthBufferDesc.SampleDesc.Quality      = 0;
    depthBufferDesc.Usage                   = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags               = D3D11_BIND_DEPTH_STENCIL;

    if (FAILED(m_Device->CreateTexture2D(&depthBufferDesc, nullptr, m_DepthStencilBuffer.GetAddressOf()))) {
        return false;
    }
    if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, m_DepthStencilView.GetAddressOf()))) {
        return false;
    }
    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
    
    // 뷰포트
    m_ViewPort.Width    = static_cast<float>(width);
    m_ViewPort.Height   = static_cast<float>(height);
    m_ViewPort.MinDepth = 0.0f;
    m_ViewPort.MaxDepth = 1.0f;
    m_ViewPort.TopLeftX = 0.0f;
    m_ViewPort.TopLeftY = 0.0f;

    m_DeviceContext->RSSetViewports(1, &m_ViewPort);

    return true;
}

/// @brief V-Sync 활성화를 설정합니다.
/// @param enabled 활성화 유무
void D3DGraphics::SetVSync(bool enabled) noexcept {
    m_VSyncEnabled = enabled;
}

/// @brief Direct3D 디바이스를 취득합니다.
/// @return Direct3D 디바이스
ID3D11Device* D3DGraphics::GetDevice() const noexcept {
    return m_Device.Get();
}

/// @brief Direct3D 디바이스 컨텍스트를 취득합니다.
/// @return Direct3D 디바이스 컨텍스트
ID3D11DeviceContext* D3DGraphics::GetDeviceContext() const noexcept {
    return m_DeviceContext.Get();
}

/// @brief 렌더 타겟 뷰를 취득합니다.
/// @return 렌더 타겟 뷰
ID3D11RenderTargetView* D3DGraphics::GetRenderTargetView() const noexcept
{
    return m_RenderTargetView.Get();
}

/// @brief 깊이 스텐실 뷰를 취득합니다.
/// @return 깊이 스텐실 뷰
ID3D11DepthStencilView* D3DGraphics::GetDepthStencilView() const noexcept
{
    return m_DepthStencilView.Get();
}