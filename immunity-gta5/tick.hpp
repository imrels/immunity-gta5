#pragma once
#include <vector>
#include <sysinfoapi.h>
#include <functional>

namespace tick
{
	class thread_invoker
	{
	public:

		inline static void queue(std::function<void()> func)
		{
			funcs_to_invoke.emplace_back(std::make_pair(func, GetTickCount64()));
		}

		void on_tick()
		{
			auto current_tick = GetTickCount64();
			for (auto& funcs : funcs_to_invoke)
			{
				if (current_tick - funcs.second < 1000)
				{
					funcs.first();
				}
			}

			funcs_to_invoke.clear();
		}
	private:
		inline static std::vector<std::pair<std::function<void()>, uintptr_t>> funcs_to_invoke;
	};

	inline thread_invoker pnative;
}