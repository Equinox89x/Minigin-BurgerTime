#pragma once
#include <SDL_rect.h>
#include <glm/glm.hpp>
//#include <glm/gtc/random.hpp>
#include <glm/ext/vector_float2.hpp>
#include <vector>
#include <cmath>
#include <random>


namespace GalagaMath {

	inline float Square(float val) {
		return val * val;
	}

	inline int BinomialCoeff(int n, int i) {
		if (i == 0 || i == n)
			return 1;

		int numerator = 1;
		int denominator = 1;

		for (int j = 1; j <= i; j++) {
			numerator *= (n - j + 1);
			denominator *= j;
		}

		return numerator / denominator;
	}

	inline glm::vec2 CalculateBezierPoint(float t, const std::vector<glm::vec2>& controlPoints) {
		int n{ static_cast<int>(controlPoints.size()) - 1 };
		float x{ 0.0 };
		float y{ 0.0 };

		//Apply the quadtratic bezier formula as many times as there are controlPoint values
		for (int i = 0; i <= n; i++) {
			float blend{ static_cast<float>(BinomialCoeff(n, i) * pow(1 - t, n - i) * pow(t, i)) };
			x += controlPoints[i].x * blend;
			y += controlPoints[i].y * blend;
		}

		return { x, y };
	}

	inline bool IsOverlapping(const SDL_Rect& source, const SDL_Rect& target) {
		bool doesXOverlap = (source.x < target.x + target.w) && (source.x + source.w > target.x);
		bool doesYOverlap = (source.y < target.y + target.h) && (source.y + source.h > target.y);
		return doesXOverlap && doesYOverlap;
	}

	inline bool IsCompletelyOverlapping(const SDL_Rect& rect1, const SDL_Rect& rect2) {
		// Calculate the coordinates of the bottom-right corners for both rectangles
		int rect1Right = rect1.x + rect1.w;
		int rect1Bottom = rect1.y + rect1.h;

		int rect2Right = rect2.x + rect2.w;
		int rect2Bottom = rect2.y + rect2.h;

		// Check if rect1 is completely to the left of rect2 or completely to the right of rect2
		if (rect1.x >= rect2Right || rect2.x >= rect1Right) {
			return false;
		}

		// Check if rect1 is completely above rect2 or completely below rect2
		if (rect1.y >= rect2Bottom || rect2.y >= rect1Bottom) {
			return false;
		}

		// If none of the above conditions are met, then rect1 and rect2 are overlapping
		return true;
	}

	enum class Side {
		None,
		Top,
		Bottom,
		Left,
		Right
	};

	inline Side GetNonOverlappingSide(const SDL_Rect& rect1, const SDL_Rect& rect2) {
		// Calculate the coordinates of the edges for both rectangles
		int rect1Right = rect1.x + rect1.w;
		int rect1Bottom = rect1.y + rect1.h;

		int rect2Right = rect2.x + rect2.w;
		int rect2Bottom = rect2.y + rect2.h;

		// Check if rect1 is completely to the left of rect2
		if (rect1Right <= rect2.x) {
			return Side::Left;
		}

		// Check if rect1 is completely to the right of rect2
		if (rect1.x >= rect2Right) {
			return Side::Right;
		}

		// Check if rect1 is completely above rect2
		if (rect1Bottom <= rect2.y) {
			return Side::Top;
		}

		// Check if rect1 is completely below rect2
		if (rect1.y >= rect2Bottom) {
			return Side::Bottom;
		}

		// If none of the above conditions are met, the squares are overlapping or intersecting
		return Side::None;
	}

	inline glm::vec2 MoveRectTowards(SDL_Rect& rect1, const SDL_Rect& rect2, float speed)
	{
		glm::vec2 rect1Center(rect1.x + rect1.w / 2, rect1.y + rect1.h / 2);
		glm::vec2 rect2Center(rect2.x + rect2.w / 2, rect2.y + rect2.h / 2);

		glm::vec2 direction = glm::normalize(rect2Center - rect1Center);

		return direction * speed;
	}

	inline float CalculateChance(float min = 0.f, float max = 1.f) {
		std::random_device randomDevice;
		std::mt19937 generatedNr(randomDevice());
		std::uniform_real_distribution<float> distributeVal(min, max);
		return distributeVal(generatedNr);
	}
}