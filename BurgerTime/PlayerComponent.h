#pragma once
#include "Component.h"
#include <Scene.h>
#include <MathLib.h>
#include <map>


namespace dae {


    class PlayerComponent final : public Component
    {

    public:
        enum Movement {
            UP, DOWN, LEFT, RIGHT
        };
        PlayerComponent(Scene* scene) : m_Scene{ scene } { };
        ~PlayerComponent() = default;
        PlayerComponent(const PlayerComponent&) = delete;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent& operator=(const PlayerComponent&) = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;
        //virtual void Initialize() override;
        virtual void Update() override;
        virtual void Render() const override;

        void Die();
        bool IsDead() { return HasDied; };
        bool CanBeGrabbed{ true };

        void SetCanMoveHorizontally(bool canMove) {
            m_CanMoveHorizontally = canMove;
        }
        void SetCanMoveVertically(bool canMove) {
            m_CanMoveVertically = canMove;
        }

        bool GetCanMoveHorizontally() { return m_CanMoveHorizontally; };
        bool GetCanMoveVertically() { return m_CanMoveVertically; };

        void CheckMovement(const std::vector<std::pair<SDL_Rect, GameObject*>>& rects, bool isVertical);

    private:
        Scene* m_Scene{};
        bool HasDied{ false };
        float DefaultDeathTimer{ 4 }, DeathTimer{ DefaultDeathTimer };
        bool m_CanMoveHorizontally{ true }, m_CanMoveVertically{ true };

        SDL_Rect m_BottomRect, m_LeftRect, m_Rect, m_BottomLeft, m_BottomRight;

        void HandleEnemyOverlap();
        void HandleDeathEnd();
        void HandleRespawn();
    };
}