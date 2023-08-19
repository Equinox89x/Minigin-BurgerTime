#pragma once
#include "Component.h"
#include "BurgerComponent.h"
#include "TextureComponent.h"
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
        
        enum PlayerState {
            ALIVE, DEAD, RESPAWN
        };
        PlayerComponent(Scene* scene, bool isOpposer, glm::vec2 startPos) : m_Scene{ scene }, m_IsOpposer{ isOpposer }, m_StartPos{ startPos } { };
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
        void SetIsController(bool isController) { m_IsController = isController; };

        void CheckHit(GameObject* go)
        {
            if (go->GetComponent<BurgerComponent>()->GetState() == BurgerState::FALLING || go->GetComponent<BurgerComponent>()->GetState() == BurgerState::FALLING_NO_INTERUPT) {
                auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
                rect.h /= 2;
                rect.y -= rect.h;
                auto rect2{ go->GetComponent<TextureComponent>()->GetRect() };
                if (MathLib::IsOverlapping(rect, rect2)) {
                    Die();
                }
            }
        }
    private:
        Scene* m_Scene{};
        bool HasDied{ false };
        float m_DefaultDeathTimer{ 5.5 }, m_DeathTimer{ m_DefaultDeathTimer };
        bool m_CanMoveHorizontally{ true }, m_CanMoveVertically{ true }, m_IsController{ false }, m_IsOpposer{false};

        SDL_Rect m_BottomRect, m_LeftRect, m_Rect, m_BottomLeft, m_BottomRight;
        PlayerState m_PlayerState{ PlayerState::ALIVE };
        glm::vec2 m_StartPos;

        void HandleEnemyOverlap();
        void HandlePlayerOverlap();
        void HandleDeathEnd();
        void HandleRespawn();
    };
}