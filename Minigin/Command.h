#pragma once
#include "Minigin.h"

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		Command(const Command& other) = delete;
		Command(Command&& other) noexcept = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) noexcept = delete;
		explicit Command() = default;

		virtual void Execute() = 0; //you can add deltatime here
	};
}

