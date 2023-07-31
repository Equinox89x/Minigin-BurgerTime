#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <SDL.h>
#include <typeindex>
#include <glm/ext/vector_float3.hpp>
#include "Component.h"

#define WindowSizeX 720
#define GameWindowSizeX WindowSizeX-0
#define WindowSizeY 800
#define WindowBuffer 80
#define Margin 40.f
#define SubMargin Margin * 2
#define Cellsize 48.3f
#define PlayerSize 14.f

	const enum Names {
		Player0,
		Player1,
		PlayerGeneral,
		Enemy,
		EnemyHolder,
		ScoreBoard,
		Score,
		Life,
		Values,
		Ladders,
		Platforms,
		PattyTop,
		PattyBottom,
		Veggie,
		Burger,
		MainMenu,
		EndScreen,
		Selector,
		Logo,
		Global,
		Opposer,
		Plates,
		ScoreHolder,
		BurgerManager,
		NUM_ENUM_VALUES
	};

	const std::string EnumStrings[NUM_ENUM_VALUES] = {
	"MainPlayer0", "MainPlayer1", "MainPlayer",
	"Enemy", "EnemyHolder",
	"ScoreBoard", "Score", "Life", "Values",
	"Ladders", "Platforms", "PattyTop", "PattyBottom", "Veggie", "Burger",
	"MainMenu", "EndScreen", "Selector", "Logo",
	"Global", "Opposer", "Plates", "ScoreHolder", "BurgerManager", 
	};

	enum class Stages {
		Stage1,
		Stage2,
		Stage3,
	};

namespace dae
{
	class Texture2D;
	class Component;
	class TransformComponent;

	class GameObject final
	{
	public:

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Initialize();
		virtual void PostInitialize();
		virtual void Update();
		virtual void Render() const;

		void MarkForDestroy() { MarkedForDelete = true; };
		bool IsMarkedForDestroy() { return MarkedForDelete; };
		void EnableCollision(bool isEnabled) { CollisionEnabled = isEnabled; };
		bool IsCollisionEnabled() { return CollisionEnabled; };

		template<typename Comp>
		//std::enable_if_t<std::is_base_of_v<Component, Comp>, Comp*>
		Comp* AddComponent(std::unique_ptr<Comp> comp) {
			if (comp) {
				//auto component{ std::make_unique<Component*>(comp) }
				comp->m_pGameObject = this;
				dynamic_cast<Component*>(comp.get())->Initialize();
				dynamic_cast<Component*>(comp.get())->PostInitialize();
				m_pComponents.push_back(std::move(comp));
				return dynamic_cast<Comp*>(comp.get());
			}
			return nullptr;
		}

		//template <typename Comp> 
		//Comp* AddComponent(std::unique_ptr<Comp> comp) {
		//	if (comp.get()) {
		//		m_pComponents.push_back(std::move(comp));
		//	}
		//	return comp;
		//}

		template<typename Comp>
		Comp* GetComponent() {
			const type_info& ti = typeid(Comp);
			for (const auto& component : m_pComponents) {
				if (component && typeid(*component.get()) == ti)
					return static_cast<Comp*>(component.get());
			}
			return nullptr;
		}
		
		template<typename Comp>
		Comp* GetComponent(std::string name) {
			const type_info& ti = typeid(Comp);
			for (const auto& component : m_pComponents)
			{
				if (component && typeid(*component.get()) == ti && component->GetName() == name)
					return static_cast<Comp*>(component.get());
			}
			return nullptr;
		}

		void RemoveComponent(const std::unique_ptr<Component>& comp);

		void SetParent(GameObject* const parent);
		GameObject* GetParent() const;
		void RemoveChild(GameObject* pObject);
		void AddChild(GameObject* const go);
		void SetName(std::string name) { m_ObjectName = name; };
		std::string GetName() { return m_ObjectName; };

		TransformComponent* GetTransform() const { return m_pTransform; };
		std::vector<GameObject*>& GetChildren() { return m_pChildren; };
		GameObject* GetChild(std::string name);
		std::vector<GameObject*> GetChildren(std::string name);


		void SetIsHidden(bool isHidden);

	private:
		std::vector<GameObject*> m_pChildren{ std::vector<GameObject*>() };
		std::vector<std::unique_ptr<Component>> m_pComponents{ };
		GameObject* m_pParent{ nullptr };
		TransformComponent* m_pTransform{};
		std::string m_ObjectName{};
		bool m_IsHidden{ false };
		bool MarkedForDelete{ false }, NeedsUpdate{ true }, CollisionEnabled{ true };
	};
}
