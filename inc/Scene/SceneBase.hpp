#pragma once

inline namespace neoxops {
    namespace scene {
        /// @brief 장면 기반 클래스
        class SceneBase {
        protected:
            virtual void onPreRender()  noexcept = 0;
            virtual void onRender3D()   noexcept = 0;
            virtual void onRender2D()   noexcept = 0;
            virtual void onPostRender() noexcept = 0;

        public:
            virtual ~SceneBase()        noexcept = default;

            virtual bool OnCreate()     noexcept = 0;
            virtual void OnDestroy()    noexcept = 0;
            virtual void OnEnter()      noexcept = 0;
            virtual void OnExit()       noexcept = 0;
            virtual void OnPause()      noexcept = 0;
            virtual void OnResume()     noexcept = 0;

            virtual void Input()        noexcept = 0;
            virtual void Update(double) noexcept = 0;
            virtual void Render()       noexcept = 0;
        };
    }
}