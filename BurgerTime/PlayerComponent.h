#pragma once
#include "Component.h"
#include <Scene.h>
#include <GalagaMath.h>
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
        void SetCanMove(GalagaMath::Side side, bool canMove) { m_Movement[side] = canMove; };
        const std::map<GalagaMath::Side, bool>& GetCanMove() { return m_Movement; };

        void CheckMovement(const std::vector<std::pair<SDL_Rect, GameObject*>>& rects, bool isVertical);

    private:
        Scene* m_Scene{};
        bool HasDied{ false };
        float DefaultDeathTimer{ 4 }, DeathTimer{ DefaultDeathTimer };
        bool m_CanMoveHorizontally{ true }, m_CanMoveVertically{ true };
        std::map<GalagaMath::Side, bool> m_Movement{
            std::make_pair(GalagaMath::Side::Top, false),
            std::make_pair(GalagaMath::Side::Left, false),
            std::make_pair(GalagaMath::Side::Right, false),
            std::make_pair(GalagaMath::Side::Bottom, false),
            std::make_pair(GalagaMath::Side::None, true),
        };

        void HandleEnemyOverlap();
        void HandleDeathEnd();
        void HandleRespawn();
    };
}