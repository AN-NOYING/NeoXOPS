#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "SceneBase.hpp"

inline namespace neoxops {
    namespace scene {
        /// @brief 장면 관리자 클래스
        class SceneManager final {
        private:
            /// @brief 장면 엔트리
            struct SceneEntry final {
                std::string Name;                       ///< 장면의 이름
                std::unique_ptr<SceneBase> Scene;       ///< 장면
                bool IsPause;                           ///< 일시정지
            };
        
            std::unordered_map<std::string, std::function<std::unique_ptr<SceneBase>()>> m_SceneRegistry;           ///< 장면 등록 레지스트리
            std::vector<SceneEntry> m_SceneStack;                                                                   ///< 장면 스택
        
        public:
            SceneManager() noexcept;
            SceneManager(const SceneManager&) noexcept = delete;
            SceneManager(SceneManager&&) noexcept = delete;
            ~SceneManager() noexcept;

            [[nodiscard]] bool AddScene(std::string_view, std::function<std::unique_ptr<SceneBase>()>) noexcept;
            [[nodiscard]] bool LoadScene(std::string_view) noexcept;
            [[nodiscard]] bool RemoveScene(std::string_view) noexcept;
            [[nodiscard]] bool ChangeScene(std::string_view) noexcept;
            [[nodiscard]] bool ReloadScene() noexcept;

            [[nodiscard]] bool Pause() noexcept;
            [[nodiscard]] bool Resume() noexcept;

            [[nodiscard]] SceneBase* GetCurrentScene() const noexcept;

            void Input() noexcept;
            void Update(double) noexcept;
            void Render() noexcept;

            SceneManager& operator=(const SceneManager&) noexcept = delete;
            SceneManager& operator=(SceneManager&&) noexcept = delete;
        };
    }
}