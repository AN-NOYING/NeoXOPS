#pragma once

#include <chrono>
#include "Types.hpp"

inline namespace neoxops {
    namespace system {
        /// @brief FPS 관리 클래스
        class FPSLimiter final {
        private:
            static constexpr double FPS_UPDATE_INTERVAL = 1.0;                          ///< FPS 계산을 위한 업데이트 간격 (초 단위)
            static constexpr auto MAX_DRIFT = std::chrono::milliseconds(10);            ///< 최대 드리프트(시간 지연) 허용 시간

            std::chrono::steady_clock::time_point m_SleepUntil;
            std::chrono::steady_clock::time_point m_StartTime;                          ///< 전체 FPS 계산 시간
            std::chrono::steady_clock::time_point m_EndTime;                            ///< 이전 프레임의 종료 시간
            std::chrono::steady_clock::time_point m_FrameStartTime;                     ///< 현재 프레임의 시작 시간
            std::chrono::steady_clock::time_point m_LastFPSTime;                        ///< 마지막으로 FPS를 업데이트한 시간

            uint32_t    m_FrameCount;                                                   ///< FPS 계산을 위한 프레임 카운트
            uint32_t    m_MaxFPS;                                                       ///< 최대 제한 FPS
            double      m_FPS;                                                          ///< 측정된 FPS
            double      m_DeltaTime;                                                    ///< 마지막 프레임이 완료되기 까지 걸린 시간 (델타 타임)

        public:
            FPSLimiter(uint32_t maxFPS = 60) noexcept;
            FPSLimiter(const FPSLimiter&) noexcept = delete;
            FPSLimiter(FPSLimiter&&) noexcept = delete;
            ~FPSLimiter() noexcept;

            void StartFrame() noexcept;
            void EndFrame(bool) noexcept;

            [[nodiscard]] uint32_t GetFrameCount() const noexcept;
            [[nodiscard]] double GetFPS() const noexcept;
            [[nodiscard]] double GetDeltaTime() const noexcept;

            void SetMaxFPS(uint32_t) noexcept;

            FPSLimiter& operator=(const FPSLimiter&) noexcept = delete;
            FPSLimiter& operator=(FPSLimiter&&) noexcept = delete;
        };
    }
}