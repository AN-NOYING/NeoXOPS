#include "Scene/SceneManager.hpp"

using namespace scene;

/// @brief 기본 생성자
SceneManager::SceneManager() noexcept {

}

/// @brief 소멸자
SceneManager::~SceneManager() noexcept {
    while (!m_SceneStack.empty()) {
        auto& top = m_SceneStack.back();
        top.Scene->OnExit();
        top.Scene->OnDestroy();
        m_SceneStack.pop_back();
    }

    // m_SceneRegistry.clear();
}

/// @brief 장면을 추가합니다.
/// @param sceneName 장면의 이름
/// @param sceneFunc 장면 생성 함수
/// @return 성공(true), 실패(false)
bool SceneManager::AddScene(std::string_view sceneName, std::function<std::unique_ptr<SceneBase>()> sceneFunc) noexcept {
    auto [it, inserted] = m_SceneRegistry.emplace(std::string(sceneName), std::move(sceneFunc));
    return inserted;
}

/// @brief 장면을 불러옵니다.
/// @param sceneName 장면의 이름
/// @return 성공(true), 실패(false)
/// @note 현재 활성화된 장면이 있다면 일시 정지합니다. 일시 정지가 아닌 파괴를 원한다면 ChangeScene을 호출해주세요.
bool SceneManager::LoadScene(std::string_view sceneName) noexcept {
    // 미등록 장면
    auto it = m_SceneRegistry.find(std::string(sceneName));
    if (it == m_SceneRegistry.end()) {
        return false;
    }

    // 현재 활성화된 장면 일시정지
    if (!m_SceneStack.empty() && !m_SceneStack.back().IsPause) {
        m_SceneStack.back().Scene->OnPause();
        m_SceneStack.back().IsPause = true;
    }

    // 새 장면 생성 및 초기화
    auto newScene = it->second();
    newScene->OnCreate();
    newScene->OnEnter();

    m_SceneStack.push_back({ std::string(sceneName), std::move(newScene), false });
    return true;
}

/// @brief 장면을 제거합니다.
/// @param sceneName 장면의 이름
/// @return 성공(true), 실패(false)
bool SceneManager::RemoveScene(std::string_view sceneName) noexcept {
    for (auto it = m_SceneStack.begin(); it != m_SceneStack.end(); ++it) {
        if (it->Name == sceneName) {
            // 자원 정리
            it->Scene->OnExit();
            it->Scene->OnDestroy();
            
            // 제거
            m_SceneStack.erase(it);

            // 최상위 장면이 있다면 재개
            if (!m_SceneStack.empty() && m_SceneStack.back().IsPause) {
                m_SceneStack.back().Scene->OnResume();
                m_SceneStack.back().IsPause = false;
            }

            return true;
        }
    }

    return false;
}

/// @brief 장면을 교체합니다.
/// @param sceneName 장면의 이름
/// @return 성공(true), 실패(false)
bool SceneManager::ChangeScene(std::string_view sceneName) noexcept {
    // 미등록 장면
    auto it = m_SceneRegistry.find(std::string(sceneName));
    if (it == m_SceneRegistry.end()) {
        return false;
    }
    
    // 자원 정리 후 제거
    if (!m_SceneStack.empty()) {
        m_SceneStack.back().Scene->OnExit();
        m_SceneStack.back().Scene->OnDestroy();
        m_SceneStack.pop_back();
    }

    // 새 장면 생성 및 초기화
    auto newScene = it->second();
    newScene->OnCreate();
    newScene->OnEnter();

    m_SceneStack.push_back({ std::string(sceneName), std::move(newScene), false });
    return true;
}

/// @brief 현재 장면을 다시 불러옵니다.
/// @return 성공(true), 실패(false)
bool SceneManager::ReloadScene() noexcept {
    if (m_SceneStack.empty()) { return false; }

    // 현재 장면
    auto& entry = m_SceneStack.back();

    // 자원 정리 및 해제
    entry.Scene->OnExit();
    entry.Scene->OnDestroy();
    entry.Scene.reset();

    // 장면 생성 함수
    auto it = m_SceneRegistry.find(entry.Name);
    if (it == m_SceneRegistry.end()) {
        return false;
    }

    // 새 장면 생성 및 초기화
    auto newScene = it->second();
    newScene->OnCreate();
    newScene->OnEnter();

    // 이동
    entry.Scene = std::move(newScene);
    entry.IsPause = false;

    return true;
}

/// @brief 장면을 일시 정지합니다.
/// @return 성공(true), 실패(false)
bool SceneManager::Pause() noexcept {
    if (m_SceneStack.empty()) { return false; }

    // 현재 장면이 이미 멈춰있다면...
    auto& entry = m_SceneStack.back();
    if (entry.IsPause) { return false; }

    // 일시 정지
    entry.Scene->OnPause();
    entry.IsPause = true;

    return true;
}

/// @brief 장면을 재개합니다.
/// @return 성공(true), 실패(false)
bool SceneManager::Resume() noexcept {
    if (m_SceneStack.empty()) { return false; }

    // 현재 장면이 안 멈춰있다면...
    auto& entry = m_SceneStack.back();
    if (!entry.IsPause) { return false; }

    entry.Scene->OnResume();
    entry.IsPause = false;

    return true;
}

/// @brief 현재 장면을 취득합니다.
/// @return 장면
SceneBase* SceneManager::GetCurrentScene() const noexcept {
    return (m_SceneStack.empty()) ? nullptr : m_SceneStack.back().Scene.get();
}

/// @brief 입력 처리를 수행합니다.
void SceneManager::Input() noexcept {
    if (!m_SceneStack.empty()) {
        auto& entry = m_SceneStack.back();
        if (!entry.IsPause) {
            entry.Scene->Input();
        }
    }
}

/// @brief 갱신 처리를 수행합니다.
/// @param dt 델타 타임
void SceneManager::Update(double dt) noexcept {
    if (!m_SceneStack.empty()) {
        auto& entry = m_SceneStack.back();
        if (!entry.IsPause) {
            entry.Scene->Update(dt);
        }
    }
}

/// @brief 렌더링 처리를 수행합니다.
void SceneManager::Render() noexcept {
    if (!m_SceneStack.empty()) {
        auto& entry = m_SceneStack.back();
        if (!entry.IsPause) {
            entry.Scene->Render();
        }
    }
}