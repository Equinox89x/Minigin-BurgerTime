#include "PlayerComponent.h"
#include "GameObject.h"
#include <TextureComponent.h>
#include <ValuesComponent.h>
#include <Timer.h>
#include <Renderer.h>
#include "AudioComponent.h"
#include <MoveKeyboardComponent.h>

void dae::PlayerComponent::Update()
{
    if (HasDied) {
        auto deltaTime{ Timer::GetInstance().GetDeltaTime() };
        DeathTimer -= deltaTime;
        if (DeathTimer < DefaultDeathTimer - 0.3f) {
            HandleDeathEnd();
        }
        if (DeathTimer <= 0) {
            HandleRespawn();

        }
    }
    else {
        HandleEnemyOverlap();
    }
}

void dae::PlayerComponent::Render() const
{

    auto r = GetGameObject()->GetComponent<TextureComponent>()->GetRect();
    auto charRect{ SDL_Rect{r.x, r.y, r.w, r.h } };
    auto charRectLeft{ charRect };
    charRectLeft.w /= 5;
    charRectLeft.x -= charRectLeft.w;
    charRectLeft.h /= 2;
    charRectLeft.y += charRect.h/4;

    auto charRectRight{ charRect };
    charRectRight.x += charRect.w;
    charRectRight.w /= 5;
    charRectRight.h /= 2;
    charRectRight.y += charRect.h/4;

    auto charRectBottom{ charRect };
    charRectBottom.h /= 5;
    charRectBottom.y += charRect.h;
    charRectBottom.w /= 2;
    charRectBottom.x += charRect.w/4;

    SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
    SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &charRectLeft);
    SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255); // Set the color to red
    SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &charRectRight);
    SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 255); // Set the color to red
    SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &charRectBottom);
}

void dae::PlayerComponent::CheckMovement(const std::vector<std::pair<SDL_Rect, GameObject*>>& rects, bool /*isVertical*/)
{
    GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(GalagaMath::Side::Right, true);
    GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(GalagaMath::Side::Left, true);
    GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(GalagaMath::Side::Top, true);
    GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(GalagaMath::Side::Bottom, true);

    auto r = GetGameObject()->GetComponent<TextureComponent>()->GetRect();
    for (auto item : rects)
    {
        auto charRect{ SDL_Rect{r.x, r.y, r.w, r.h } };
        auto charRectLeft{ charRect };
        charRectLeft.w /= 5;
        charRectLeft.x -= charRectLeft.w;
        charRectLeft.h /= 2;
        charRectLeft.y += charRect.h / 4;

        auto charRectRight{ charRect };
        charRectRight.x += charRect.w;
        charRectRight.w /= 5;
        charRectRight.h /= 2;
        charRectRight.y += charRect.h / 4;

        auto charRectBottom{ charRect };
        charRectBottom.h /= 5;
        charRectBottom.y += charRect.h;
        charRectBottom.w /= 2;
        charRectBottom.x += charRect.w / 4;

        if (GalagaMath::IsOverlapping(charRectRight, item.first)) { 
            GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(GalagaMath::Side::Right, false);
            break;
        }
        if (GalagaMath::IsOverlapping(charRectLeft, item.first)) {
            GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(GalagaMath::Side::Left, false);
            break;
        }
        if (GalagaMath::IsOverlapping(charRectBottom, item.first)) {
            GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(GalagaMath::Side::Left, true);
            GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(GalagaMath::Side::Right, true);
              break;}
    }
}


void dae::PlayerComponent::HandleEnemyOverlap()
{
    auto go{ m_Scene->GetGameObject(EnumStrings[EnemyHolder]) };
    if (go) {
        auto children{ go->GetChildren(EnumStrings[Enemy]) };
        auto rect1{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
        for (auto enemy : children) {
            if (enemy->IsMarkedForDestroy()) continue;
            auto rect2{ enemy->GetComponent<TextureComponent>(EnumStrings[Enemy])->GetRect() };
            if (GalagaMath::IsOverlapping(rect1, rect2)) {
                Die();
                enemy->MarkForDestroy();
                return;
            }
        }
    }
}

void dae::PlayerComponent::HandleDeathEnd()
{
    auto player{ GetGameObject() };
    player->GetComponent<TextureComponent>()->SetIsVisible(false);
    player->GetComponent<TextureComponent>()->Scale(0.7f, 0.7f);
    player->GetComponent<TextureComponent>()->SetTexture(player->GetName() == EnumStrings[Player0] ? "galaga.png" : "galagaRed.png");
    auto rect = player->GetComponent<TextureComponent>()->GetRect();
    player->GetComponent<TextureComponent>()->SetOffset({ (rect.w * 1.5f) - 1.5f, 0.f });
}

void dae::PlayerComponent::HandleRespawn()
{
    DeathTimer = DefaultDeathTimer;
    HasDied = false;
    auto player{ GetGameObject() };
    player->GetComponent<TextureComponent>()->SetIsVisible(true);
    player->GetComponent<TextureComponent>()->SetPosition((GameWindowSizeX) / 2 - Margin, WindowSizeY - Margin * 3);
    GetGameObject()->EnableCollision(true);
    CanBeGrabbed = true;
}

void dae::PlayerComponent::Die()
{
    HasDied = true;
    auto player{ GetGameObject() };
    player->GetComponent<TextureComponent>()->Scale(2.5f, 2.5f);
    player->GetComponent<TextureComponent>()->SetTexture("playerExplosion.png", 0.1f, 4);
    auto rect = player->GetComponent<TextureComponent>()->GetRect();
    player->GetComponent<TextureComponent>()->SetOffset({ -rect.w / 2, -rect.h / 2 });
    auto values{ m_Scene->GetGameObject(EnumStrings[Values])->GetComponent<ValuesComponent>() };
    values->Damage();

    player->EnableCollision(false);

    m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayPlayerDeathSound(false);
}