#include "System/FPSLimiter.hpp"
#include <algorithm>
#include <thread>

using namespace system;

/// @brief 생성자
/// @param maxFPS 최대 제한 FPS
FPSLimiter::FPSLimiter(uint32_t maxFPS) noexcept {
    auto initTime       = std::chrono::steady_clock::now();
    m_StartTime         = initTime;
    m_EndTime           = initTime;
    m_FrameStartTime    = initTime;
    m_LastFPSTime       = initTime;

    m_FrameCount        = 0U;
    m_MaxFPS            = std::max(1U, maxFPS);
    m_FPS               = 0.0;
    m_DeltaTime         = 0.0;
}

/// @brief 소멸자
FPSLimiter::~FPSLimiter() noexcept {

}

/// @brief 프레임의 측정을 시작합니다.
void FPSLimiter::StartFrame() noexcept {
    m_FrameStartTime = std::chrono::steady_clock::now();
}

/// @brief 프레임의 측정을 종료한 후, 연산을 수행합니다.
/// @param vsyncEnabled V-Sync 활성화 유무
void FPSLimiter::EndFrame(bool vsyncEnabled) noexcept {    
    // 현재 시각
    auto currentTime = std::chrono::steady_clock::now();

    // 델타 타임
    m_DeltaTime = std::chrono::duration<double>(currentTime - m_FrameStartTime).count();

    // FPS 계산
    ++m_FrameCount;
    if (m_FrameCount >= m_MaxFPS) {
        // 총 경과 시간
        auto totalTime = std::chrono::duration<double>(currentTime - m_StartTime).count();

        // 경과 시간이 0보다 크냐?
        if (totalTime > 0.0) {
            m_FPS = static_cast<double>(m_FrameCount) / totalTime;
        }

        // 초기화 및 갱신
        m_FrameCount = 0U;
        m_StartTime = currentTime;
    }

    // V-Sync 비활성화 일 때
    if (!vsyncEnabled) {
        if (m_SleepUntil.time_since_epoch().count() == 0) {
            m_SleepUntil = currentTime;
        }

        // 1 프레임의 목표 시간
        const auto targetDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(1.0 / m_MaxFPS));

        // 다음 프레임의 시작 시간
        m_SleepUntil += targetDuration;

        // 밀렸다면?
        m_SleepUntil = std::max(m_SleepUntil, std::chrono::steady_clock::now() - MAX_DRIFT);

        // 스레드 대기
        // sleep_for는 비정확함.
        std::this_thread::sleep_until(m_SleepUntil);
    }

    m_EndTime = currentTime;
}

/// @brief 프레임 카운트를 취득합니다.
/// @return 프레임 카운트
uint32_t FPSLimiter::GetFrameCount() const noexcept {
    return m_FrameCount;
}

/// @brief FPS를 취득합니다.
/// @return FPS
double FPSLimiter::GetFPS() const noexcept {
    return m_FPS;
}

/// @brief 델타 타임을 취득합니다.
/// @return 델타 타임
double FPSLimiter::GetDeltaTime() const noexcept {
    return m_DeltaTime;
}

/// @brief 최대 제한 FPS를 설정합니다.
/// @param maxFPS 최대 제한 FPS
void FPSLimiter::SetMaxFPS(uint32_t maxFPS) noexcept {
    m_MaxFPS = std::max(1U, maxFPS);
}